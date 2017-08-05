if(BUILD_SHARED_LIBS)
set(EXTRA_LIBS ${EXTRA_LIBS} wampcc_shared wampcc_json_shared)
else()
set(EXTRA_LIBS ${EXTRA_LIBS} wampcc_static wampcc_json_static)
endif()

if (WIN32)
  list(APPEND EXTRA_LIBS libuv ssleay32 libeay32)
  # libuv library dependencies
  list(APPEND EXTRA_LIBS advapi32 iphlpapi psapi shell32 user32 userenv ws2_32)
else()
  list(APPEND EXTRA_LIBS uv crypto ssl pthread)
endif()

list(APPEND EXTRA_LIBS jansson)

if(BUILD_EXAMPLES)

##
## Basic embedded router
##
add_executable(basic_embedded_router "${PROJECT_SOURCE_DIR}/examples/basic/basic_embedded_router.cc")
set_property(TARGET basic_embedded_router PROPERTY CXX_STANDARD 11)
set_property(TARGET basic_embedded_router PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries (basic_embedded_router  ${EXTRA_LIBS})

if (WIN32)
  set_target_properties(example_basic_embedded_router PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt.lib /NODEFAULTLIB:libcmtd.lib")
endif()

##
## Add more example build rules here
##

endif() # BUILD_EXAMPLES
