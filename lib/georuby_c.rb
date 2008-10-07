$:.unshift(File.dirname(__FILE__)) unless
  $:.include?(File.dirname(__FILE__)) || $:.include?(File.expand_path(File.dirname(__FILE__)))

require 'georuby_c/base/helper'
require 'georuby_c/base/geometry'
require 'georuby_c/base/point'
require 'georuby_c/base/line_string'
require 'georuby_c/base/linear_ring'
require 'georuby_c/base/polygon'
require 'georuby_c/base/multi_point'
require 'georuby_c/base/multi_line_string'
require 'georuby_c/base/multi_polygon'
require 'georuby_c/base/geometry_collection'
require 'georuby_c/base/envelope'
require 'georuby_c/base/geometry_factory'
require 'georuby_c/base/ewkt_parser'
require 'georuby_c/base/georss_parser'
require 'georuby_c/shp4r/shp'

require 'ext/geo'

module GeorubyC
  module Native
    include Geo
  end
end
