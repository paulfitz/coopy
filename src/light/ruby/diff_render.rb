
class DiffRender
  def begin_table
  end

  def begin_row(mode)
  end

  def insert_cell(txt,mode,separator)
  end

  def end_row
  end

  def end_table
  end

  def apply(rows)
    render = self
    render.begin_table
    change_row = nil
    rows.each do |r|
      row_mode = ""
      open = false
      next if r.length == 0
      txt = r[0]
      if txt=="@" or txt=="@@"
        row_mode = "@@"
      elsif txt=="!" or txt=="+++" or txt=="---"
        row_mode = txt
      elsif txt=="!"
        change_row = r
      else
        open = true
      end
      cmd = txt
      render.begin_row(row_mode)
      r.each_with_index do |txt, c|
        txt = "" if txt=="NULL"
        cell_mode = ""
        separator = ""
        if open and !change_row.nil?
          change = change_row[c]
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
        render.insert_cell(txt,cell_mode,separator)
      end
      render.end_row
    end
    render.end_table
  end

  def to_string
    return ""
  end
end

