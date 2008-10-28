def mock_point(stubs={})
  stubs = { :x => 10.100, :y => -20.200 }.merge!(stubs)
  @mock_point ||= mock(Point, stubs)
end
