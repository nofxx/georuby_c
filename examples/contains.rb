#!/usr/bin/env ruby
#
# Workaround to get a "contains". objective here is get this native in C
# I will set 8 lines ("walkers"), starting from the point, each one going a cadinal way,
# If any hit the main line, we are contained! =D
# It works, looks fast .. any suggestions/help welcome

# CONSTANTS
#
PI = 3.14159265358979
R = 6371
W = 0.5

# DEG 2 RAD
#
def deg2rad(d);  (d/180.0) * PI;  end
def rad2deg(r);  (r*180.0) / PI;  end

# Haversine formula
def distance(x1, y1, x2, y2)
  dlat = deg2rad(x2-x1)/2
  dlon = deg2rad(y2-y1)/2
  a = (sin(dlat) ** 2) + cos(deg2rad(x1)) * cos(deg2rad(x2)) * (sin(dlon) ** 2)
  b = 2 * atan2(sqrt(a), sqrt(1-a))
  c = R * b
  return c
end


xPoint = Struct.new("Point", :x, :y)
def walker(p, ang)
  lat,lon = p[:x], p[:y]
  lat,lon,ang = deg2rad(lat), deg2rad(lon), deg2rad(ang)
  d = deg2rad(W/R.to_f)
  x = asin(sin(lat)*cos(d)+cos(lat)*sin(d)*cos(ang))
  y = lon+atan2(sin(ang)*sin(d)*cos(lat), cos(d)-sin(lat)*sin(x))
  y = (y+PI) % (2*PI) - PI
  x = rad2deg(x)
  y = rad2deg(y)
  puts x.to_s + " - " + y.to_s
  #Geo::Point.new(x,y)
  { :x => x, :y => y }
end


def hit
  r1 = Geo::Line.new(14.0,14.0,15.0,15.0)  
 # r2 = Geo::Line.new(15,15,20,20)
  rota = Geo::LineSet.new
  rota << r1
  #rota << r2
  #rota._reindex
  g = 0
  #gps = Geo::Point.new(14.45, 14.45)
  gps = { :x => 14.47644, :y => 14.47645 }
  catch :hit do
    while(g<360) do      
      walk = xwalker(gps, g)#XX, YY, g)
      walker = Geo::Line.new(gps[:x], gps[:y], walk[:x], walk[:y])
      i = rota.intersects?(walker)
      puts "#{g}º -> " + i.to_s
      #throw :hit if i
      g+=45
    end
  end
end

puts "result =>> " + hit.to_s
puts "-----------------------------------"
#puts walkers

# #
# BENCHMARK
#
