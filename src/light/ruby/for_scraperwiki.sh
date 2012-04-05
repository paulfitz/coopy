for f in diff_columns.rb row_change.rb sql_wrapper.rb sql_compare.rb diff_output.rb diff_output_table.rb diff_render_html.rb scraperwiki_sql_wrapper.rb; do
    echo ""
    echo ""
    echo "# $f"
    echo ""
    cat $f | grep -v require
done
