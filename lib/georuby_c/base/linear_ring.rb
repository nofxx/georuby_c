require 'georuby_c/base/line_string'

module GeorubyC
  module Base
    #Represents a linear ring, which is a closed line string (see LineString). Currently, no check is performed to verify if the linear ring is really closed.
    class LinearRing < LineString
      def initialize(srid= @@srid,with_z=false,with_m=false)
        super(srid,with_z,with_m)
      end
    end
  end
end
