require 'coopy/diff_output'

class DiffOutputAction < DiffOutput
  def row_insert(rc)
  end

  def row_delete(rc)
  end

  def row_update(rc)
  end

  def row_skip(rc)
  end

  def row_context(rc)
  end

  def apply_row(rc)
    mode = rc.row_mode
    case mode
    when "+++"
      row_insert(rc)
    when "---"
      row_delete(rc)
    when "->"
      row_update(rc)
    when "..."
      row_skip(rc)
    when ""
      row_context(rc)
    end
  end
end
