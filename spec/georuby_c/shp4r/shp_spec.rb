require File.expand_path(File.dirname(__FILE__) + '/../../spec_helper')

include GeorubyC::Shp4r
include GeorubyC::Base

describe Shp4r do

  describe "Point" do
    before(:each) do
      @shpfile = ShpFile.open(File.dirname(__FILE__) + '/../../data/point.shp')
    end

    it "should parse ok" do
      @shpfile.record_count.should eql(2)
      @shpfile.should have(1).fields
      @shpfile.shp_type.should eql(ShpType::POINT)
    end

    it "should parse fields" do
      field = @shpfile.fields.first
      field.name.should eql("Hoyoyo")
      field.type.should eql("N")
    end

    it "should parse record 1" do
      rec = @shpfile.first
      rec.geometry.should be_kind_of Point
      rec.geometry.x.should be_close(-90.08375, 0.00001)
      rec.geometry.y.should be_close(34.39996, 0.00001)
      rec.data["Hoyoyo"].should eql(6)
    end

    it "should parse record 2" do
      rec = @shpfile[1]
      rec.geometry.should be_kind_of Point
      rec.geometry.x.should be_close(-87.82580, 0.00001)
      rec.geometry.y.should be_close(33.36416, 0.00001)
      rec.data["Hoyoyo"].should eql(9)
    end

  end

  describe "Polyline" do
    before(:each) do
      @shpfile = ShpFile.open(File.dirname(__FILE__) + '/../../data/polyline.shp')
    end

    it "should parse ok" do
      @shpfile.record_count.should eql(1)
      @shpfile.should have(1).fields
      @shpfile.shp_type.should eql(ShpType::POLYLINE)
    end

    it "should parse fields" do
      field = @shpfile.fields.first
      field.name.should eql("Chipoto")
      field.type.should eql("F")
    end

    it "should parse record 1" do
      rec = @shpfile.first
      rec.geometry.should be_kind_of MultiLineString
      rec.geometry.length.should eql(1)
      rec.geometry[0].length.should eql(6)
      rec.data["Chipoto"].should eql(5.678)
    end

  end

  describe "Polygon" do
    before(:each) do
      @shpfile = ShpFile.open(File.dirname(__FILE__) + '/../../data/polygon.shp')
    end

    it "should parse ok" do
      @shpfile.record_count.should eql(1)
      @shpfile.should have(1).fields
      @shpfile.shp_type.should eql(ShpType::POLYGON)
    end

    it "should parse fields" do
      field = @shpfile.fields.first
      field.name.should eql("Hello")
      field.type.should eql("C")
    end

    it "should parse record 1" do
      rec = @shpfile.first
      rec.geometry.should be_kind_of MultiPolygon
      rec.geometry.length.should eql(1)
      rec.geometry[0].length.should eql(1)
      rec.geometry[0][0].length.should eql(7)
      rec.data["Hello"].should eql("Bouyoul!")
    end
  end
end
