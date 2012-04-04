require 'sql_wrapper'
require 'dbi'

class DbiSqlWrapper
  # just sqlite for now
  def initialize(fname)
    @db = DBI.connect('DBI:SQLite3:' + fname, 'ruby', 'ruby')
    @qt = nil
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

  def start_transaction
    @db["AutoCommit"]=false
  end

  def commit
    @db.commit
  end

  def rollback
    @db.rollback
  end
end
