import coopy.PolyBook;
import coopy.CompareOutput;
import coopy.Patcher;
import coopy.CompareFlags;
import coopy.BookCompare;

class example {

    static class TableDiff extends coopy.Patcher {
	public boolean changeRow(coopy.RowChange change) {
	    System.out.println("Row change of type '" + change.modeString() + "'");
	    coopy.StringList names = change.getNames();
	    coopy.CellMap cond = change.getCond();
	    coopy.CellMap val = change.getVal();
	    for (int i=0; i<names.size(); i++) {
		String c = names.get(i);
		if (cond.has_key(c)) {
		    System.out.println("  condition: " + c + " = " + cond.get(c).toString());
		}
	    }
	    for (int i=0; i<names.size(); i++) {
		String c = names.get(i);
		if (val.has_key(c)) {
		    System.out.println("  value: " + c + " -> " + val.get(c).toString());
		}
	    }
	    return true;
	}
    }

    public static void convertFile(String src, String dest) {
	coopy.PolyBook book = new coopy.PolyBook();
	book.read(src);
	book.write(dest);
    }

    public static void generateDiff(String version1, String version2, String dest) {
	coopy.PolyBook book1 = new coopy.PolyBook();
	coopy.PolyBook book2 = new coopy.PolyBook();
	book1.read(version1);
	book2.read(version2);
	CompareOutput out = new CompareOutput(dest);
	Patcher patcher = Patcher.createByName("tdiff");
	CompareFlags flags = new CompareFlags();
	flags.setOutput(out);
	BookCompare cmp = new BookCompare();
	cmp.compare(book1,book1,book2,patcher,flags);
    }

    public static void examineDiff(String version1, String version2) {
	coopy.PolyBook book1 = new coopy.PolyBook();
	coopy.PolyBook book2 = new coopy.PolyBook();
	book1.read(version1);
	book2.read(version2);
	CompareFlags flags = new CompareFlags();
	BookCompare cmp = new BookCompare();
	cmp.compare(book1,book1,book2,new TableDiff(),new CompareFlags());
    }

    public static void main(String[] args) {
	System.loadLibrary("coopy");

	generateDiff("named_numbers.csv","named_numbers_three_23.csv",
		     "patch.diff");
	convertFile("named_numbers.csv","named_numbers.sqlite");
	examineDiff("named_numbers.csv","named_numbers_three_23.csv");
    }
}
