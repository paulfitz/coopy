require 'diff_output'

# this is just a stub, not yet functional
class DiffOutputTdiff < DiffOutput
  def begin_diff
    puts "# tdiff version 0.3"
  end

  def row_insert(rc)
    puts "insert"
  end

  def row_delete(rc)
    puts "delete"
  end

  def row_update(rc)
    puts "update"
  end

  def row_skip(rc)
    puts "* ||"
  end

  def row_context(rc)
    print "* "
    rc.active_columns.each do |col|
      title = col[:title]
      offset = col[:diff_offset]
      print "|"
      print title
      print "="
      print rc.cells[offset][:txt]
    end
    puts "|"
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
    else
      puts "[#{mode}]"
    end
  end
end
