require 'coopy'

def convertFile(src,dest)
  book = Coopy::PolyBook.new
  book.read(src)
  book.write(dest)
end

def generateDiff(version1,version2,dest)
  book1 = Coopy::PolyBook.new
  book1.read(version1)
  book2 = Coopy::PolyBook.new
  book2.read(version2)
  out = Coopy::CompareOutput.new(dest)
  patcher = Coopy::Patcher.createByName('tdiff')
  flags = Coopy::CompareFlags.new
  flags.setOutput(out)
  cmp = Coopy::BookCompare.new
  cmp.compare(book1,book1,book2,patcher,flags)
end

class TableDiff < Coopy::Patcher
  
  def changeRow(change)
    puts "Row change of type '%s'"%change.modeString()
    for c in change.cond.keys()
      puts "  condition: %s = %s"%[c,change.cond[c].toString()]
    end
    for c in change.val.keys()
      puts "  value: %s -> %s"%[c,change.val[c].toString()]
    end
    return true
  end
  
end

def examineDiff(version1,version2)
  book1 = Coopy::PolyBook.new
  book1.read(version1)
  book2 = Coopy::PolyBook.new
  book2.read(version2)
  cmp = Coopy::BookCompare.new
  cmp.compare(book1,book1,book2,TableDiff.new,Coopy::CompareFlags.new)
end

generateDiff('named_numbers.csv','named_numbers_three_23.csv','patch.diff')
convertFile('named_numbers.csv','named_numbers.sqlite')
examineDiff('named_numbers.csv','named_numbers_three_23.csv')

book1 = Coopy::PolyBook.new()
book2 = Coopy::PolyBook.new()

book1.read('named_numbers.csv')
book2.read('named_numbers_three_23.csv')
book1.write('named_numbers.sqlite')
out = Coopy::CompareOutput.new('patch.diff')

patcher = Coopy::Patcher.createByName('tdiff')
flags = Coopy::CompareFlags.new()
flags.setOutput(out)
cmp = Coopy::BookCompare.new()
cmp.compare(book1,book1,book2,patcher,flags)
