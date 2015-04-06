#!/usr/bin/env ruby

require_relative 'modules/build-system/setup'

#------------------------------------------------------------------------------
# Environment Definitions
#------------------------------------------------------------------------------
# Define the default compiler environment
base_env = BuildEnv.new do |env|
  # Move the object files to the build dir
  env.build_dir('source',  'build/obj/source')
  env.build_dir('tests',   'build/obj/tests')
  env.build_dir('modules', 'build/obj/modules')
  # Setup include paths
  env['CPPPATH'] += Dir['source/**/', 'modules/atf/source/']
  # Turn on all warnings and treat them as errors, Also C99 strict mode
  env['CFLAGS'] += ['--std=c99', '-pedantic', '-Wall', '-Wextra', '-Werror']
  # Enable debug symbols for test
  if Opts[:profile].include? "test"
    env['CFLAGS'] += ['-g', '-O0']
  else
    env['CFLAGS'] += ['-O3']
  end
  # Enable profiling info
  if Opts[:profile].include? "profile"
    env['CFLAGS'] += ['-pg']
    env['LDFLAGS'] += ['-pg']
  end

  # Enable coverage info
  if Opts[:profile].include? "coverage"
    env['CFLAGS'] += ['--coverage']
    env['LDFLAGS'] += ['--coverage']
  end
end

#------------------------------------------------------------------------------
# Release Build Targets
#------------------------------------------------------------------------------
base_env.Library('build/libatc.a', Dir['source/runtime/*.c'])

#------------------------------------------------------------------------------
# Test Build Targets
#------------------------------------------------------------------------------
if Opts[:profile].include? "test"
    base_env.Program('build/test_atc.exe',
                     Dir['tests/*.c', 'modules/atf/source/*.c'] + ['build/libatc.a'])
    base_env.Command('Unit Tests', [], 'CMD' => ['build/test_atc.exe'])
end
