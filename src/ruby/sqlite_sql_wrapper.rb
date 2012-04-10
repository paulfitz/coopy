require 'sql_wrapper'

class SqliteSqlWrapper < SqlWrapper
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
    return @db.execute(template,*vals)
  end

  def get_table_names
    return sqlite_execute("SELECT name FROM sqlite_master WHERE type='table'",[]).flatten
  end

  def complete_table(tbl)
    @t = tbl unless tbl.nil?
    @t
  end

  def quote_table(tbl)
    complete_table(tbl)
    @t
  end

  def quote_column(col)
    return col if col.match(/^[a-zA-Z0-9_]+$/)
    "`#{col}`"
  end

  def insert(tbl,cols,vals)
    tbl = quote_table(tbl)
    template = cols.map{|x| '?'}.join(",")
    template = "INSERT INTO #{tbl} VALUES(#{template})"
    sqlite_execute(template,vals)
  end

  def delete(tbl,cols,vals)
    tbl = quote_table(tbl)
    template = cols.map{|c| quote_column(c) + ' = ?'}.join(" AND ")
    template = "DELETE FROM #{tbl} WHERE #{template}"
    sqlite_execute(template,vals)
  end
  
  def update(tbl,set_cols,set_vals,cond_cols,cond_vals)
    tbl = quote_table(tbl)
    conds = cond_cols.map{|c| quote_column(c) + ' = ?'}.join(" AND ")
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
      dbname = tbl.gsub(/\..*/,'.')
      tbname = tbl.gsub(/.*\./,'')
      query = "PRAGMA #{dbname}#{info}(#{tbname})"
    else
      query = "PRAGMA #{info}(#{tbl})"
    end
    result = sqlite_execute(query,[])
    result
  end

  def part(row,n,name)
    row[n]
  end

  def columns(tbl)
    tbl = quote_table(tbl)
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
end
