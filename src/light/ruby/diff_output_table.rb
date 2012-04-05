require 'diff_output'

class DiffOutputTable < DiffOutput
  def begin_table
  end

  def end_table
  end

  def begin_row(mode)
  end

  def insert_cell(txt,mode,separator)
  end

  def end_row
  end

  def apply_row(rc)
    self.begin_row(rc.row_mode)
    self.insert_cell(rc.row_mode,
                     (rc.row_mode=="->") ? rc.row_mode : "",
                     (rc.row_mode=="->") ? rc.row_mode : "")
    rc.cells.each do |cell|
      if cell[:new_value]
        self.insert_cell(cell[:value] + "->" + cell[:new_value],
                         cell[:cell_mode],
                         "->")
      else
        self.insert_cell(cell[:txt],cell[:cell_mode],cell[:separator])
      end
    end
    self.end_row
  end

  def begin_diff
    self.begin_table
  end

  def end_diff
    self.end_table
  end
end

