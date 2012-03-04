
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
	SocialCalc.ExecuteSheetCommand(spreadsheet.sheet, new SocialCalc.Parse("set " + SocialCalc.crToCoord(x+1,y+1) + " text t __FIGURE_OUT_NULL__"), false);
    }
}

function sheet_get_cell(x,y) {
    var cell = spreadsheet.sheet.cells[SocialCalc.crToCoord(x+1,y+1)] || null;
    if (cell==null) return null;
    return cell.datavalue;
}

function sheet_get_width() {
    return spreadsheet.sheet.attribs.lastcol;
}

function sheet_get_height() {
    return spreadsheet.sheet.attribs.lastrow;
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

