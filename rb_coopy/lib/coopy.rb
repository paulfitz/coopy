require 'optparse'

class Coopy

  class Flavor
    attr_accessor :key
    attr_accessor :banner
    attr_accessor :min_length

    def sql_subject?
      [:diff,:patch].include? key
    end

    def sql_object?
      key == :diff
    end

    def can_choose_format?
      key != :patch
    end

    def can_set_output?
      key != :patch
    end
    
    def can_set_table?
      key != :rediff
    end

    def default_format
      (key==:patch) ? :apply : :csv
    end
  end

  class OpenStruct
    attr_accessor :format
    attr_accessor :output
    attr_accessor :table
    attr_accessor :debug
  end

  def self.parse(flavor,args)
    options = OpenStruct.new
    options.format = flavor.default_format
    options.output = nil
    options.table = []
    options.debug = false
    OptionParser.new do |opts|
      begin
        opts.banner = flavor.banner
        opts.separator ""
        opts.separator "Specific options"
        if flavor.can_choose_format?
          opts.on("-f","--format [FORMAT]", [:csv, :html, :tdiff, :apply, :stats, :raw],
                  "select format (csv,html,tdiff,apply,stats,raw)") do |fmt|
            options.format = fmt
          end
        end
        if flavor.can_set_output?
          opts.on("-o", "--output [FILENAME]",
                  "direct output to a file") do |name|
            options.output = name
          end
        end
        if flavor.can_set_table?
          opts.on("-t", "--table [TABLE]",
                  "use specified table name") do |name|
            options.table << name
          end
        end
        opts.on_tail("-d", "--debug", "Debug mode") do
          options.debug = true
        end
        opts.on_tail("-h", "--help", "Show this message") do
          puts opts
          exit
        end
        opts.parse!(args)
        return options
      rescue
        puts "#{$!} (--help for help)"
        exit 1
      end
    end
  end

  def self.core(options,flavor,argv)
    if argv.length < flavor.min_length
      self.parse(flavor,["--help"])
      exit(1)
    end

    data = DatabaseContainer.new
    data.set_table_names(options.table)

    if flavor.sql_subject?
      data.open_primary(argv[0])
    end

    if flavor.sql_object?
      data.open_secondary(argv[1])
      cmp = data.compare_tool
    else
      cmp = DiffParser.new(argv[flavor.min_length-1])
    end

    patches = DiffOutputGroup.new
    # patches << DiffOutputRaw.new
    case options.format
    when :html
      patches << DiffRenderHtml.new
    when :tdiff
      patches << DiffOutputTdiff.new
    when :csv
      patches << DiffRenderCsv.new
    when :apply
      patches << data.apply_tool
    when :raw
      patches << DiffOutputRaw.new
    when :stats
      patches << DiffOutputStats.new
    else
      patches << DiffRenderCsv.new
    end

    cmp.set_output(patches)
    
    cmp.apply
    result = patches.to_string
    if result != ""
      if options.output.nil?
        print result
      else
        File.open(options.output,"w") do |f|
          f << result
        end
      end
    end
    0
  end

  def self.core_wrap(flavor,argv)
    options = self.parse(flavor,argv)
    begin
      self.core(options,flavor,argv)
    rescue => e
      if options.debug
        $stderr.puts  "FAILURE for #{flavor} called with: #{argv.inspect}"
        raise e
      end
      puts e
      exit(1)
    end
  end

  def self.diff(argv)
    flavor = Flavor.new
    flavor.key = :diff
    flavor.banner = "Usage: sqlite_diff [options] ver1.sqlite ver2.sqlite"
    flavor.min_length = 2
    self.core_wrap(flavor,argv)
  end

  def self.patch(argv)
    flavor = Flavor.new
    flavor.key = :patch
    flavor.banner = "Usage: sqlite_patch [options] db.sqlite patch.csv"
    flavor.min_length = 2
    self.core_wrap(flavor,argv)
  end

  def self.rediff(argv)
    flavor = Flavor.new
    flavor.key = :rediff
    flavor.banner = "Usage: sqlite_rediff [options] patch.csv"
    flavor.min_length = 1
    self.core_wrap(flavor,argv)
  end
end

require 'coopy/diff_output_raw'
require 'coopy/diff_output_tdiff'
require 'coopy/diff_render_html'
require 'coopy/diff_render_csv'
require 'coopy/diff_output_action'
require 'coopy/diff_output_group'
require 'coopy/diff_output_stats'
require 'coopy/diff_parser'
require 'coopy/database_container'


