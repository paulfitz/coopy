import sys
import csv
import DiffRender

ARGV = sys.argv[1:]

if len(ARGV)==0:
  print("Please supply a filename, pointing to a highlighter diff in CSV format\n")
  print("  coopy_test.py INPUT.csv html|log OUTPUT.txt\n")
  exit(1)

fname = ARGV[0]
rows = csv.reader(open(fname,"r").readlines())

is_html = True
if len(ARGV)>2:
  if ARGV[1] == "log":
    is_html = False

diff = DiffRender.DiffRenderHtml() if is_html else DiffRender.DiffRenderLog()

diff.apply(rows)

if len(ARGV) >= 3:
  f = open(ARGV[2],"w")
  f.write(diff.to_string())
  f.close()
else:
  print diff.to_string()
