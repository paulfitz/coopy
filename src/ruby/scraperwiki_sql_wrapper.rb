require 'sqlite_sql_wrapper'

# Tweak sqlite wrapper slightly to match ScraperWiki's API
class ScraperwikiSqlWrapper < SqliteSqlWrapper
  def sqlite_execute(template,vals)
    @db.sqliteexecute(template,vals)["data"]
  end
end
