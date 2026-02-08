project(
  'planning',
  'cpp',
  version: '1.0.0',
  default_options: [ 'cpp_std=c++17' ]
)

gtkmm_dep = dependency('gtkmm-4.0') # c++ for gtk

#server = get_option('server')

sources = files(
  './src/functionality/lists/make.cpp',
  './src/functionality/lists/remove.cpp',
  './src/functionality/plans/make.cpp',
  './src/functionality/plans/remove.cpp',
  './src/main.cpp',
  './src/planList.cpp',
  './src/planPage.cpp',
)

executable(
  'planning',
  sources,
  dependencies: [ gtkmm_dep ],
  cpp_args: [
    '-O2',
    #'-DSERVER="' + server + '"'
  ]
)
