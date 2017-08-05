# Install script for directory: /mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/library/json

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/mnt/projects/aries_platform/wamp_brainstorming/local2")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  foreach(file
      "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/lib/libwampcc_json.so.4.0.0"
      "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/lib/libwampcc_json.so.4"
      "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/lib/libwampcc_json.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/mnt/projects/aries_platform/wamp_brainstorming/local2/lib/libwampcc_json.so.4.0.0;/mnt/projects/aries_platform/wamp_brainstorming/local2/lib/libwampcc_json.so.4;/mnt/projects/aries_platform/wamp_brainstorming/local2/lib/libwampcc_json.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/mnt/projects/aries_platform/wamp_brainstorming/local2/lib" TYPE SHARED_LIBRARY FILES
    "/mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/build/library/json/libwampcc_json.so.4.0.0"
    "/mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/build/library/json/libwampcc_json.so.4"
    "/mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/build/library/json/libwampcc_json.so"
    )
  foreach(file
      "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/lib/libwampcc_json.so.4.0.0"
      "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/lib/libwampcc_json.so.4"
      "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/lib/libwampcc_json.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/mnt/projects/aries_platform/wamp_brainstorming/local2/include/wampcc/json.h;/mnt/projects/aries_platform/wamp_brainstorming/local2/include/wampcc/json_internals.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/mnt/projects/aries_platform/wamp_brainstorming/local2/include/wampcc" TYPE FILE FILES
    "/mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/include/wampcc/json.h"
    "/mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/include/wampcc/json_internals.h"
    )
endif()

