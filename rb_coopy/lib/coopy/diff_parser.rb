require 'coopy/diff_output'
require 'csv'

class DiffParser
  def initialize(fname)
    @rows = CSV.read(fname)
  end

  def set_output(output)
    @diff_output = output
  end

  def apply_row_main(rc,columns)
    rc.columns = columns
    @diff_output.apply_row(rc)
  end

  def apply
    rows = @rows
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
      row.each_with_index do |val, c|
        next if c == 0
        nval = nil
        txt = ""
        txt = val.to_s unless val.nil?
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
          if txt.include? cmd
            cell_mode = "->"
            separator = cmd
            b = txt.index(cmd)
            val = txt[0,b]
            nval = txt[b+cmd.length,txt.length]
          end
        end
        cells << {
          :txt => txt,
          :value => val,
          :new_value => nval,
          :cell_mode => cell_mode,
          :separator => separator
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

