cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

project(wampcc)

# Version number.
set (WAMPCC_VERSION_MAJOR 1)
set (WAMPCC_VERSION_MINOR 4)
set (WAMPCC_VERSION "${WAMPCC_VERSION_MAJOR}.${WAMPCC_VERSION_MINOR}")
# Include extra cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

##
## Build options
##
option (BUILD_SHARED_LIBS "Use shared libraries" ON)
option (BUILD_STATIC_LIBS "Use static libraries" ON)
option (BUILD_EXAMPLES    "Build example apps"   ON)
option (BUILD_UTILS       "Build utility apps"   ON)
option (BUILD_TESTS       "Build test apps"      OFF)
set(LIBUV_DIR   "" CACHE STRING "libuv installation directory")
set(JANSSON_DIR "" CACHE STRING "Jansson installation directory")

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH  "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

##
## Platform checks, and generate config.h
##
include(PlatformCheck)
include(CheckLibraryExists)
find_package(PkgConfig)

##
## Try to find libssl
##
pkg_check_modules(SSL openssl REQUIRED)
if(SSL_FOUND)
  set(HAVE_SSL ON)
endif()
  
##
## Try to find libuv
##

if("${LIBUV_DIR}" STREQUAL "")
  pkg_check_modules(LIBUV libuv>=1.8.0 REQUIRED)
  if(LIBUV_FOUND)
    set(HAVE_LIBUV ON)
  endif()
else() # LIBUV_DIR
  if(CMAKE_HOST_UNIX)
      set(LIBUV_FILE "uv")
  else()
      set(LIBUV_FILE "libuv")
  endif()
  
  check_library_exists(${LIBUV_FILE} uv_version ${LIBUV_DIR}/lib HAVE_LIBUV)
  
  if(NOT HAVE_LIBUV)
    message(FATAL_ERROR "libuv not found")
  endif()
  
  link_directories(${INSTALL_LIB_DIR} ${LIBUV_DIR}/lib)
endif() # LIBUV_DIR

##
## Try to find jansson
##

if("${JANSSON_DIR}" STREQUAL "")
  pkg_check_modules(JANSSON jansson>=2.7 REQUIRED)
  if(JANSSON_FOUND)
    set(HAVE_JANSSON ON)
  endif()
else() # JANSSON_DIR
  if(CMAKE_HOST_UNIX)
      set(JANSSON_FILE "jansson")
  else()
      set(JANSSON_FILE "libjansson")
  endif()
  
  check_library_exists(${JANSSON_FILE} json_object ${JANSSON_DIR}/lib HAVE_JANSSON)
  
  if(NOT HAVE_JANSSON)
    message(FATAL_ERROR "jansson not found")
  endif()
  
  if(NOT "${LIBUV_DIR}" STREQUAL "")
    link_directories(${INSTALL_LIB_DIR} ${LIBUV_DIR}/lib ${JANSSON_DIR}/lib)
  else()
    link_directories(${INSTALL_LIB_DIR} ${JANSSON_DIR}/lib)
  endif()
endif() # JANSSON_DIR

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/config.h.incmake"
  "${PROJECT_BINARY_DIR}/config.h"
  )

# pkg-config
configure_file( 
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/wampcc.pc.incmake"
  "${CMAKE_CURRENT_BINARY_DIR}/wampcc.pc" @ONLY)
configure_file( 
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/wampcc_json.pc.incmake"
  "${CMAKE_CURRENT_BINARY_DIR}/wampcc_json.pc" @ONLY)


# check websocketpp is available in the source tree
set(websocketpp "${PROJECT_SOURCE_DIR}/3rdparty/websocketpp/websocketpp/message_buffer/message.hpp")
message(STATUS "checking for ${websocketpp}")
if(NOT EXISTS "${websocketpp}")
  message(FATAL_ERROR "websocketpp header not found! Check that websocketpp is unzipped under 3rdparty/")
endif()

# check msgpack is available in the source tree
set(msgpackfile "${PROJECT_SOURCE_DIR}/3rdparty/msgpack-c/include/msgpack.h")
message(STATUS "checking for ${msgpackfile}")
if(NOT EXISTS "${msgpackfile}")
  message(FATAL_ERROR "msgpack header not found! Check that msgpack is unzipped under 3rdparty/")
endif()


# add the binary tree to the search path for include files
# so that we will find config.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/websocketpp")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty")

# include sub directories
add_subdirectory(library/json)
add_subdirectory(library/wampcc)
add_subdirectory(utility)
add_subdirectory(tests)
add_subdirectory(examples)

install(FILES ${PROJECT_BINARY_DIR}/wampcc.pc DESTINATION "${INSTALL_PKGCONFIG_DIR}")
install(FILES ${PROJECT_BINARY_DIR}/wampcc_json.pc DESTINATION "${INSTALL_PKGCONFIG_DIR}")
