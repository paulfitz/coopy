import coopy

def convertFile(src,dest):
    book = coopy.PolyBook()
    book.read(src)
    book.write(dest)

def generateDiff(version1,version2,dest):
    book1 = coopy.PolyBook()
    book1.read(version1)
    book2 = coopy.PolyBook()
    book2.read(version2)
    out = coopy.CompareOutput(dest)
    patcher = coopy.Patcher.createByName('tdiff')
    flags = coopy.CompareFlags()
    flags.setOutput(out)
    cmp = coopy.BookCompare()
    cmp.compare(book1,book1,book2,patcher,flags)

class TableDiff(coopy.Patcher):

    def changeRow(self,change):
        print("Row change of type '%s'"%change.modeString())
        for c in change.cond.keys():
            print("  condition: %s = %s"%(c,change.cond[c].toString()))
        for c in change.val.keys():
            print("  value: %s -> %s"%(c,change.val[c].toString()))
        return True

def examineDiff(version1,version2):
    book1 = coopy.PolyBook()
    book1.read(version1)
    book2 = coopy.PolyBook()
    book2.read(version2)
    cmp = coopy.BookCompare()
    cmp.compare(book1,book1,book2,TableDiff(),coopy.CompareFlags())

generateDiff('named_numbers.csv','named_numbers_three_23.csv','patch.diff')
convertFile('named_numbers.csv','named_numbers.sqlite')
examineDiff('named_numbers.csv','named_numbers_three_23.csv')
