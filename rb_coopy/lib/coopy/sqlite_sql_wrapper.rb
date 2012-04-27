require 'coopy/sql_wrapper'

class Coopy::SqliteSqlWrapper < SqlWrapper
  def initialize(db)
    @db = db
    @t = nil
    @qt = nil
    @pk = nil
    @info = {}
  end

  def set_primary_key(lst)
    @pk = lst
  end

  def sqlite_execute(template,vals)
    @db.execute(template,*vals)
  end

  def get_table_names
    sqlite_execute("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name",[]).flatten
  end

  def complete_table(tbl)
    @t = tbl unless tbl.nil?
    @t = get_table_names[0] if @t.nil?
    @t
  end

  def quote_with_dots(x)
    return x if x.match(/^[a-zA-Z0-9_]+$/)
    x.split('.').map{|p| "`#{p}`"}.join('.')
  end

  def quote_table(tbl)
    complete_table(tbl)
    return @t if @t.match(/^[a-zA-Z0-9_]+$/)
    quote_with_dots(@t)
  end

  def quote_column(col)
    return col if col.match(/^[a-zA-Z0-9_]+$/)
    quote_with_dots(col)
  end

  def insert(tbl,cols,vals)
    tbl = quote_table(tbl)
    template = cols.map{|x| '?'}.join(",")
    template = "INSERT INTO #{tbl} VALUES(#{template})"
    sqlite_execute(template,vals)
  end

  def delete(tbl,cols,vals)
    tbl = quote_table(tbl)
    template = cols.map{|c| quote_column(c) + ' IS ?'}.join(" AND ")
    template = "DELETE FROM #{tbl} WHERE #{template}"
    sqlite_execute(template,vals)
  end
  
  def update(tbl,set_cols,set_vals,cond_cols,cond_vals)
    tbl = quote_table(tbl)
    conds = cond_cols.map{|c| quote_column(c) + ' IS ?'}.join(" AND ")
    sets = set_cols.map{|c| quote_column(c) + ' = ?'}.join(", ")
    template = "UPDATE #{tbl} SET #{sets} WHERE #{conds}"
    v = set_vals + cond_vals
    sqlite_execute(template,v)
  end

  def transaction(&block)
    # not yet mapped, not yet used
    block.call
  end

  def pragma(tbl,info)
    if tbl.include? '.'
      dbname, tbname, *ignore = tbl.split('.')
      dbname = quote_with_dots(dbname)
      tbname = quote_with_dots(tbname)
      query = "PRAGMA #{dbname}.#{info}(#{tbname})"
    else
      tbl = quote_with_dots(tbl)
      query = "PRAGMA #{info}(#{tbl})"
    end
    result = sqlite_execute(query,[])
    result
  end

  def part(row,n,name)
    row[n]
  end

  def columns(tbl)
    tbl = complete_table(tbl)
    @info[tbl] = pragma(tbl,"table_info") unless @info.has_key? tbl
    @info[tbl]
  end

  def column_names(tbl)
    columns(tbl).map{|c| part(c,1,"name")}
  end

  def fetch(sql,names)
    sqlite_execute(sql,[]).each do |row|
      yield row
    end
  end

  def primary_key(tbl)
    return @pk unless @pk.nil?
    cols = columns(tbl)
    cols = cols.select{|c| part(c,5,"pk").to_s=="1"}.map{|c| part(c,1,"name")}
    if cols.length == 0
      cols = pk_from_unique_index(tbl)
    end
    @pk = cols if cols.length>0
    cols
  end

  def pk_from_unique_index(tbl)
    pragma(tbl,"index_list").each do |row|
      if part(row,2,"unique").to_s == "1"
        idx = part(row,1,"name")
        return pragma(idx,"index_info").map{|r| part(r,2,"name")}
      end
    end
    nil
  end

  # copy the structure of an attached table, along with any indexes
  def copy_table_structure(rdb,tbl)
    template = "SELECT sql, type from X.sqlite_master WHERE tbl_name = ? ORDER BY type DESC"
    lsql = template.gsub('X',"main")
    rsql = template.gsub('X',quote_with_dots(rdb))
    args = [quote_with_dots(tbl)]
    lschema = sqlite_execute(lsql,args)
    rschema = sqlite_execute(rsql,args)
    if lschema.length>0
      return false
    end
    rschema.each{ |row| sqlite_execute(row[0],[]) }
    true
  end
end
