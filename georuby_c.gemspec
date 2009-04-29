# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{georuby_c}
  s.version = "0.0.0"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Marcos Augusto"]
  s.date = %q{2009-04-28}
  s.email = %q{x@nofxx.com}
  s.extra_rdoc_files = [
    "README.txt"
  ]
  s.files = [
    "History.txt",
    "License.txt",
    "Manifest.txt",
    "README.txt",
    "Rakefile",
    "VERSION.yml",
    "examples/contains.rb",
    "examples/intersects.rb",
    "lib/georuby_c.rb",
    "lib/georuby_c/base/envelope.rb",
    "lib/georuby_c/base/ewkt_parser.rb",
    "lib/georuby_c/base/geometry.rb",
    "lib/georuby_c/base/geometry_collection.rb",
    "lib/georuby_c/base/geometry_factory.rb",
    "lib/georuby_c/base/georss_parser.rb",
    "lib/georuby_c/base/helper.rb",
    "lib/georuby_c/base/line_string.rb",
    "lib/georuby_c/base/linear_ring.rb",
    "lib/georuby_c/base/multi_line_string.rb",
    "lib/georuby_c/base/multi_point.rb",
    "lib/georuby_c/base/multi_polygon.rb",
    "lib/georuby_c/base/point.rb",
    "lib/georuby_c/base/polygon.rb",
    "lib/georuby_c/shp4r/dbf.rb",
    "lib/georuby_c/shp4r/shp.rb",
    "lib/georuby_c/version.rb",
    "spec/famous_mocks.rb",
    "spec/georuby_c/base/envelope_spec.rb",
    "spec/georuby_c/base/geometry_collection_spec.rb",
    "spec/georuby_c/base/geometry_factory_spec.rb",
    "spec/georuby_c/base/geometry_spec.rb",
    "spec/georuby_c/base/line_string_spec.rb",
    "spec/georuby_c/base/linear_ring_spec.rb",
    "spec/georuby_c/base/multi_point_spec.rb",
    "spec/georuby_c/base/multi_polygon_spec.rb",
    "spec/georuby_c/base/point_spec.rb",
    "spec/georuby_c/base/polygon_spec.rb",
    "spec/georuby_c/native/native_point_spec.rb",
    "spec/georuby_c_spec.rb",
    "spec/spec.opts",
    "spec/spec_helper.rb"
  ]
  s.has_rdoc = true
  s.homepage = %q{http://github.com/nofxx/georuby_c}
  s.rdoc_options = ["--charset=UTF-8"]
  s.require_paths = ["lib"]
  s.rubygems_version = %q{1.3.2}
  s.summary = %q{GeoRuby with some ext C}
  s.test_files = [
    "spec/georuby_c_spec.rb",
    "spec/georuby_c/native/native_point_spec.rb",
    "spec/georuby_c/base/polygon_spec.rb",
    "spec/georuby_c/base/linear_ring_spec.rb",
    "spec/georuby_c/base/geometry_factory_spec.rb",
    "spec/georuby_c/base/geometry_spec.rb",
    "spec/georuby_c/base/multi_point_spec.rb",
    "spec/georuby_c/base/line_string_spec.rb",
    "spec/georuby_c/base/geometry_collection_spec.rb",
    "spec/georuby_c/base/multi_polygon_spec.rb",
    "spec/georuby_c/base/point_spec.rb",
    "spec/georuby_c/base/envelope_spec.rb",
    "spec/famous_mocks.rb",
    "spec/spec_helper.rb",
    "examples/intersects.rb",
    "examples/contains.rb"
  ]

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
