begin
  require 'spec'
rescue LoadError
  require 'rubygems'
  gem 'rspec'
  require 'spec'
end

$:.unshift(File.dirname(__FILE__) + '/../lib')
require 'georuby_c'
include GeorubyC
include Base


# module Spec  
#   module Example
#     class NonStandardError < Exception; end
#   end
# 
#   module Matchers
#     def fail
#       raise_error(Spec::Expectations::ExpectationNotMetError)
#     end
# 
#     def fail_with(message)
#       raise_error(Spec::Expectations::ExpectationNotMetError, message)
#     end
# 
#     def exception_from(&block)
#       exception = nil
#       begin
#         yield
#       rescue StandardError => e
#         exception = e
#       end
#       exception
#     end
#   end
# end
# 
# 
# share_as :SandboxedOptions do
#   attr_reader :options
# 
#   before(:each) do
#     @original_rspec_options = ::Spec::Runner.options
#     ::Spec::Runner.use(@options = ::Spec::Runner::Options.new(StringIO.new, StringIO.new))
#   end
# 
#   after(:each) do
#     ::Spec::Runner.use(@original_rspec_options)
#   end
# 
#   def run_with(options)
#     ::Spec::Runner::CommandLine.run(options)
#   end
# end unless Object.const_defined?(:SandboxedOptions)