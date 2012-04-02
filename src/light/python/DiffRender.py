
class DiffRender:

  def begin_table(self):
      pass

  def begin_row(self,mode):
      pass

  def insert_cell(self,txt,mode,separator):
      pass

  def end_row(self):
      pass

  def end_table(self):
      pass

  def apply(self,rows):
    render = self
    render.begin_table()
    change_row = None

    for r in rows:
      row_mode = ""
      open = False
      if len(r) == 0:
          continue
      txt = r[0]
      if txt=="@" or txt=="@@":
        row_mode = "@@"
      elif txt=="!" or txt=="+++" or txt=="---":
        row_mode = txt
      elif txt=="!":
        change_row = r
      else:
        open = True

      cmd = txt
      render.begin_row(row_mode)
      for c in range(len(r)):
        txt = r[c]
        if txt=="NULL":
            txt = ""
        cell_mode = ""
        separator = ""
        if open and not(change_row == None):
          change = change_row[c]
          if change=="+++" or change == "---":
            cell_mode = change

        if str(cmd).find("->")>=0:
          if str(txt).find(cmd)>=0:
            cell_mode = "->"
            separator = cmd

        render.insert_cell(txt,cell_mode,separator)

      render.end_row()

    render.end_table()

  def to_string(self):
    return ""

class DiffRenderHtml(DiffRender):
  def __init__(self):
    self.text_to_insert = []

  def insert(self,str):
    self.text_to_insert.append(str)

  def begin_table(self):
    self.insert("<table class='csv_sheet'>\n")

  def begin_row(self,mode):
    self.td_open = '<td'
    self.td_close = '</td>'
    self.row_color = ""
    self.open = False
    if mode=="@@":
      self.td_open = "<th"
      self.td_close = "</th>"
    elif mode=="!":
      self.row_color = "#aaaaaa"
    elif mode=="+++":
      self.row_color = "#7fff7f"
    elif mode=="---":
      self.row_color = "#ff7f7f"
    else:
      self.open = True

    tr = "<tr>";
    row_decorate = ""
    if self.row_color!="":
      row_decorate = " bgcolor=\"" + self.row_color + "\" style=\"background-color: " + self.row_color + ";\""
      tr = "<tr" + row_decorate + ">"

    self.insert(tr)

  def insert_cell(self,txt,mode,separator):
    cell_decorate = ""
    if mode=="+++":
      cell_decorate = " bgcolor=\"#7fff7f\" style=\"background-color: #7fff7f;\""
    elif mode=="---":
      cell_decorate = " bgcolor=\"#ff7f7f\" style=\"background-color: #ff7f7f;\""
    elif mode=="->":
	cell_decorate = " bgcolor=\"#7f7fff\" style=\"background-color: #7f7fff;\""

    self.insert(self.td_open+cell_decorate+">")
    self.insert(txt)
    self.insert(self.td_close)

  def end_row(self):
    self.insert("</tr>\n")

  def end_table(self):
    self.insert("</table>\n")

  def html(self):
    return "".join(self.text_to_insert)

  def to_string(self):
    return self.html()




class DiffRenderLog(DiffRender):
  def __init__(self):
    self.r = 0
    self.c = 0
    self.row_mode = ""
    self.row_log = []
    self.cell_log = []

  def begin_table(self):
      pass

  def begin_row(self,mode):
    self.row_mode = mode
    self.row_log.append([self.r, mode])
    self.c = 0

  def insert_cell(self,txt,mode,separator):
    self.cell_log.append({
      'col': self.c,
      'row': self.r,
      'txt': txt,
      'cell_mode': mode,
      'row_mode': self.row_mode,
      'separator': separator
    })
    self.c = self.c + 1

  def end_row(self):
    self.r = self.r + 1

  def end_table(self):
    pass
  
  def log(self):
    txt = ""
    for v in self.cell_log:
      txt = txt + str(v['col']) + "," + str(v['row']) + " [" + v['cell_mode'] + "/" + v['row_mode'] + "] " + v['separator'] + " : " + str(v['txt']) + "\n"

    return txt

  def to_string(self):
    return self.log()
