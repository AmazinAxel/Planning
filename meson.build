project(
  'planning',
  'cpp',
  version: '1.0.0',
  default_options: [ 'cpp_std=c++17', 'optimization=2' ]
)

gtk4_dep = dependency('gtk4')

executable(
  'planning',
  ['src/main.cpp'],
  dependencies: [ gtk4_dep ],
)
