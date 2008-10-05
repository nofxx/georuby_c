require File.expand_path(File.dirname(__FILE__) + '/../../spec_helper')


describe Point do
  describe "C extensions" do
    include Native
    before(:each) do
            @point1 = Native::Point.new(1,2)
            @point2 = Native::Point.new(-10.31,23.3131)
            @point3 = Native::Point.new(10.131,-77.131)
    end
    it "should create a native C point directly" do
      @point1.should be_instance_of(Native::Point)
    end

    it "shoud substract points" do
      res = @point1 -  @point2
      res.x.should be_close(11.31, 0.001)
      res.y.should be_close(-21.3131,0.001)
    end

  end

end
