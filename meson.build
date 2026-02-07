project(
  'planning',
  'cpp',
  version: '1.0.0',
  default_options: [ 'cpp_std=c++17' ]
)

gtk4_dep = dependency('gtk4')
gtkmm_dep = dependency('gtkmm-4.0') # c++ bindings for gtk

#server = get_option('server')

sources = files(
  './src/services/backup.cpp',
  './src/main.cpp',
  './src/planListView.cpp',
  './src/plans.cpp',
)

executable(
  'planning',
  sources,
  dependencies: [ gtk4_dep, gtkmm_dep ],
  cpp_args: [
    '-O2',
    #'-DSERVER="' + server + '"'
  ]
)
