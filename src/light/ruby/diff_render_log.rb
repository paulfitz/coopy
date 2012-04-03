require 'diff_output_table'

class DiffRenderLog < DiffOutputTable
  def initialize
    @r = 0
    @c = 0
    @row_mode = ""
    @row_log = []
    @cell_log = []
  end

  def begin_table
  end

  def begin_row(mode)
    @row_mode = mode
    @row_log << [@r, mode]
    @c = 0
  end

  def insert_cell(txt,mode,separator)
    @cell_log << {
      :col => @c,
      :row => @r,
      :txt => txt,
      :cell_mode => mode,
      :row_mode => @row_mode,
      :separator => separator
    }
    @c = @c + 1
  end

  def end_row
    @r = @r + 1
  end

  def end_table
  end
  
  def log
    txt = ""
    @cell_log.each do |v|
      txt << v[:col].to_s + "," + v[:row].to_s + " [" + v[:cell_mode] + "/" +
        v[:row_mode] + "] " + v[:separator] + " : " + v[:txt].to_s + "\n"
    end
    txt
  end

  def to_string
    log
  end
end
