load('//:buckaroo_macros.bzl', 'buckaroo_deps_from_package')
load('//:subdir_glob.bzl', 'subdir_glob')

parserutils = \
  buckaroo_deps_from_package('github.com/buckaroo-pm/netsurf-parserutils')

wapcaplet = \
  buckaroo_deps_from_package('github.com/buckaroo-pm/netsurf-wapcaplet')

prebuilt_cxx_library(
  name = 'internal-headers',
  header_namespace = '',
  header_only = True,
  exported_headers = subdir_glob([
    ('src', '**/*.h'),
  ]),
  visibility = [
    'PUBLIC',
  ],
)

cxx_library(
  name = 'dom',
  header_namespace = '',
  exported_headers = subdir_glob([
    ('include', '**/*.h'),
  ]),
  headers = subdir_glob([
    ('src', '**/*.h'),
  ]),
  srcs = glob([
    'src/**/*.c',
  ]),
  deps = parserutils + wapcaplet,
  visibility = [
    'PUBLIC',
  ],
)
