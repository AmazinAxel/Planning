project(
  'planning',
  'cpp',
  version: '1.0.0',
  default_options: [ 'cpp_std=c++17' ]
)

gtkmm_dep = dependency('gtkmm-4.0') # c++ for gtk
json = dependency('nlohmann_json') # json
giomm = dependency('giomm-2.68') # Gio

sources = files(
  './src/functionality/lists/lists.cpp',
  './src/functionality/plans/plans.cpp',
  './src/functionality/json.cpp',
  './src/functionality/sync.cpp',
  './src/main.cpp',
  './src/planList.cpp',
  './src/planPage.cpp',
)

executable(
  'planning',
  sources,
  dependencies: [ gtkmm_dep, json, giomm ],
  cpp_args: [ '-O2' ]
)
