# geo - a fast 2D geometry library for ruby
# Copyright (C) 2007  Martin Kihlgren <zond at troja dot ath dot cx>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

require 'rubygems'
require 'geo'
require 'benchmark'

m = 10000
puts "creating LineSet with #{m} lines"
STDOUT.flush
ls = Geo::LineSet.new
m.times do |n|
  l = Geo::Line.new(rand(1000), rand(1000), 1, 1)
  l.abs = rand(20)
  l.angle = rand * 2 * Math::PI
  ls << Geo::Line.new(rand(1000), rand(1000), rand(1000), rand(1000))
end
print "reindexing LineSet..."
STDOUT.flush
ls._reindex
puts "done!"
l = Geo::Line.new(rand(1000), rand(1000), rand(1000), rand(1000))
Benchmark.benchmark("LineSet#intersects?(#{l.inspect})") do |bm|
  bm.report do
    ls.intersects?(l)
  end
end


