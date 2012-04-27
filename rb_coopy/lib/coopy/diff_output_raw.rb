require 'coopy/diff_output'

class DiffOutputRaw < DiffOutput
  def apply_row(rc)
    unless @shown_columns
      unless rc.columns
        puts "COLUMN NAMES:      #{rc.columns.column_name}"
        puts "COLUMN OFFSETS:    #{rc.columns.column_offset}"
        puts "COLUMN CHANGE ROW: #{rc.columns.change_row}"
        puts "COLUMN TITLE ROW:  #{rc.columns.title_row}"
        puts " "
        @shown_columns = true
      end
    end
    puts "Mode: [#{rc.row_mode}]"
    puts "Cells: #{rc.cells}"
    puts " "
  end
end
