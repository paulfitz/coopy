require 'coopy/diff_output_table'

class DiffRenderCsv < DiffOutputTable
  def initialize(fname = nil)
    require "fastercsv" unless RUBY_VERSION > "1.9"
    @txt = ""
    @csv = FCSV.open(fname,'w') if fname
  end

  def begin_row(row_mode)
    @line = []
  end

  def insert_cell(txt,mode,separator)
    @line << txt
  end

  def end_row
    if @csv
      @csv.add_row(@line)
    else
      @txt << @line.to_csv
    end
  end

  def to_string
    @txt
  end
end
