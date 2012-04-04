require 'sql_wrapper'
require 'dbi'

class DbiSqlWrapper < SqlWrapper
  def initialize(db)
    @db = db
    @t = nil
    @qt = nil
  end

  def clone
    DbiSqlWrapper.new(@db)
  end

  def complete_table(tbl)
    return tbl unless tbl.nil?
    return @t unless @t.nil?
    @t = @db.tables[0]
    @t
  end

  def quote_table(tbl)
    return @db.quote(tbl) unless tbl.nil?
    return @qt unless @qt.nil?
    @t = @db.tables[0]
    @qt = @db.quote(@t)
    @qt
  end

  def insert(tbl,cols,vals)
    tbl = quote_table(tbl)
    template = cols.map{|x| '?'}.join(",")
    template = "INSERT INTO #{tbl} VALUES(#{template})"
    stmt = @db.prepare(template)
    stmt.execute(*vals)
    stmt.finish
  end

  def delete(tbl,cols,vals)
    tbl = quote_table(tbl)
    template = cols.map{|c| @db.quote(c) + ' = ?'}.join(" AND ")
    template = "DELETE FROM #{tbl} WHERE #{template}"
    stmt = @db.prepare(template)
    stmt.execute(*vals)
    stmt.finish
  end
  
  def update(tbl,set_cols,set_vals,cond_cols,cond_vals)
    tbl = quote_table(tbl)
    conds = cond_cols.map{|c| @db.quote(c) + ' = ?'}.join(" AND ")
    sets = set_cols.map{|c| @db.quote(c) + ' = ?'}.join(", ")
    template = "UPDATE #{@qt} SET #{sets} WHERE #{conds}"
    v = set_vals + cond_vals
    stmt = @db.prepare(template)
    stmt.execute(*v)
    stmt.finish
  end

  def transaction(&block)
    @db["AutoCommit"]=false
    begin
      block.call
      @db.commit
    rescue Exception => e
      @db.rollback
      raise e
    end
  end

  def columns(tbl)
    tbl = complete_table(tbl)
    @db.columns(tbl)
  end

  def column_names(tbl)
    columns(tbl).map{|c| c[:name]}
  end

  def enhash(cols,vals)
    Hash[*cols.map{|c| c.to_sym}.zip(vals).flatten]
  end

  def fetch(sql,names)
    @db.select_all(sql) do |row|
      yield enhash(names,row)
    end
  end

  def primary_key(tbl)
    # don't seem to have this information? oy.
    [column_names(tbl)[0]]
  end
end
