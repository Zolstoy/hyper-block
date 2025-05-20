project('hyper-block', 'cpp', version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

# Dependencies
boost = dependency('boost', version : '>=1.70.0')
gtest = dependency('gtest', version : '>=1.10.0', required : false)

dependencies = [
  boost,
  gtest,
]

# Compiler options
cpp_args = [
  '-Wall',
  '-Wextra',
]

src = [
  'src/server.cpp',
  'src/main.cpp',
]

executable('hyperblock-server', src, dependencies : [boost, gtest],
  cpp_args : cpp_args,
  include_directories : include_directories('include'),
  install : true)

# Test executable
test('test_all',
  executable('test_all',
    ['tests/test_all.cpp'],
    dependencies: [bnetcpp_dep, tests, network],
    cpp_args: tests_cpp_args + cpp_args,
    include_directories: include_directories(['include']),
    override_options: override
  ),
  args : ['--gtest_color=yes'])
