def link_tables(watch_scraper,watch_tables)
  current_tables = ScraperWiki.show_tables()
  remote_tables = nil
  watch_tables.each do |tbl|
    unless current_tables.has_key? tbl
      remote_tables = ScraperWiki.show_tables("#{watch_scraper}") if remote_tables.nil? 
      sql = remote_tables[tbl]
      #puts "Making #{tbl}= #{sql}"
      ScraperWiki.sqliteexecute(sql)
    end
  end
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
