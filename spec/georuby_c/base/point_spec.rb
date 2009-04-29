# -*- coding: utf-8 -*-
require File.expand_path(File.dirname(__FILE__) + '/../../spec_helper')

describe Point do
  before(:each) do
    @point = Point.new
  end

  it "should instantiatember" do
    violated unless @point
  end

  it "should have binary_geometry_type 2" do
    @point.binary_geometry_type.should eql(1)
  end

  describe "> Instantiation" do

    it "should instantiate a 2d point" do
      point = Point.from_x_y(10,20)
      point.should be_instance_of(Point)
    end

    it "should instantiate a 3d point" do
      point = Point.from_x_y_z(10,20,30)
      point.should be_instance_of(Point)
    end

    it "should instantiate a 4d point" do
      point = Point.from_x_y_z_m(10,20,30,40)
      point.m.should eql(40)
    end

    it "should instantiate a point from positive degrees" do
      point = Point.from_x_y_xl_yl('04720.0609','E','2250.7735','N')
      point.y.should be_close(22.846225, 0.000001)
      point.x.should be_close(47.3343483333333, 0.0000001)
    end

    it "should instantiate a point from negative degrees" do
      point = Point.from_x_y_xl_yl('04720.0609','W','2250.7735','S')
      point.y.should be_close(-22.846225, 0.000001)
      point.x.should be_close(-47.3343483333333, 0.0000001)
    end

    it "should instantiate a point from polar coordinates" do
      point = Point.from_r_t(1.4142,45)
      point.y.should be_close(1, 0.00001)
      point.x.should be_close(1, 0.00001)
    end

    it "should parse lat long" do
      Point.from_latlong("-20° 47' 26.37","-20° 47' 26.37").x.should be_close(-20.790658, 0.00001)
      Point.from_latlong("20° 47' 26.378","20° 47' 26.378").y.should be_close(20.790658, 0.00001)
    end

    it "should parse lat long w/o sec" do
      Point.from_latlong("-20°47′26″","-20°47′26″").x.should be_close(-20.790555, 0.00001)
      Point.from_latlong("20°47′26″","20°47′26″").y.should be_close(20.790555, 0.00001)
    end

    it "should accept with W or S notation" do
      Point.from_latlong("20° 47' 26.37 W","20° 47' 26.37 S").x.should be_close(-20.790658, 0.00001)
      Point.from_latlong("20° 47' 26.37 W","20° 47' 26.37 S").y.should be_close(-20.790658, 0.00001)
    end

    it "should print out nicely" do
      Point.from_x_y(47.88, -20.1).as_latlong.should eql("47°52′48″, -20°06′00″")
    end

    it "should print out nicely" do
      Point.from_x_y(-20.78, 20.78).as_latlong(:full => true).should eql("-20°46′48.00″, 20°46′48.00″")
    end

    it "should print out nicely" do
      Point.from_x_y(47.11, -20.2).as_latlong(:full => true).should eql("47°06′36.00″, -20°11′60.00″")
    end

    it "should print out nicely" do
      Point.from_x_y(47.11, -20.2).as_latlong(:coord => true).should eql("47°06′36″N, 20°11′60″W")
    end

    it "should print out nicely" do
      Point.from_x_y(-47.11, 20.2).as_latlong(:full => true,:coord => true).should eql("47°06′36.00″S, 20°11′60.00″E")
    end

  end

  describe " > Distance" do

    before(:each) do
      @p1 = Point.from_x_y(1,1)
      @p2 = Point.from_x_y(2,2)
    end

    it "and a 3th grade child should calculate euclidian distance" do
      @p1.euclidian_distance(@p2).
        should be_close(1.4142135623731, 0.00000001)
    end

    it "should calculate spherical distance" do
      @p1.spherical_distance(@p2).
        should be_close(157225.358003181,0.00000001)
    end

    it "should calculate ellipsoidal distance" do
      @p1.ellipsoidal_distance(@p2).
        should be_close(156876.149400742, 0.00000001)
    end

  end

  describe "> Export Formats" do

     before(:each) do
       @point = Point.from_x_y( -11.2431, 32.3141 )
     end

     it "should print nicely" do
       @point.text_representation.should eql("-11.2431 32.3141")
     end

     it "should have a nice bounding box" do
       @point.should have(2).bounding_box
       @point.bounding_box.each do |point|
         point.x.should eql(@point.x)
         point.y.should eql(@point.y)
       end
     end

     it "should print as kml too" do
       @point.kml_representation.should eql("<Point>\n<coordinates>-11.2431,32.3141</coordinates>\n</Point>\n")
     end

     it "should print as georss" do
       @point.georss_simple_representation('hey').should eql("<georss:point>32.3141 -11.2431</georss:point>\n")
     end

     it "should print r (polar coords)" do
       @point.r.should be_close(34.214154, 0.00001)
     end

     it "should print theta as degrees" do
       @point.theta_deg.should be_close(289.184406352127, 0.0001)
     end

     it "should print theta as radians" do
       @point.theta_rad.should be_close(5.04722003626982, 0.0001)
     end

     it "should output as polar" do
       @point.as_polar.should be_instance_of(Array)
       @point.should have(2).as_polar #.length.should eql(2)
     end

   end

end
