cat<<EOF
# SOURCE: https://github.com/paulfitz/coopy/tree/master/src/ruby
# EXAMPLE USE:
# see https://scraperwiki.com/views/paulfitzplaygroundview/
# show difference between paulfitzplayground.broken_bridge and paulfitzplayground.bridge
#   ScraperWiki.attach("paulfitzplayground")
#   sql = ScraperwikiSqlWrapper.new(ScraperWiki)
#   sql.set_primary_key(["bridge"])
#   cmp = SqlCompare.new(sql,"paulfitzplayground.broken_bridge","paulfitzplayground.bridge")
#   render = DiffRenderHtml.new
#   cmp.set_output(render)
#   cmp.apply
#   print render.to_string


EOF

for f in diff_columns.rb row_change.rb sql_wrapper.rb sql_compare.rb diff_output.rb diff_output_table.rb diff_render_html.rb diff_output_action.rb diff_output_group.rb diff_apply_sql.rb sqlite_sql_wrapper.rb scraperwiki_sql_wrapper.rb scraperwiki_utils.rb; do
    echo ""
    echo ""
    echo "# $f"
    echo ""
    cat $f | grep -v require
done
