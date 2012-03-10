
// LogDiffRender : alternate render output for test purposes

function LogDiffRender() {
    this.row = 0;
    this.col = 0;
    this.row_mode = "";
    this.row_log = [];
    this.cell_log = [];
}

LogDiffRender.prototype.begin_table = function() {
}

LogDiffRender.prototype.begin_row = function(mode) {
    this.row_mode = mode;
    this.row_log.push([this.row, mode]);
    this.col = 0;
}

LogDiffRender.prototype.insert_cell = function(txt,mode,separator) {
    this.cell_log[this.cell_log.length] =
	{ col: this.col, 
	  row: this.row, 
	  txt: txt, 
	  cell_mode: mode, 
	  row_mode: this.row_mode,
	  separator: separator };
    this.col++;
}

LogDiffRender.prototype.end_row = function() {
    this.row++;
}

LogDiffRender.prototype.end_table = function() {
}

var input = null;

function set_input(jsn) {
    input = eval('(' + jsn + ')').sheet.rows;
}

function test_reader() {
    var render = new LogDiffRender();
    render_diff(render,input);
    var log = render.cell_log;
    var txt = "";
    for (var it in log) {
	var v = log[it];
	txt += v.col + "," + v.row + " [" + v.cell_mode + "/" +
	    v.row_mode + "] " + v.separator + " : " + v.txt + "\n";
    }
    return txt;
}