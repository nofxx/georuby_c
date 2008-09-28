require File.expand_path(File.dirname(__FILE__) + '/spec_helper')


# Time to add your specs!
# http://rspec.info/
describe "Place your specs here" do
  
  it "should instantiate Geometry" do
    @geo = Geometry.new
    @geo.class.should eql(Geometry)
  end
  
  it "should instantiate Point" do
    @point = Point.new
    @point.class.should eql(Point)
  end
  
  it "should instantiate Line" do
    @point = LineString.new
    @point.class.should eql(LineString)
  end
  
end
