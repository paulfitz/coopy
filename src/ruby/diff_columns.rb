class DiffColumns
  attr_accessor :change_row
  attr_accessor :title_row

  # general
  attr_accessor :column_name   # *after* any column changes
  attr_accessor :column_offset # *after* any column changes
  attr_accessor :column_by_name
  attr_accessor :column_by_offset

  def update(prefix=1)
    return if @title_row.nil?
    @column_name = {}
    @column_offset = {}
    @column_by_name = {}
    @column_by_offset = []
    offset = -prefix
    @title_row.each_with_index do |title,idx|
      @column_name[idx] = title
      if offset>=0
        # assuming no column changes for the moment
        @column_offset[idx] = offset
        @column_by_name[title] = {
          :title => title,
          :in_offset => offset,
          :diff_offset => idx
        }
        @column_by_offset << @column_by_name[title]
      end
      offset = offset+1
    end
  end
end
