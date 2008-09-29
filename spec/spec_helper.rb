lib_path = File.expand_path("#{File.dirname(__FILE__)}/../../lib")
$LOAD_PATH.unshift lib_path unless $LOAD_PATH.include?(lib_path)

begin
  require 'spec'
rescue LoadError
  require 'rubygems'
  gem 'rspec'
  require 'spec'
end

require 'georuby_c'
include GeorubyC
include Base

Spec::Runner.configure do |config|

  
  
end