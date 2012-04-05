require 'diff_columns'
require 'row_change'

class SqlCompare
  def initialize(db1,db2)
    @db1 = db
    @db2 = db2
    @table1 = nil
    @table2 = nil
    @single_db = false
    raise "not implemented yet"
  end

  def initialize(db,table1,table2)
    @db1 = db
    @db2 = db.clone
    @table1 = table1
    @table2 = table2
    @single_db = true
  end

  def set_output(patch)
    @patch = patch
  end

  def apply
    apply_single
  end

  # We are not implementing full comparison, just an adequate subset
  # for easy cases (a table with a trustworthy primary key, and constant
  # columns).  Make sure we are not trying to do something we're not ready 
  # for.
  def validate_schema
    all_cols1 = @db1.column_names(@table1)
    all_cols2 = @db2.column_names(@table2)
    if all_cols1 != all_cols2
      raise "Columns do not match, please use full coopy toolbox"
    end

    key_cols1 = @db1.primary_key(@table1)
    key_cols2 = @db2.primary_key(@table2)
    if key_cols1 != key_cols2
      raise "Primary keys do not match, please use full coopy toolbox"
    end
  end

  def keyify(lst)
    lst.map{|x| x.to_s}.join("___")
  end

  # When working within a single database, we can delegate more work to SQL.
  # So we specialize this case.
  def apply_single
    validate_schema

    # Prepare some lists of columns.
    key_cols = @db1.primary_key(@table1)
    data_cols = @db1.except_primary_key(@table1)
    all_cols = @db1.column_names(@table1)

    # Let our public know we are beginning.
    @patch.begin_diff

    # Advertise column names.
    @rc_columns = DiffColumns.new
    @rc_columns.title_row = all_cols
    @rc_columns.update(0)
    cells = all_cols.map{|v| { :txt => v, :value => v, :cell_mode => "" }}
    rc = RowChange.new("@@",cells)
    @patch.apply_row(rc)

    # If requested, we will be providing context rows around changed rows.
    # This is not a natural thing to do with SQL, so we do it only on request.
    # When requested, we need to buffer row changes.
    @pending_rcs = []

    # Prepare some useful SQL fragments to assemble later.
    sql_table1 = @db1.quote_table(@table1)
    sql_table2 = @db1.quote_table(@table2)
    sql_key_cols = key_cols.map{|c| @db1.quote_column(c)}.join(",")
    sql_all_cols = all_cols.map{|c| @db1.quote_column(c)}.join(",")
    sql_key_match = key_cols.map{|c| @db1.quote_column(c)}.map{|c| "#{sql_table1}.#{c} = #{sql_table2}.#{c}"}.join(" AND ")
    sql_data_mismatch = data_cols.map{|c| @db1.quote_column(c)}.map{|c| "#{sql_table1}.#{c} <> #{sql_table2}.#{c}"}.join(" OR ")

    # For one query we will need to interleave columns from two tables.  For
    # portability we need to give these columns distinct names.
    weave = all_cols.map{|c| [[sql_table1,@db1.quote_column(c)],
                              [sql_table2,@db2.quote_column(c)]]}.flatten(1)
    dbl_cols = weave.map{|c| "#{c[0]}.#{c[1]}"}
    sql_dbl_cols = weave.map{|c| "#{c[0]}.#{c[1]} AS #{c[0].gsub('.','_')}_#{c[1].gsub('.','_')}"}.join(",")

    # Prepare a map of primary key offsets.
    keys_in_all_cols = key_cols.each.map{|c| all_cols.index(c)}
    keys_in_dbl_cols = keys_in_all_cols.map{|x| 2*x}

    # Find rows in table2 that are not in table1.
    sql = "SELECT #{sql_all_cols} FROM #{sql_table2} WHERE NOT EXISTS (SELECT 1 FROM #{sql_table1} WHERE #{sql_key_match})"
    apply_inserts(sql,all_cols,keys_in_all_cols)

    # Find rows in table1 and table2 that differ while having the same primary
    # key.
    sql = "SELECT #{sql_dbl_cols} FROM #{sql_table1} INNER JOIN #{sql_table2} ON #{sql_key_match} WHERE #{sql_data_mismatch}"
    apply_updates(sql,dbl_cols,keys_in_dbl_cols)

    # Find rows that are in table1 but not table2
    sql = "SELECT #{sql_all_cols} FROM #{sql_table1} WHERE NOT EXISTS (SELECT 1 FROM #{sql_table2} WHERE #{sql_key_match})"
    apply_deletes(sql,all_cols,keys_in_all_cols)

    # If we are supposed to provide context, we need to deal with row order.
    if @patch.want_context
      sql = "SELECT #{sql_all_cols}, 0 AS __coopy_tag__ FROM #{sql_table1} UNION SELECT #{sql_all_cols}, 1 AS __coopy_tag__ FROM #{sql_table2} ORDER BY #{sql_key_cols}"
      apply_with_context(sql,all_cols,keys_in_all_cols)
    end

    # Done!
    @patch.end_diff
  end


  def apply_inserts(sql,all_cols,keys_in_all_cols)
    @db1.fetch(sql,all_cols) do |row|
      cells = row.map{|v| { :txt => v, :value => v, :cell_mode => "" }}
      rc = RowChange.new("+++",cells)
      apply_rc(rc,row,keys_in_all_cols)
    end
  end


  def apply_updates(sql,dbl_cols,keys_in_dbl_cols)
    @db1.fetch(sql,dbl_cols) do |row|
      pairs = row.enum_for(:each_slice,2).to_a
      cells = pairs.map do |v| 
        if v[0]==v[1] 
          { :txt => v[0], :value => v[0], :cell_mode => "" }
        else
          { :txt => v[0], :value => v[0], :new_value => v[1], :cell_mode => "->" }
        end
      end
      rc = RowChange.new("->",cells)
      apply_rc(rc,row,keys_in_dbl_cols)
    end
  end


  def apply_deletes(sql,all_cols,keys_in_all_cols)
    @db1.fetch(sql,all_cols) do |row|
      cells = row.map{|v| { :txt => v, :value => v, :cell_mode => "" }}
      rc = RowChange.new("---",cells)
      apply_rc(rc,row,keys_in_all_cols)
    end
  end

  def apply_rc(rc,row,keys_in_cols)
    rc.columns = @rc_columns
    if @patch.want_context
      rc.key = keyify(row.values_at(*keys_in_cols))
      @pending_rcs << rc
    else
      @patch.apply_row(rc)
    end
  end

  # Do the context dance.
  def apply_with_context(sql,all_cols,keys_in_all_cols)
    hits = {}
    @pending_rcs.each do |rc|
      hits[rc.key] = rc
    end 
    hist = []
    n = 2
    pending = 0
    skipped = false
    @db1.fetch(sql,all_cols + ["__coopy_tag__"]) do |row|
      tag = row.pop.to_i
      k = keyify(row.values_at(*keys_in_all_cols))
      if hits[k]
        hist.each do |row0|
          cells = row0.map{|v| { :txt => v, :value => v, :cell_mode => "" }}
          rc = RowChange.new("",cells)
          rc.columns = @rc_columns
          @patch.apply_row(rc)
        end
        hist.clear
        pending = n
        @patch.apply_row(hits[k])
        hits.delete(k)
        skipped = false
      elsif tag == 1
        # ignore redundant row
      elsif pending>0
        cells = row.map{|v| { :txt => v, :value => v, :cell_mode => "" }}
        rc = RowChange.new("",cells)
        rc.columns = @rc_columns
        @patch.apply_row(rc)
        pending = pending-1
        skipped = false
      else
        hist << row
        if hist.length>n
          if !skipped
            cells = row.map{|v| { :txt => "...", :value => "...", :cell_mode => "" }}
            rc = RowChange.new("...",cells)
            rc.columns = @rc_columns
            @patch.apply_row(rc)
            skipped = true
          end
          hist.shift
        end
      end
    end
  end
end

