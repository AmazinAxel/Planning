project(
  'planning',
  'cpp',
  version: '1.0.0',
  meson_version: '>= 1.8.0',
  default_options: [ 'cpp_std=c++17' ]
)

gtkmm = dependency('gtkmm-4.0')
json = dependency('nlohmann_json', fallback: ['nlohmann_json', 'nlohmann_json_dep'])
giomm = dependency('giomm-2.68')
glib = dependency('glib-2.0')

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
  dependencies: [ gtkmm, json, giomm, glib ],
  android_exe_type: 'application',
  install: true,
  cpp_args: [ '-O2' ]
)
