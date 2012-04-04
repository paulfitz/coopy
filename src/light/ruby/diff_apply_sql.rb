require 'diff_output_action'
require 'dbi'

# for now, assume no schema changes, and a single table
class DiffApplySql < DiffOutputAction
  def initialize(db)
    @db = db
  end

  def row_insert(rc)
    cols = rc.active_columns
    @db.insert(nil,
               cols.map{|c| c[:title]},
               cols.map{|c| rc.value_at(c)})
  end

  def row_delete(rc)
    cols = rc.active_columns
    @db.delete(nil,
               cols.map{|c| c[:title]},
               cols.map{|c| rc.value_at(c)})
  end

  def row_update(rc)
    cols = rc.active_columns
    touched_cols = cols.select{|c| !rc.new_value_at(c).nil?}
    @db.update(nil,
               touched_cols.map{|c| c[:title]},
               touched_cols.map{|c| rc.new_value_at(c)},
               cols.map{|c| c[:title]},
               cols.map{|c| rc.value_at(c)})
  end
end

