#!/usr/bin/env ruby

require 'csv'
require 'diff_parser.rb'
require 'diff_apply_sql.rb'
require 'dbi_sql_wrapper.rb'

if ARGV.length < 2
  puts "call as:"
  puts "  coopy_patch.rb database_to_patch.sqlite patch.csv"
  exit 1
end

fname = ARGV[1]
rows = CSV.read(fname)

sql = DbiSqlWrapper.new(ARGV[0])
diff = DiffApplySql.new(sql)

parser = DiffParser.new diff

begin
  sql.start_transaction
  parser.apply rows
  sql.commit
rescue Exception => e
  puts "Problem: #{e.inspect}"
  puts "Rolling back any changes made."
  sql.rollback
  exit(1)
end

