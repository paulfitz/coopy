require 'coopy/sqlite_sql_wrapper'
require 'coopy/sql_compare'
require 'coopy/diff_apply_sql'
require 'sqlite3'
require "fastercsv" unless RUBY_VERSION > "1.9"

class Coopy::DatabaseContainer 

  def initialize
    @foreign1 = nil
    @foreign2 = nil
    @completed = false
  end

  def set_table_names(names)
    @table_names = names
    @name1 = names[0]
    @base1 = "main"
    @name2 = names[1]
    @base2 = "main"
  end

  def open_primary(file_name)
    @db = SQLite3::Database.new(file_name)
    @sql = Coopy::SqliteSqlWrapper.new(@db)
    begin
      chk = @sql.get_table_names[0]
      @name1 = chk if @name1.nil?
    rescue => e
      # not an sql database; load it in to memory
      @db = SQLite3::Database.new(":memory:")
      @sql = Coopy::SqliteSqlWrapper.new(@db)
      @foreign1 = true
    end
  end

  def open_secondary(file_name)
    begin
      @db.execute("ATTACH ? AS `__peer__`",file_name)
    rescue
      @foreign2 = true
    end  
    @base2 = "__peer__"
    @name2 = @name1 if @name2.nil?
  end


  def complete_sql
    return if @completed
    @completed = !(@foreign1||@foreign2)
    return if @completed
    CSV.foreach(file_name) do |line|
      puts line[1]
    end
    @completed = true
  end

  def apply_tool
    complete_sql
    Coopy::DiffApplySql.new(@sql,"#{@base1}.#{@name1}")
  end

  def compare_tool
    complete_sql
    Coopy::SqlCompare.new(@sql,"#{@base1}.#{@name1}","#{@base2}.#{@name2}")
  end
end
