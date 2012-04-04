require 'diff_columns'
require 'row_change'
require 'enumerator'

class SqlCompare
  def initialize(db1,db2)
    @db1 = db
    @db2 = db2
    @table1 = nil
    @table2 = nil
    @single_db = false
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
    if @single_db
      apply_single
    else
      apply_pair
    end
  end

  def apply_pair
    cols1 = @db1.primary_key(nil)
    cols2 = @db2.primary_key(nil)
    if cols1 != cols2
      raise "Primary keys do not match, please use full coopy toolbox"
    end
    cols = cols1
    raise "not implemented"
  end

  # within a single database, so we can delegate more work
  def apply_single
    cols1 = @db1.primary_key(@table1)
    cols2 = @db2.primary_key(@table2)
    if cols1 != cols2
      raise "Primary keys do not match, please use full coopy toolbox"
    end
    cols = cols1

    ecols1 = @db1.except_primary_key(@table1)
    ecols2 = @db2.except_primary_key(@table2)
    if ecols1 != ecols2
      raise "Non-primary key columns do not match, please use full coopy toolbox"
    end
    ecols = ecols1

    all_cols1 = @db1.column_names(@table1)
    all_cols2 = @db2.column_names(@table2)
    if all_cols1 != all_cols2
      raise "Columns do not match, please use full coopy toolbox"
    end
    all_cols = all_cols1

    @patch.begin_diff

    # find rows in table1 that are not in table2
    # join t1 and t2, find
    # select .., .. from t1 where not exists (select 1 from x=x y=y)
    
    # We build SQL manually to reuse against non-Sequel backends

    qkey = cols.map{|c| @db1.quote_column(c)}.join(", ")
    qall = all_cols.map{|c| @db1.quote_column(c)}.join(", ")
    qtable1 = @db1.quote_table(@table1)
    qtable2 = @db1.quote_table(@table2)
    qkeys = cols.map{|c| @db1.quote_column(c)}.map{|c| "#{qtable1}.#{c} = #{qtable2}.#{c}"}.join(" AND ")
    qekeys = ecols.map{|c| @db1.quote_column(c)}.map{|c| "#{qtable1}.#{c} <> #{qtable2}.#{c}"}.join(" OR ")

    rc_columns = DiffColumns.new
    rc_columns.title_row = all_cols
    rc_columns.update(0)

    # Things in R that are not in L
    sel = "SELECT #{qall} FROM #{qtable2} WHERE NOT EXISTS (SELECT 1 FROM #{qtable1} WHERE #{qkeys})"
    @db1.fetch(sel,all_cols) do |row|
      cells = row.map{|v| { :txt => v, :value => v, :cell_mode => "" }}
      rc = RowChange.new("+++",cells)
      rc.columns = rc_columns
      @patch.apply_row(rc)
    end

    # Things in L and R that are not equal
    nall2 = all_cols.map do |c|
      [
       "#{qtable1}_#{@db1.quote_column(c)}_1",
       "#{qtable1}_#{@db1.quote_column(c)}_2",       
      ]
    end
    nall2 = nall2.flatten
    qall2 = all_cols.map do |c| 
      [
       "#{qtable1}.#{@db1.quote_column(c)} AS #{qtable1}_#{@db1.quote_column(c)}_1",
       "#{qtable2}.#{@db1.quote_column(c)} AS #{qtable1}_#{@db1.quote_column(c)}_2",
      ]
    end
    qall2 = qall2.flatten.join(", ")

    sel = "SELECT #{qall2} FROM #{qtable1} INNER JOIN #{qtable2} ON #{qkeys} WHERE #{qekeys}"
    @db1.fetch(sel,nall2) do |row|
      pairs = row.enum_for(:each_slice,2).to_a
      cells = pairs.map do |v| 
        if v[0]==v[1] 
          { :txt => v[0], :value => v[0], :cell_mode => "" }
        else
          { :txt => v[0], :value => v[0], :new_value => v[1], :cell_mode => "->" }
        end
      end
      rc = RowChange.new("->",cells)
      rc.columns = rc_columns
      @patch.apply_row(rc)
    end

    rc_columns = DiffColumns.new
    rc_columns.title_row = cols
    rc_columns.update(0)

    # Things in L that are not in R
    sel = "SELECT #{qkey} FROM #{qtable1} WHERE NOT EXISTS (SELECT 1 FROM #{qtable2} WHERE #{qkeys})"
    @db1.fetch(sel,cols) do |row|
      cells = row.map{|v| { :txt => v, :value => v, :cell_mode => "" }}
      rc = RowChange.new("---",cells)
      rc.columns = rc_columns
      @patch.apply_row(rc)
    end

    @patch.end_diff
  end

end

