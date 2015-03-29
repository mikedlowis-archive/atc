#!/usr/bin/ruby

require_relative 'modules/build-system/setup'

#------------------------------------------------------------------------------
# Environment Definitions
#------------------------------------------------------------------------------
# Define the default compiler environment
base_env = BuildEnv.new do |env|
  # Move the object files to the build dir
  env.build_dir('source','build/obj/source')
end

#------------------------------------------------------------------------------
# Release Build Targets
#------------------------------------------------------------------------------
base_env.Library('build/libatc.a', Dir['source/runtime/*.c'])

#------------------------------------------------------------------------------
# Test Build Targets
#------------------------------------------------------------------------------
#if Opts[:profile].include? "test"
#  compiler_libs = ['build/lib/libparse-test.a'] + runtime_libs
#  test_env.Library('build/lib/libparse-test.a', FileList['source/libparse/*.c'])
#  test_env.Program('build/bin/sclpl-test', FileList['source/sclpl/*.c'] + compiler_libs)
#  test_env.Command('RSpec', [], 'CMD' => [
#      'rspec', '--pattern', 'spec/**{,/*/**}/*_spec.rb', '--format', 'documentation'])
#end
