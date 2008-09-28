require File.expand_path(File.dirname(__FILE__) + '/../../spec_helper')

describe LineString do  
  before(:each) do
    @line = LineString.new
  end
  
  it "should instantiate" do
    violated unless @line
  end
  
  
  
  
end