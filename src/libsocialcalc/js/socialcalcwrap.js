
var spreadsheet = null;

var socialcalcwrap_loaded = true;

function create_spreadsheet() {
    spreadsheet = new SocialCalc.SpreadsheetControl();
    return spreadsheet;
}

var document = new Array();
document['getElementById'] = function(x) {
    return new Array();
}

function set_active_sheet(sheet) {
    spreadsheet = sheet;
}

function sheet_size(w,h) {
    spreadsheet.sheet.attribs.lastcol = w;
    spreadsheet.sheet.attribs.lastrow = h;
}

function load_from_string(savestr) {
    var parts = spreadsheet.DecodeSpreadsheetSave(savestr);
    if (parts) {
	if (parts.sheet) {
            spreadsheet.sheet.ResetSheet();
            spreadsheet.ParseSheetSave(savestr.substring(parts.sheet.start, parts.sheet.end));
        }
	//if (parts.edit) {
        //spreadsheet.editor.LoadEditorSettings(savestr.substring(parts.edit.start, parts.edit.end));
	//}
    }
}

function get_full() {
    var sheetstr = spreadsheet.CreateSpreadsheetSave();
    //var sheetstr = SocialCalc.ConvertSaveToOtherFormat(sheetstr0,"csv",false);
    return sheetstr;
}

function get_sheet_save() {
    //var r = SocialCalc.crToCoord(spreadsheet.sheet.attribs.lastcol,spreadsheet.sheet.attribs.lastrow);
    //return spreadsheet.sheet.CreateSheetSave("A1:" + r);
    return spreadsheet.sheet.CreateSheetSave();
}

function get_csv() {
    var sheetstr0 = get_sheet_save();
    var sheetstr = SocialCalc.ConvertSaveToOtherFormat(sheetstr0,"csv",false);
    return sheetstr;
}

function load_from_csv(savestr0) {
    var savestr = SocialCalc.ConvertOtherFormatToSave(savestr0,"csv");
    spreadsheet.sheet.ResetSheet();
    spreadsheet.ParseSheetSave(savestr);
    return "ok";
}

function set_src(x) {
    load_from_string(x);
    return "ok";
}

function sheet_set_cell(x,y,v) {
    if (v!=null) {
	SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("set " + SocialCalc.crToCoord(x+1,y+1) + " text t " + v), false);
    } else {
	SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("erase " + SocialCalc.crToCoord(x+1,y+1) + " all"), false);
    }
}

function sheet_get_cell(x,y) {
    var coord = SocialCalc.crToCoord(x+1,y+1);
    if (coord in spreadsheet.sheet.cells) {
	var cell = spreadsheet.sheet.cells[coord];
	if (!cell) return null;
	if (cell.datatype==null) return null;
	return cell.datavalue;
    }
    return null;
}

function sheet_get_width() {
    return spreadsheet.sheet.attribs.lastcol;
}

function sheet_get_height() {
    return spreadsheet.sheet.attribs.lastrow;
}

function sheet_move_column(x1,x2,w,h) {
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("cut " + SocialCalc.crToCoord(x1+1,1) + ":" + SocialCalc.crToCoord(x1+1,h)  + " all"), false);
    var dest = x2;
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("deletecol " + SocialCalc.crToCoord(x1+1,1)), false);
    if (dest>x1) {
	dest--;
    }
    if (dest<0) {
	dest = w-1;
    }
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("insertcol " + SocialCalc.crToCoord(dest+1,1)), false);
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("paste " + SocialCalc.crToCoord(dest+1,1) + ":" + SocialCalc.crToCoord(dest+1,h)  + " all"), false);
    return dest;
}

function sheet_insert_column(x,w,h) {
    sheet_size(w,h);
    if (x<0) x = w;
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("insertcol " + SocialCalc.crToCoord(x+1,1)), false);
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("erase " + SocialCalc.crToCoord(x+1,1) + ":" + SocialCalc.crToCoord(x+1,h)  + " all"), false);
    return x;
}

function sheet_delete_column(x,w,h) {
    sheet_size(w,h);
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("deletecol " + SocialCalc.crToCoord(x+1,1)), false);
    return x;
}

function sheet_insert_row(y,w,h) {
    sheet_size(w,h);
    if (y<0) y = h;
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("insertrow " + SocialCalc.crToCoord(1,y+1)), false);
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("erase " + SocialCalc.crToCoord(1,y+1) + ":" + SocialCalc.crToCoord(w,y+1)  + " all"), false);
    return y;
}

function sheet_delete_rows(y1,y2,w,h) {
    sheet_size(w,h);
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("deleterow " + SocialCalc.crToCoord(1,y1+1) + ":" + SocialCalc.crToCoord(1,y2+1)), false);
    return y2-y1+1;
}


function test_sheet() {
    //SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("set sheet lastcol 4"), false);
    //SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("set sheet lastrow 4"), false);
    //SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("set sheet usermaxcol 4"), false);
    //SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("set sheet usermaxrow 4"), false);
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("set A1:B5 text t hello"), false);
    SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("insertrow A1"), false);
    //SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("insertcol A1"), false);
    return get_csv();
}

