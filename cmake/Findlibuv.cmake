find_package(PkgConfig)
pkg_check_modules(PC_LIBUV libuv)
set(LIBUV_DEFINITIONS ${PC_LIBUV_CFLAGS_OTHER})

find_path(LIBUV_INCLUDE_DIR uv.h
          HINTS ${PC_LIBUV_INCLUDEDIR} ${PC_LIBUV_INCLUDE_DIRS}
          PATH_SUFFIXES LIBUV )

find_library(LIBUV_LIBRARY NAMES uv libuv
             HINTS ${PC_LIBUV_LIBDIR} ${PC_LIBUV_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBUV_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libuv DEFAULT_MSG
                                  LIBUV_LIBRARY LIBUV_INCLUDE_DIR)

mark_as_advanced(LIBUV_INCLUDE_DIR LIBUV_LIBRARY )

set(LIBUV_LIBRARIES ${LIBUV_LIBRARY} )
set(LIBUV_INCLUDE_DIRS ${LIBUV_INCLUDE_DIR} )

if(LIBUV_FOUND)
  set(HAVE_LIBUV ON )
  #~ message(STATUS "libuv version: ${PC_LIBUV_VERSION}")
  #~ message(STATUS "libuv include: ${LIBUV_INCLUDE_DIR}")
endif()
