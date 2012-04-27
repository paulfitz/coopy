require 'test/unit'
require 'coopy'
require 'tmpdir'
require 'fileutils'

class CoopyTest < Test::Unit::TestCase
  def initialize(*args)
    super(*args)
    odir = File.dirname(__FILE__)
    @broken_bridges = File.join(odir,"broken_bridges.sqlite")
    @bridges = File.join(odir,"bridges.sqlite")
    @dir = nil
  end

  def setup
    @dir = Dir.mktmpdir
    @ct = 0
    @output = ""
    new_output
  end

  def teardown
    FileUtils.remove_entry_secure @dir
    @dir = nil
  end

  def new_output
    @ct = @ct+1
    tmp = @output
    @output = File.join(@dir,"output#{@ct}.txt")
    tmp
  end

  def cp(src,key)
    target = File.join(@dir,"#{key}.sqlite")
    FileUtils.cp src, target
    target
  end

  def read(fname)
    begin
      File.open(fname,"r") do |f|
        return f.read
      end
    rescue
      return ""
    end
  end

  def diff(*argv)
    Coopy.diff(["--debug","--output",@output] + argv)
    new_output
  end

  def rediff(*argv)
    Coopy.rediff(["--debug","--output",@output] + argv)
    new_output
  end

  def patch(*argv)
    Coopy.patch(["--debug"] + argv)
  end

  def assert_rows_equal(x1,x2)
    txt = read(diff("--format","stats",x1,x2))
    assert_match(/row,insert,0$/,txt)
    assert_match(/row,delete,0$/,txt)
    assert_match(/row,update,0$/,txt)
  end

  def assert_rows_unequal(x1,x2)
    txt = read(diff("--format","stats",x1,x2))
    assert_match(/row,((insert)|(delete)|(update)),[^0]/,txt)
  end

  def test_basic_stats
    txt = read(diff("--format","stats",@broken_bridges,@bridges))
    assert_match(/row,insert,1$/,txt)
    assert_match(/row,delete,1$/,txt)
    assert_match(/row,update,1$/,txt)
  end

  def test_basic_csv
    txt = read(diff("--format","csv",@broken_bridges,@bridges))
    assert_match(/,/,txt)
    assert_no_match(/<tr/,txt)
  end

  def test_basic_html
    txt = read(diff("--format","html",@broken_bridges,@bridges))
    assert_match(/<tr/,txt)
  end

  def test_basic_apply
    v1 = cp(@broken_bridges,"v1")
    v2 = cp(@bridges,"v2")
    assert_rows_unequal(v1,v2)
    diff("--format","apply",v1,v2)
    assert_rows_equal(v1,v2)
  end

  def diff_patch(b1,b2)
    v1 = cp(b1,"v1")
    v2 = cp(b2,"v2")
    @diff = diff(v1,v2)
    assert_rows_unequal(v1,v2)
    patch(v1,@diff)
    assert_rows_equal(v1,v2)
  end

  def test_basic_diff_patch
    diff_patch(@broken_bridges,@bridges)
  end

  def test_reverse_diff_patch
    diff_patch(@bridges,@broken_bridges)
  end

  def test_basic_rediff
    v1 = cp(@broken_bridges,"v1")
    v2 = cp(@bridges,"v2")
    @diff = diff(v1,v2)
    @diff2 = rediff(@diff)
    assert_equal read(@diff), read(@diff2)
  end
end
