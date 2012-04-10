#!/usr/bin/env ruby

require 'diff_output_raw'
require 'diff_output_tdiff'
require 'diff_render_html'
require 'diff_render_csv'
require 'diff_output_action'
require 'diff_output_group'
require 'diff_apply_sql'

require 'sqlite_sql_wrapper'
require 'sql_compare'
require 'sqlite3'

if ARGV.length < 2
  puts "call as:"
  puts "  coopy_diff.rb ver1.sqlite ver2.sqlite"
  puts "  coopy_diff.rb data.sqlite table1 table2"
  puts "  coopy_diff.rb ver1.sqlite table1 ver2.sqlite table2"
  exit 1
end

db = SQLite3::Database.new(ARGV[0])
sql = SqliteSqlWrapper.new(db)

name1 = nil
name2 = nil
case ARGV.length
when 2
  name0 = sql.get_table_names[0]
  db.execute("ATTACH ? AS __peer__",ARGV[1])
  name1 = "main.#{name0}"
  name2 = "__peer__.#{name0}"
when 3
  name1 = ARGV[1]
  name2 = ARGV[2]
when 4
  name1 = "main.#{ARGV[1]}"
  db.execute("ATTACH ? AS __peer__",ARGV[2])
  name2 = "__peer__.#{ARGV[3]}"
end


cmp = SqlCompare.new(sql,name1,name2)

patches = DiffOutputGroup.new
patches << DiffRenderHtml.new
# patch = DiffOutputTdiff.new
patches << DiffRenderCsv.new("output.csv")
# patches << DiffApplySql.new(sql,name1)

cmp.set_output(patches)

cmp.apply
result = patches.to_string
puts result unless result == ""
