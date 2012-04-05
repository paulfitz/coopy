require 'diff_columns'
require 'row_change'

class DiffOutput
  def begin_diff
  end

  def end_diff
  end

  def apply_row(rc)
  end

  def to_string
    ""
  end

  def want_context
    true
  end
end
