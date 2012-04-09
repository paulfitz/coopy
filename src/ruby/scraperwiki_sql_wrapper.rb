require 'sql_wrapper'

class ScraperwikiSqlWrapper < SqlWrapper
  def initialize(db)
    @db = db
    @t = nil
    @qt = nil
    @pk = []
    @info = {}
  end

  def set_primary_key(lst)
    @pk = lst
  end

  def sqlite_execute(template,vals)
    @db.sqliteexecute(template,vals)
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
      cols = @info[tbl] = @db.table_info(tbl)
    end
    cols
  end

  def column_names(tbl)
    columns(tbl).map{|c| c["name"]}
  end

  def fetch(sql,names)
    res = sqlite_execute(sql,[])
    res["data"].each do |row|
      yield row
    end
  end

  def primary_key(tbl)
    return @pk unless @pk.nil?
    cols = columns(tbl)
    cols.select{|c| c["pk"].to_s=="1"}.map{|c| c["name"]}
  end
end
