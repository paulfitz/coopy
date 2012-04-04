require 'sql_wrapper'
require 'sequel'

class SequelSqlBare < SqlWrapper
  def initialize(db)
    @db = db
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
    sync_table(tbl)
    @db.schema(@tname)
  end

  def column_names(tbl)
    columns(tbl).map{|x| x[0]}
  end
  
  def primary_key(tbl)
    cols = columns(tbl)
    cols.select{|x| x[1][:primary_key]}.map{|x| x[0]}
  end

  def index(tbl)
    key = primary_key(tbl)
    @t.select(*key)
  end

  def fetch(sql,names)
    @db.fetch(sql) do |row|
      yield names.map{|n| row[n]}
    end
  end
end


class SequelSqlWrapper < SequelSqlBare
  def initialize(*params)
    super(Sequel.connect(*params))
  end
end
