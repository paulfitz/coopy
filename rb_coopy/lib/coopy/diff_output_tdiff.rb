require 'coopy/diff_output_action'

# this is just a stub, not yet functional
class DiffOutputTdiff < DiffOutputAction
  def begin_diff
    puts "# tdiff version 0.3"
  end

  def row_show(rc)
    rc.active_columns.each do |col|
      title = col[:title]
      offset = col[:diff_offset]
      print "|"
      print title
      print "="
      print rc.cells[offset][:txt]
      unless rc.cells[offset][:new_value].nil?
        print "->"
        print rc.cells[offset][:new_value]
      end
    end
    puts "|"
  end

  def row_insert(rc)
    print "+ "
    row_show(rc)
  end

  def row_delete(rc)
    print "- "
    row_show(rc)
  end

  def row_update(rc)
    print "= "
    row_show(rc)
  end

  def row_skip(rc)
    puts "* ||"
  end

  def row_context(rc)
    print "* "
    row_show(rc)
  end
end
