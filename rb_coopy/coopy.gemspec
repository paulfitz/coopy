Gem::Specification.new do |s|
  s.name               = "coopy"
  s.version            = "0.6.4.1"
  # s.default_executable = "sqlite_diff"
  s.executables << 'sqlite_diff'
  s.executables << 'sqlite_patch'
  s.executables << 'sqlite_rediff'

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Paul Fitzpatrick"]
  s.date = %q{2012-04-10}
  s.description = %q{Data diffs/patches}
  s.email = %q{paul@robotrebuilt.com}
  s.files = Dir["Rakefile", "lib/coopy.rb", "lib/coopy/*.rb", "bin/sqlite_diff", "bin/sqlite_rediff", "bin/sqlite_patch"]
  s.test_files = ["test/test_coopy.rb"]
  s.homepage = %q{http://share.find.coop/}
  s.require_paths = ["lib"]
  s.rubygems_version = %q{1.6.2}
  s.summary = %q{Coopy!}
  # s.add_dependency('sqlite3') # dependency seems to do more harm than good

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end

