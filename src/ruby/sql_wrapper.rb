class SqlWrapper
  def insert(tbl,cols,vals)
  end

  def delete(tbl,cols,vals)
  end

  def update(tbl,set_cols,set_vals,cond_cols,cond_vals)
  end

  def column_names(tbl)
    []
  end

  def primary_key(tbl)
    []
  end

  def except_primary_key(tbl)
    column_names(tbl)-primary_key(tbl)
  end

  def fetch(sql)
    []
  end

  def quote_column(c)
    c.to_s
  end

  def quote_table(t)
    t.to_s
  end
end
