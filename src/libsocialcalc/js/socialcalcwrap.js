
var socialcalcwrap_loaded = true;

function create_spreadsheet() {
    return new SocialCalc.SpreadsheetControl();
}

var document = new Array();
document['getElementById'] = function(x) {
    return new Array();
}

var spreadsheet = null;

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

function get_csv() {
    var sheetstr0 = spreadsheet.sheet.CreateSheetSave();
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


