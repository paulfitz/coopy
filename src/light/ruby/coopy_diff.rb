#!/usr/bin/env ruby

require 'dbi_sql_wrapper'
require 'sql_compare'
require 'diff_output_raw'
require 'diff_output_tdiff'
require 'diff_render_html'
require 'diff_render_csv'

use_sequel = false # switch from dbi to sequel

require 'dbi_sql_wrapper' unless use_sequel
require 'sequel_sql_wrapper' if use_sequel

if ARGV.length < 3
  puts "call as:"
  puts "  coopy_diff.rb ver1.sqlite table1 table2"
  exit 1
end

if use_sequel
  sql = SequelSqlWrapper.new('sqlite://' + ARGV[0])
else
  db = DBI.connect('DBI:SQLite3:' + ARGV[0], 'ruby', 'ruby')
  sql = DbiSqlWrapper.new(db)
end

cmp = SqlCompare.new(sql,ARGV[1],ARGV[2])

patch = DiffRenderHtml.new
# patch = DiffOutputTdiff.new
patch = DiffRenderCsv.new("output.csv")

cmp.set_output(patch)

cmp.apply
result = patch.to_string
puts result unless result == ""
