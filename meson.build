project('hyper-block', 'cpp', version : '0.1.0',
  default_options : ['cpp_std=c++20'])

# Compiler options
common_cpp_args = []

if host_machine.system() == 'windows'
    common_cpp_args += ['-DWIN32_LEAN_AND_MEAN', '-D_WIN32_WINNT=0x0601']
endif

# Dependencies
boost_headers = dependency('boost')

boost_program_options = dependency('boost', modules : [
  'program_options',
])

gtest = dependency('gtest')
gtest_main = dependency('gtest_main')
openssl = dependency('openssl')
cereal = dependency('cereal')

lib_src = [
  'src/session.cpp',
  'src/player.cpp',
]

lib = library('gameserver', lib_src,
  include_directories : include_directories('include/gameserver'),
  dependencies : [boost_headers, openssl, cereal],
  cpp_args : common_cpp_args,
)

executable('hyperblock-server', 'hyperblock_server_main.cpp',
  link_with: lib, cpp_args : common_cpp_args,
  dependencies : [boost_program_options, boost_headers, openssl],
  include_directories : include_directories('include'))

# Test executable
test('test_lib',
  executable('test_lib',
    ['test_lib_main.cpp'],
    dependencies: [gtest, gtest_main, boost_headers, openssl],
    include_directories: include_directories('include'),
    cpp_args: common_cpp_args,
  ),
  args : ['--gtest_color=yes'])
