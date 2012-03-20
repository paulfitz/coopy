
import com.healthmarketscience.jackcess.Database;
import com.healthmarketscience.jackcess.Table;
import com.healthmarketscience.jackcess.TableBuilder;
import com.healthmarketscience.jackcess.ColumnBuilder;
import java.sql.Types;
import java.lang.System;
import java.io.File;

public class JackBox {
    public int hello() {
	return 42;
    }

    public TableBuilder makeTable(String name) {
	return new TableBuilder(name);
    }

    public void addColumn(TableBuilder tbl, String name, String code) {
	tbl.addColumn(makeColumn(name,code).toColumn());
    }

    public ColumnBuilder makeColumn(String name,String code) {
	try {
	    int t = Types.VARCHAR;
	    return new ColumnBuilder(name).setSQLType(t);
	} catch (Exception e) {
	    System.out.println("fail! " + e);
	}
	return null;
    }

    public void save(TableBuilder tbl, String fname) {
	try {
	    Database db = Database.create(new File(fname));
	    tbl.toTable(db);
	} catch (Exception e) {
	    System.out.println("fail! " + e);
	}
    }

    public int go() {
	try {
	    Database db1 = Database.open(new File("Nwind.mdb"),true);
	    System.out.println(db1.getTableNames());

	    Database db = Database.create(new File("new.mdb"));
	    Table newTable = new TableBuilder("NewTable")
		.addColumn(new ColumnBuilder("a")
			   .setSQLType(Types.INTEGER)
			   .toColumn())
		.addColumn(new ColumnBuilder("b")
			   .setSQLType(Types.VARCHAR)
			   .toColumn())
		.toTable(db);
	    newTable.addRow(1, "foo");
	} catch (Exception e) {
	    System.out.println("fail! " + e);
	}
	return 99;
    }
}
