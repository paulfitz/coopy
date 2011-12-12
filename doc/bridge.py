import coopy

class TableDiff(coopy.Patcher):
    
    def changeRow(self,change):
        print("Row change of type '%s'"%change.modeString())
        for c in change.cond.keys():
            print("* condition: %s = %s"%(c,change.cond[c].toString()))
        for v in change.val.keys():
            print("* value: %s -> %s"%(v,change.val[v].toString()))
        print("")
        return True

flags = coopy.CompareFlags()
differ = TableDiff()
parser = coopy.PatchParser(differ,"diff.xls",flags)
parser.applyPatch()
