require 'sql_wrapper'
require 'sqlite3'

class SqliteSqlWrapper < SqlWrapper
  def initialize(db)
    @db = db
    @t = nil
    @qt = nil
    @info = {}
  end

  def clone
    SqliteSqlWrapper.new(@db)
  end

  def sqlite_execute(template,vals)
    return @db.execute(template,*vals)
  end

  def complete_table(tbl)
    @t = tbl unless tbl.nil?
    @t
  end

  def quote_table(tbl)
    complete_table(tbl)
    @t
  end

  def insert(tbl,cols,vals)
    tbl = quote_table(tbl)
    template = cols.map{|x| '?'}.join(",")
    template = "INSERT INTO #{tbl} VALUES(#{template})"
    sqlite_execute(template,vals)
  end

  def delete(tbl,cols,vals)
    tbl = quote_table(tbl)
    template = cols.map{|c| @db.quote(c) + ' = ?'}.join(" AND ")
    template = "DELETE FROM #{tbl} WHERE #{template}"
    sqlite_execute(template,vals)
  end
  
  def update(tbl,set_cols,set_vals,cond_cols,cond_vals)
    tbl = quote_table(tbl)
    conds = cond_cols.map{|c| @db.quote(c) + ' = ?'}.join(" AND ")
    sets = set_cols.map{|c| @db.quote(c) + ' = ?'}.join(", ")
    template = "UPDATE #{@qt} SET #{sets} WHERE #{conds}"
    v = set_vals + cond_vals
    sqlite_execute(template,v)
  end

  def transaction(&block)
    # not yet mapped, not yet used
    block.call
  end

  def columns(tbl)
    tbl = quote_table(tbl)
    cols = @info[tbl]
    if cols.nil?
      query = "PRAGMA table_info(#{tbl})"
      cols = @info[tbl] = sqlite_execute(query,[])
    end
    cols
  end

  def column_names(tbl)
    columns(tbl).map{|c| c[1]}
  end

  def fetch(sql,names)
    sqlite_execute(sql,[]).each do |row|
      yield row
    end
  end

  def primary_key(tbl)
    cols = columns(tbl)
    cols.select{|c| c[5]==1}.map{|c| c[1]}
  end
end
