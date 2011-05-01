require 'coopy'

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
