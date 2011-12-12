require 'coopy'

class TableDiff < Coopy::Patcher
  
  def changeRow(change)
    puts "Row change of type '%s'"%change.modeString()
    for c in change.cond.keys()
      puts "* condition: %s = %s"%[c,change.cond[c].toString()]
    end
    for c in change.val.keys()
      puts "* value: %s -> %s"%[c,change.val[c].toString()]
    end
    puts ""
    true
  end
  
end

flags = Coopy::CompareFlags.new()
differ = TableDiff.new()
parser = Coopy::PatchParser.new(differ,"diff.xls",flags)
parser.applyPatch()
