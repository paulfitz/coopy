import coopy
book1 = coopy.PolyBook()
book2 = coopy.PolyBook()

book1.read('named_numbers.csv')
book2.read('named_numbers_three_23.csv')
book1.write('named_numbers.sqlite')
out = coopy.CompareOutput('patch.diff')

patcher = coopy.Patcher.createByName('tdiff')
flags = coopy.CompareFlags()
flags.setOutput(out)
cmp = coopy.BookCompare()
cmp.compare(book1,book1,book2,patcher,flags)
