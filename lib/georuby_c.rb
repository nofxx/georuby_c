$:.unshift(File.dirname(__FILE__)) unless
  $:.include?(File.dirname(__FILE__)) || $:.include?(File.expand_path(File.dirname(__FILE__)))

require 'ext/geo'
require 'georuby_c/base/helper'
require 'georuby_c/base/geometry'
require 'georuby_c/base/point'

module GeorubyC
  module Native
    include Geo
  end
end