project('hyper-block', 'cpp', version : '0.1.0',
  default_options : ['cpp_std=c++20'])

# Compiler options
cpp_args = []

if host_machine.system() == 'windows'
    cpp_args += ['-DWIN32_LEAN_AND_MEAN', '-D_WIN32_WINNT=0x0601']
endif

# Dependencies
boost = dependency('boost', modules : [
  'program_options',
])

gtest = dependency('gtest')
openssl = dependency('openssl')
cereal = dependency('cereal')

dependencies = [
  boost,
  openssl,
  cereal
]

src = [
  'src/server.cpp',
  'src/client.cpp',
  'src/player.cpp',
]

executable('hyperblock-server', src + 'src/main.cpp', dependencies : dependencies, cpp_args : cpp_args)

# Test executable
test('test_all',
  executable('test_all',
    ['test_all.cpp'] + src,
    dependencies: [gtest] + dependencies,
    include_directories: include_directories('src'),
  ),
  args : ['--gtest_color=yes'])
