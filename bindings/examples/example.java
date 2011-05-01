import coopy.PolyBook;
import coopy.CompareOutput;
import coopy.Patcher;
import coopy.CompareFlags;
import coopy.BookCompare;

class example {

    public static void main(String[] args) {
	System.loadLibrary("coopy");

	coopy.PolyBook book1 = new coopy.PolyBook();
	coopy.PolyBook book2 = new coopy.PolyBook();
	
	book1.read("named_numbers.csv");
	book2.read("named_numbers_three_23.csv");
	book1.write("named_numbers.sqlite");
	CompareOutput out = new CompareOutput("patch.diff");
	
	Patcher patcher = Patcher.createByName("tdiff");
	CompareFlags flags = new CompareFlags();
	flags.setOutput(out);
	BookCompare cmp = new BookCompare();
	cmp.compare(book1,book1,book2,patcher,flags);
    }
}
