#!/usr/bin/env ruby

require 'csv'
require 'diff_parser'
require 'diff_apply_sql'

use_sequel = false # switch from dbi to sequel

require 'dbi_sql_wrapper' unless use_sequel
require 'sequel_sql_wrapper' if use_sequel

if ARGV.length < 2
  puts "call as:"
  puts "  coopy_patch.rb database_to_patch.sqlite patch.csv"
  exit 1
end

fname = ARGV[1]
rows = CSV.read(fname)

if use_sequel
  sql = SequelSqlWrapper.new('sqlite://' + ARGV[0])
else
  db = DBI.connect('DBI:SQLite3:' + ARGV[0], 'ruby', 'ruby')
  sql = DbiSqlWrapper.new(db)
end

diff = DiffApplySql.new(sql)

parser = DiffParser.new diff

begin
  sql.transaction do
    parser.apply rows
  end
rescue Exception => e
  puts "Problem: #{e.inspect}"
  puts "Rolling back any changes made."
  raise e
  exit(1)
end

