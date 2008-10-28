begin
  require 'spec'
rescue LoadError
  require 'rubygems'
  gem 'rspec'
  require 'spec'
end

$:.unshift(File.dirname(__FILE__) + '/../lib')
require 'georuby_c'
require File.expand_path(File.dirname(__FILE__) + "/famous_mocks")
include GeorubyC
include Base
