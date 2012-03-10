
// DiffRender: generate html version of a diff

function DiffRender() {
    this.text_to_insert = [];
    this.text_index = 0;
}

DiffRender.prototype.insert = function(str) {
    this.text_to_insert[this.text_index++] = str;
}

DiffRender.prototype.begin_table = function() {
    this.insert("<table class='csv_sheet'>\n");
}

DiffRender.prototype.begin_row = function(mode) {
    this.td_open = '<td';
    this.td_close = '</td>';
    this.row_color = "";
    this.open = false;
    switch(mode) {
    case "@@":
	this.td_open = "<th";
	this.td_close = "</th>";
	break;
    case "!":
	this.row_color = "#aaaaaa";
	break;
    case "+++":
	this.row_color = "#7fff7f";
	break;
    case "---":
	this.row_color = "#ff7f7f";
	break;
    default:
	this.open = true;
	break;
    }
    var tr = "<tr>";
    var row_decorate = "";
    if (this.row_color!="") {
	row_decorate = " bgcolor=\"" + this.row_color + "\" style=\"background-color: " + this.row_color + ";\"";
	tr = "<tr" + row_decorate + ">";
    }
    this.insert(tr);
}

DiffRender.prototype.insert_cell = function(txt,mode,separator) {
    var cell_decorate = "";
    switch (mode) {
    case "+++":
	cell_decorate += " bgcolor=\"#7fff7f\" style=\"background-color: #7fff7f;\"";
	break;
    case "---":
	cell_decorate += " bgcolor=\"#ff7f7f\" style=\"background-color: #ff7f7f;\"";
	break;
    case "->":
	cell_decorate += " bgcolor=\"#7f7fff\" style=\"background-color: #7f7fff;\"";
	break;
    }

    this.insert(this.td_open+cell_decorate+">");
    this.insert(txt);
    this.insert(this.td_close);
}

DiffRender.prototype.end_row = function() {
    this.insert('</tr>\n');
}

DiffRender.prototype.end_table = function() {
    this.insert('</table>\n');
}

DiffRender.prototype.html = function() {
    return this.text_to_insert.join('');
}

function render_diff(render,rows) {
    render.begin_table();
    var change_row = [];
    for (row in rows) {
	var r = rows[row];
	var row_mode = "";
	var txt = "";
	var open = false;
	if (r.length>0) {
	    txt = r[0];
	    if (txt=="@"||txt=="@@") {
		row_mode = "@@";
	    } else if (txt=="!"||txt=="+++"||txt=="---") {
		row_mode = txt;
                if (txt=="!") { change_row = r; }
	    } else {
		open = true;
	    }
	}
	var cmd = txt;
	render.begin_row(row_mode);
	for (c in r) {
	    txt = r[c];
	    if (txt=="NULL") txt = "";
	    var cell_mode = "";
	    var separator = "";
	    if (open && change_row.length>0) {
		var change = change_row[c];
		if (change=="+++"||change=="---") {
		    cell_mode = change;
		}
	    }

	    if (cmd.indexOf("->")>=0) {
		if (txt.indexOf(cmd)>=0) {
		    cell_mode = "->";
		    separator = cmd;
		}
	    }
	    render.insert_cell(txt,cell_mode,separator);
	}
	render.end_row();
    }
    render.end_table();
}
