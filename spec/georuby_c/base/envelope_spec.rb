require File.expand_path(File.dirname(__FILE__) + '/../../spec_helper')

describe Envelope do
  before(:each) do
    @env = Envelope.from_points([Point.from_x_y(10,20),Point.from_x_y(20,30)])
  end

  it "should initialize" do
    @env.should be_instance_of(Envelope)
  end


#  it "should have a lower corner" do
#    @env.upper_corner.x.should eql('')
#  end


  it "should have a center" do
    @env.center.x.should eql(15)
    @env.center.y.should eql(25)
  end

  it "should print a kml_representation" do
    @env.as_kml.should eql("<LatLonAltBox>\n<north>30</north>\n<south>20</south>\n<east>20</east>\n<west>10</west>\n</LatLonAltBox>\n")
  end

end
