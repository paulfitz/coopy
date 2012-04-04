class RowChange
  attr_accessor :row_mode
  attr_accessor :cells
  attr_accessor :columns

  def initialize(row_mode,cells)
    @row_mode = row_mode
    @cells = cells
  end

  def active_columns
    return [] if @columns.nil?
    @columns.column_by_offset
  end

  def value_at(column)
    @cells[column[:diff_offset]][:value]
  end

  def new_value_at(column)
    @cells[column[:diff_offset]][:new_value]
  end
end
