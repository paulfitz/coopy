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

    # find rows in table1 that are not in table2
    # join t1 and t2, find
    # select .., .. from t1 where not exists (select 1 from x=x y=y)
    
    # We build SQL manually to reuse against non-Sequel backends

    qkey = cols.map{|c| @db1.quote_column(c)}.join(", ")
    qtable1 = @db1.quote_table(@table1)
    qtable2 = @db1.quote_table(@table2)
    qkeys = cols.map{|c| @db1.quote_column(c)}.map{|c| "#{qtable1}.#{c} = #{qtable2}.#{c}"}.join(" AND ")
    qekeys = ecols.map{|c| @db1.quote_column(c)}.map{|c| "#{qtable1}.#{c} <> #{qtable2}.#{c}"}.join(" OR ")

    # Things in L that are not in R
    sel = "SELECT #{qkey} FROM #{qtable1} WHERE NOT EXISTS (SELECT 1 FROM #{qtable2} WHERE #{qkeys})"
    puts sel
    @db1.fetch(sel,cols) do |row|
      puts row.inspect
    end

    # Things in R that are not in L
    sel = "SELECT #{qkey} FROM #{qtable2} WHERE NOT EXISTS (SELECT 1 FROM #{qtable1} WHERE #{qkeys})"
    puts sel
    @db1.fetch(sel,cols) do |row|
      puts row.inspect
    end

    # Things in L and R that are not equal
    sel = "SELECT #{qtable1}.#{qkey} FROM #{qtable1} INNER JOIN #{qtable2} ON #{qkeys} WHERE #{qekeys}"
    puts sel
    @db1.fetch(sel,cols) do |row|
      puts row.inspect
    end

  end

end

