Gem::Specification.new do |s|
  s.name = %q{georuby_c}
  s.version = "0.1.7"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Martin Kihlgren"]
  s.cert_chain = ["/Users/nofxx/.gem/gem-public_cert.pem"]
  s.date = %q{2008-08-14}
  s.description = %q{description of gem}
  s.email = ["FIXME email"]
  s.extra_rdoc_files = ["History.txt", "License.txt", "Manifest.txt", "README.txt", "website/index.txt"]
  s.files = ["History.txt", "License.txt", "Manifest.txt", "README.txt", "Rakefile", "config/hoe.rb", "config/requirements.rb", "examples/contains.rb", "examples/intersects.rb", "ext/Makefile", "ext/common.c", "ext/common.h", "ext/common.o", "ext/extconf.rb", "ext/geo.bundle", "ext/geo.c", "ext/geo.o", "ext/geo_set.c", "ext/geo_set.h", "ext/geo_set.o", "ext/intersection.c", "ext/intersection.h", "ext/intersection.o", "ext/line.c", "ext/line.h", "ext/line.o", "ext/line_set.c", "ext/line_set.h", "ext/line_set.o", "ext/point.c", "ext/point.h", "ext/point.o", "ext/point_set.c", "ext/point_set.h", "ext/point_set.o", "ext/triangle.c", "ext/triangle.h", "ext/triangle.o", "ext/triangle_set.c", "ext/triangle_set.h", "ext/triangle_set.o", "ext/types.h", "georuby_c.gemspec", "lib/georuby_c.rb", "lib/georuby_c/version.rb", "metadata", "script/console", "script/destroy", "script/generate", "script/txt2html", "setup.rb", "spec/georuby_c_spec.rb", "spec/spec.opts", "spec/spec_helper.rb", "tasks/deployment.rake", "tasks/environment.rake", "tasks/rspec.rake", "tasks/website.rake", "website/index.html", "website/index.txt", "website/javascripts/rounded_corners_lite.inc.js", "website/stylesheets/screen.css", "website/template.html.erb"]
  s.platform = Gem::Platform::RUBY
  s.extensions = ['ext/extconf.rb']
  s.has_rdoc = true
  s.homepage = %q{http://georuby_c.rubyforge.org}
  s.post_install_message = %q{

    For more information about this fork, see http://github.com/nofxx/georuby_c
    For more information on Geo, see http://geo.rubyforge.org

}
  s.rdoc_options = ["--main", "README.txt"]
  s.require_paths = ["lib", "ext"]
  s.rubyforge_project = %q{georuby_c}
  s.rubygems_version = %q{1.2.0}
  s.signing_key = %q{/Users/nofxx/.gem/gem-private_key.pem}
  s.summary = %q{description of gem}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 2

    if current_version >= 3 then
      s.add_development_dependency(%q<hoe>, [">= 1.7.0"])
    else
      s.add_dependency(%q<hoe>, [">= 1.7.0"])
    end
  else
    s.add_dependency(%q<hoe>, [">= 1.7.0"])
  end
end