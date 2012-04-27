require 'coopy/diff_output'

class Coopy::DiffOutputStats < DiffOutput
  attr_accessor :row

  def initialize
    @row = {}
  end

  def row_inc(tag)
    @row[tag] = 0 unless @row.has_key? tag
    @row[tag] = @row[tag]+1
  end

  def apply_row(rc)
    row_inc(:all)
    case rc.row_mode
    when "+++"
      row_inc(:insert)
    when "---"
      row_inc(:delete)
    when "->"
      row_inc(:update)
    when "..."
      row_inc(:skip)
    when ""
      row_inc(:context)
    end
  end

  def add(sel,sel_tag,tag)
    v = 0
    v = sel[tag] if sel.has_key? tag
    "#{sel_tag.to_s},#{tag.to_s},#{v}\n"
  end

  def to_string
    txt = ""
    [:all,:insert,:delete,:update,:skip,:context].each do |tag|
      txt = txt + add(@row,:row,tag)
    end
    txt
  end
end

