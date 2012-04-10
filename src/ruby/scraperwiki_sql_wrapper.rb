require 'sqlite_sql_wrapper'

# Tweak sqlite wrapper slightly to match ScraperWiki's API
class ScraperwikiSqlWrapper < SqliteSqlWrapper
  def set_primary_key(lst)
    @pk = lst
  end

  def sqlite_execute(template,vals)
    @db.sqliteexecute(template,vals)
  end

  def part(row,n,name)
    row[n]
  end

  def fetch(sql,names)
    res = sqlite_execute(sql,[])
    res["data"].each do |row|
      yield row
    end
  end

  def pragma(tbl,info)
    result = super(tbl,info)
    result["data"]
  end
end
