require 'diff_output_table'
require 'csv'

class DiffRenderCsv < DiffOutputTable
  def initialize(fname)
    @csv = CSV.open(fname,'w')
  end

  def begin_row(row_mode)
    @line = []
  end

  def insert_cell(txt,mode,separator)
    @line << txt
  end

  def end_row
    @csv.add_row(@line)
  end

  def to_string
    ""
  end
end
