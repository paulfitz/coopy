def link_tables(watch_scraper,watch_tables)
  sql = ScraperwikiSqlWrapper.new(ScraperWiki)
  watch_tables.each { |tbl| sql.copy_table_structure(watch_scraper,tbl) }
end

class CoopyResult
  attr_accessor :html
end

def sync_table(watch_scraper,tbl,keys)
  sql = ScraperwikiSqlWrapper.new(ScraperWiki)
  sql.set_primary_key(keys) unless keys.nil? 
  cmp = SqlCompare.new(sql,"main.#{tbl}","#{watch_scraper}.#{tbl}")
  sinks = DiffOutputGroup.new
  render = DiffRenderHtml.new
  sinks << render
  sinks << DiffApplySql.new(sql,"main.#{tbl}")
  cmp.set_output(sinks)
  cmp.apply
  result = CoopyResult.new
  result.html = render.to_string
  result
end
