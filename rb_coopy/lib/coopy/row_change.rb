class RowChange
  attr_accessor :row_mode
  attr_accessor :cells
  attr_accessor :columns
  attr_accessor :key

  def initialize(row_mode,cells)
    @row_mode = row_mode
    @cells = cells
    @key = nil
  end

  def active_columns
    return [] if @columns.nil?
    @columns.column_by_offset
  end

  def value_at(column)
    @cells[column[:in_offset]][:value]
  end

  def new_value_at(column)
    @cells[column[:in_offset]][:new_value]
  end
end
