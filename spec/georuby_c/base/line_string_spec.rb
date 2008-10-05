require File.expand_path(File.dirname(__FILE__) + '/../../spec_helper')

module LineSpecHelper
  def mock_points
#    @point = mock(Point, :x => 1.0, :y => 2.0)
    Array.new(7) { |i| mock_point(i,i) }
  end

  def mock_point(x=1,y=2)
    mock(Point, :x => x, :y => y, :text_representation => "#{x} #{y}")
  end
end

describe LineString do
  include LineSpecHelper
  before(:each) do
    @line = LineString.from_points([mock(Point)])
  end

  it "should instantiate" do
    violated unless @line
  end

  it "should have binary_geometry_type 2" do
    @line.binary_geometry_type.should eql(2)
  end

  it "should have text_geometry_type" do
    @line.text_geometry_type.should eql("LINESTRING")
  end

  it "should have a points array" do
    @line.points.should be_instance_of(Array)
  end

  describe ", from coordinates" do
    before(:each) do
      Point.should_receive(:from_coordinates).
        exactly(4).with(anything, 4326, false, false).and_return(mock_point)
      @line = LineString.from_coordinates([1.2,2.5,2.2,4.5])
    end

    it "should instantiate from coordinates" do
      @line.points.length.should eql(4)
    end
  end

  describe ", Instantiated" do

    before(:each) do
      @line = LineString.from_points(mock_points)
    end

    it "should be closed if the last point equals the first" do
      @line.push(@line.first)
      @line.should be_closed
      @line.length.should eql(8)
    end

    it "should print the text representation" do
      @line.text_representation.should eql("0 0,1 1,2 2,3 3,4 4,5 5,6 6")
    end

    it "should print the georss_simple_representation" do
      @line.georss_simple_representation({:geom_attr => nil}).
        should eql("<georss:line>0 0 1 1 2 2 3 3 4 4 5 5 6 6</georss:line>\n")
    end

    it "should map the georss_poslist" do
      @line.georss_poslist.should eql("0 0 1 1 2 2 3 3 4 4 5 5 6 6")
    end

    it "should print the kml_representation" do
      @line.kml_representation.should
        eql("<LineString>\n<coordinates>0,0 1,1 2,2 3,3 4,4 5,5 6,6</coordinates>\n</LineString>\n")
    end

    it "should print the kml_poslist without reverse or z" do
      @line.kml_poslist({}).should eql("0,0 1,1 2,2 3,3 4,4 5,5 6,6")
    end

    it "should print the kml_poslist reverse" do
      @line.kml_poslist({:reverse => true}).should eql("6,6 5,5 4,4 3,3 2,2 1,1 0,0")
    end
  end
  
  require File.dirname(__FILE__) + '/../spec_helper'

  describe "controller" do
    it "should print the lenght in meters" do
      pending
    end
  end
end
