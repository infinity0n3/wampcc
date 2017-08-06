find_package(PkgConfig)
pkg_check_modules(PC_JANSSON jansson>=2.7)
set(JANSSON_DEFINITIONS ${PC_JANSSON_CFLAGS_OTHER})

find_path(JANSSON_INCLUDE_DIR jansson.h
          HINTS ${PC_JANSSON_INCLUDEDIR} ${PC_JANSSON_INCLUDE_DIRS}
          PATH_SUFFIXES JANSSON )

find_library(JANSSON_LIBRARY NAMES jansson libjansson
             HINTS ${PC_JANSSON_LIBDIR} ${PC_JANSSON_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set JANSSON_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libjansson DEFAULT_MSG
                                  JANSSON_LIBRARY JANSSON_INCLUDE_DIR)

mark_as_advanced(JANSSON_INCLUDE_DIR JANSSON_LIBRARY )

set(JANSSON_LIBRARIES ${JANSSON_LIBRARY} )
set(JANSSON_INCLUDE_DIRS ${JANSSON_INCLUDE_DIR} )

if(LIBJANSSON_FOUND)
set(HAVE_JANSSON ON )
#~ message(STATUS "jansson version: ${PC_JANSSON_VERSION}")
#~ message(STATUS "jansson include: ${JANSSON_INCLUDE_DIR}")
endif()
