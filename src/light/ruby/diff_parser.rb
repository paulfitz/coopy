require 'diff_output'

class DiffParser
  def initialize(output)
    @diff_output = output
  end

  def apply_row_main(rc,columns)
    rc.columns = columns
    @diff_output.apply_row(rc)
  end

  def apply(rows)
    render = @diff_output
    return if render.nil?
    render.begin_diff
    rcs = []
    columns = DiffColumns.new
    rows.each_with_index do |row, r|
      row_mode = ""
      open = false
      next if row.length == 0
      txt = row[0]
      txt = "" if txt.nil?
      if txt=="@" or txt=="@@"
        row_mode = "@@"
        columns.title_row = row
        columns.update
      elsif txt=="!" or txt=="+++" or txt=="---" or txt=="..." or txt.include? "->"
        row_mode = txt
        columns.change_row = row if txt=="!"
      else
        open = true
      end
      cmd = txt
      cells = []
      row.each_with_index do |txt, c|
        txt = "" if txt=="NULL"
        cell_mode = ""
        separator = ""
        if open and !columns.change_row.nil?
          change = columns.change_row[c]
          if change=="+++" or change == "---"
            cell_mode = change
          end
        end
        if cmd.to_s.include? "->"
          if txt.to_s.include? cmd
            cell_mode = "->"
            separator = cmd
          end
        end
        cells << {
          :txt => txt,
          :cell_mode => cell_mode,
          :separator => separator,
          :r => r,
          :c => c
        }
      end
      rc = RowChange.new(row_mode,cells)
      unless columns.title_row.nil?
        rcs.each { |rc| self.apply_row_main(rc,columns) }
        rcs = []
        self.apply_row_main(rc,columns)
      else
        rcs << rc
      end
    end
    rcs.each { |rc| self.apply_row_main(rc,columns) }
    render.end_diff
  end

  def to_string
    return diff_output.to_string unless diff_output.nil?
    ""
  end
end

