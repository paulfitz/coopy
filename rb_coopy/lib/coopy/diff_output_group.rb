require 'coopy/diff_columns'
require 'coopy/row_change'

class DiffOutputGroup
  def initialize(*sinks)
    @sinks = sinks
  end

  def <<(x)
    @sinks = [] if @sinks.nil?
    @sinks << x
  end

  def begin_diff
    @sinks.each { |s| s.begin_diff }
  end

  def end_diff
    @sinks.each { |s| s.end_diff }
  end

  def apply_row(rc)
    @sinks.each { |s| s.apply_row(rc) }
  end

  def to_string
    @sinks.each do |s|
      result = s.to_string
      return result if result!=""
    end
    ""
  end

  def want_context
    return @want_context0 unless @want_context0.nil?
    @want_context0 = false
    @want_context0 = @sinks.each { |s| @want_context0 ||= s.want_context }
    @want_context0
  end
end
