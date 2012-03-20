
import com.healthmarketscience.jackcess.Database;
import com.healthmarketscience.jackcess.Table;
import com.healthmarketscience.jackcess.TableBuilder;
import com.healthmarketscience.jackcess.ColumnBuilder;
import com.healthmarketscience.jackcess.Cursor;
import com.healthmarketscience.jackcess.Column;
import java.sql.Types;
import java.lang.System;
import java.io.File;
import java.util.Set;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

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

    public Database create(String fname) {
	try {
	    return Database.create(new File(fname));
	} catch (Exception e) {
	    System.out.println("fail! " + e);
	}
	return null;
    }

    public void addTable(Database db, TableBuilder tbl) {
	try {
	    tbl.toTable(db);
	} catch (Exception e) {
	    System.out.println("fail! " + e);
	}
    }

    public Database read(String fname) {
	try {
	    return Database.open(new File(fname),false);
	} catch (Exception e) {
	    System.out.println(e);
	}
	return null;
    }

    public List<String> getTableNames(Database d) {
	try {
	    Set<String> n = d.getTableNames();
	    ArrayList<String> out = new ArrayList<String>();
	    for(String name : n) {
		out.add(name);
	    }
	    return out;
	} catch (Exception e) {
	    System.out.println(e);
	}
	return null;
    }

    public int getCount(List<?> names) {
	return names.size();
    }

    public String getElement(List<String> names, int x) {
	return names.get(x);
    }

    public Table getTable(Database db, String name) {
	try {
	    return db.getTable(name);
	} catch (Exception e) {
	    System.out.println(e);
	}
	return null;
    }

    public List<String> getColumns(Table tbl) {
	List<Column> cs = tbl.getColumns();
	ArrayList<String> out = new ArrayList<String>();
	for(Column c : cs) {
	    out.add(c.getName());
	}
	return out;
    }

    public Cursor createCursor(Table table) {
	return Cursor.createCursor(table);
    }

    public List<String> getNextRow(Cursor c) {
	try {
	    Map<String,Object> m = c.getNextRow();
	    ArrayList<String> out = new ArrayList<String>();
	    for(Object val : m.keySet()) {
		out.add(m.get(val).toString());
	    }
	    return out;
	} catch (Exception e) {
	    //System.out.println(e);
	    // ok
	}
	return null;
    }

    public Cursor.Savepoint getSavepoint(Cursor cursor) {
	return cursor.getSavepoint();
    }

    public void restoreSavepoint(Cursor cursor, Cursor.Savepoint savepoint) {
	try {
	    cursor.restoreSavepoint(savepoint);
	} catch (Exception e) {
	    System.out.println("fail! " + e);
	}
    }

    public boolean setCurrentRowValue(Table table, Cursor cursor, 
				      String columnName, 
				      String value) {
	Column column = table.getColumn(columnName);
	try {
	    cursor.setCurrentRowValue(column,value);
	} catch (Exception e) {
	    System.out.println("fail! " + e);
	    return false;
	}
	return true;
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
