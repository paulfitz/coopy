require 'csv'
require 'diff_render_html.rb'
require 'diff_render_log.rb'

if ARGV.length < 1
  puts "Please supply a filename, pointing to a highlighter diff in CSV format"
  puts "  coopy_test.rb INPUT.csv log|html OUTPUT.txt"
  exit 1
end

fname = ARGV[0]
rows = CSV.read(fname)

fmt = "log"
fmt = ARGV[1] if ARGV.length>=2

if fmt == "log"
  diff = DiffRenderLog.new
else
  diff = DiffRenderHtml.new
end

diff.apply rows

if ARGV.length == 3
  f = File.open(ARGV[2],"w")
  f.write(diff.to_string)
  f.close()
else
  print diff.to_string
end
