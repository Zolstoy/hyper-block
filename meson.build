project('hyper-block', 'cpp', version : '0.1.0', meson_version : '>=1.0.0')

if host_machine.system() == 'windows'
 override_args = ['cpp_std=vc++latest']
 common_cpp_args =  ['-D_WIN32_WINNT=0x0601', '/bigobj', '-DBOOST_PROGRAM_OPTIONS_DYN_LINK=1']
else
 override_args = ['cpp_std=c++23']
 common_cpp_args = []
endif

boost_headers = dependency('boost')
openssl = dependency('openssl')
cereal = dependency('cereal')
spdlog = dependency('spdlog', default_options: ['default_library=static'])

dependencies = [
  boost_headers,
  openssl,
  cereal,
  spdlog
]

boost_program_options = dependency('boost', modules : ['program_options'])
gtest = dependency('gtest', default_options: ['default_library=static'])
gtest_main = dependency('gtest_main', default_options: ['default_library=static'])

src = [
  'src/protocol.cpp',
  'src/instance.cpp',
  'src/session.cpp',
]

server = executable('hyperblock-server',
  src + ['src/bin/main.cpp'],
  dependencies : dependencies + [boost_program_options],
  include_directories : include_directories('src'),
  cpp_args : common_cpp_args,
  override_options: override_args,
  install: true
)

tests_details = executable('tests-details',
  src + ['src/tests/details.cpp', 'src/tests/common.cpp'],
  dependencies: dependencies + [gtest, spdlog],
  include_directories: include_directories('src'),
  cpp_args: common_cpp_args + ['-DHYPERBLOCK_STEPBYSTEP'],
  override_options: override_args,
  install: true
)
  
test('details', tests_details, args : ['--gtest_color=yes'])

tests_siege = executable('tests-siege',
  src + ['src/tests/siege.cpp', 'src/tests/common.cpp'],
  dependencies: dependencies + [gtest, gtest_main, spdlog],
  include_directories: include_directories('src'),
  cpp_args: common_cpp_args,
  override_options: override_args,
  install: true
)

test('siege', tests_siege, args : ['--gtest_color=yes'])
