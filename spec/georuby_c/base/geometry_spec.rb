require File.expand_path(File.dirname(__FILE__) + '/../../spec_helper')

describe Geometry do  
  before(:each) do
    @geo = Geometry.new
  end
  
  it "should instantiate" do
    violated unless @geo
  end
  
  
  it "should output as_ewkb" do
    @geo.stub!(:binary_geometry_type).and_return(1)
    @geo.stub!(:binary_representation).and_return(1)    
    @geo.as_ewkb.should eql("\001\001\000\000 \346\020\000\000\001")
  end
end