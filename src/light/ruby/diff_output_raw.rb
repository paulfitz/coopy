require 'diff_output'

class DiffOutputRaw < DiffOutput
  def apply_row(rc)
    puts "Diff: #{rc.inspect}"
  end
end
