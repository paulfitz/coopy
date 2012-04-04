require 'sql_wrapper'
require 'sequel'

class SequelSqlWrapper
  def initialize(*params)
    @db = Sequel.connect(*params)
    @tname = nil
    @t = nil
  end

  def sync_table(tbl)
    tbl = @tname if tbl.nil?
    tbl = @db.tables[0] if tbl.nil?
    return @t if tbl==@tname
    @tname = tbl
    @t = @db[tbl]
  end

  def enhash(cols,vals)
    Hash[*cols.map{|c| c.to_sym}.zip(vals).flatten]
  end

  def insert(tbl,cols,vals)
    sync_table(tbl)
    @t.insert(enhash(cols,vals))
  end

  def delete(tbl,cols,vals)
    sync_table(tbl)
    @t.filter(enhash(cols,vals)).delete
  end
  
  def update(tbl,set_cols,set_vals,cond_cols,cond_vals)
    sync_table(tbl)
    @t.filter(enhash(cond_cols,cond_vals)).update(enhash(set_cols,set_vals))
  end

  def transaction(&block)
    @db.transaction(&block)
  end

  def columns(tbl)
    tbl = complete_table(tbl)
    puts tbl.inspect
    @db.columns(tbl)
  end
end
