project('hyper-block', 'cpp', version : '0.1.0',
  default_options : ['cpp_std=c++20'])

# Dependencies
boost = dependency('boost')
gtest = dependency('gtest')

dependencies = [
  boost,
]

src = [
  'src/server.cpp',
  'src/main.cpp',
]

executable('hyperblock-server', src, dependencies : dependencies)

# Test executable
test('test_all',
  executable('test_all',
    ['test_all.cpp'],
    dependencies: [gtest, boost],
  ),
  args : ['--gtest_color=yes'])
