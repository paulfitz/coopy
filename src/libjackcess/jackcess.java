
import com.healthmarketscience.jackcess.Database;
import com.healthmarketscience.jackcess.Table;
//import com.healthmarketscience.jackcess.Column;
import com.healthmarketscience.jackcess.TableBuilder;
import com.healthmarketscience.jackcess.ColumnBuilder;
import java.sql.Types;
import java.lang.System;
import java.io.File;
//import java.io.IOException;

public class jackcess
{
    public int hello() {
	return 42;
    }

    public int go() {
	try {
	    System.out.println("foo!");
	    Database db = Database.create(new File("new.mdb"));
	    System.out.println("foo 2!");
	    Table newTable = new TableBuilder("NewTable")
		.addColumn(new ColumnBuilder("a")
			   .setSQLType(Types.INTEGER)
			   .toColumn())
		.addColumn(new ColumnBuilder("b")
			   .setSQLType(Types.VARCHAR)
			   .toColumn())
		.toTable(db);
	    System.out.println("foo!");
	    newTable.addRow(1, "foo");
	    System.out.println("done!");

	} catch (Exception e) {
	    System.out.println("fail! " + e);
	}
	return 99;
    }
}
