require 'diff_render'

class DiffRenderHtml < DiffRender
  def initialize
    @text_to_insert = []
  end

  def insert(str)
    @text_to_insert << str
  end

  def begin_table
    self.insert "<table class='csv_sheet'>\n"
  end

  def begin_row(mode)
    @td_open = '<td';
    @td_close = '</td>';
    @row_color = "";
    @open = false;
    case mode
    when "@@"
      @td_open = "<th"
      @td_close = "</th>"
    when "!"
      @row_color = "#aaaaaa"
    when "+++"
      @row_color = "#7fff7f";
    when "---"
      @row_color = "#ff7f7f";
    else
      @open = true
    end
    tr = "<tr>";
    row_decorate = ""
    if @row_color!=""
      row_decorate = " bgcolor=\"" + @row_color + "\" style=\"background-color: " + @row_color + ";\""
      tr = "<tr" + row_decorate + ">"
    end
    self.insert(tr)
  end

  def insert_cell(txt,mode,separator)
    cell_decorate = ""
    case mode
    when "+++"
      cell_decorate = " bgcolor=\"#7fff7f\" style=\"background-color: #7fff7f;\"";
    when "---"
      cell_decorate = " bgcolor=\"#ff7f7f\" style=\"background-color: #ff7f7f;\"";
    when "->":
	cell_decorate = " bgcolor=\"#7f7fff\" style=\"background-color: #7f7fff;\"";
    end
    self.insert @td_open+cell_decorate+">"
    self.insert txt
    self.insert @td_close
  end

  def end_row
    self.insert "</tr>\n"
  end

  def end_table
    self.insert "</table>\n"
  end

  def html
    @text_to_insert.join ''
  end

  def to_string
    html
  end
end

