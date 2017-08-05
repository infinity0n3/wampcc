# Install script for directory: /mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/utility

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
  if(EXISTS "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/bin/admin" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/bin/admin")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/bin/admin"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/mnt/projects/aries_platform/wamp_brainstorming/local2/bin/admin")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/mnt/projects/aries_platform/wamp_brainstorming/local2/bin" TYPE EXECUTABLE FILES "/mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/build/utility/admin")
  if(EXISTS "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/bin/admin" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/bin/admin")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/bin/admin"
         OLD_RPATH "/mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/build/library/wampcc:/mnt/projects/aries_platform/wamp_brainstorming/wampcc/new_structure/build/library/json:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/mnt/projects/aries_platform/wamp_brainstorming/local2/bin/admin")
    endif()
  endif()
endif()

