if(BUILD_SHARED_LIBS)
set(EXTRA_LIBS ${EXTRA_LIBS} wampcc_shared wampcc_json_shared)
else()
set(EXTRA_LIBS ${EXTRA_LIBS} wampcc_static wampcc_json_static)
endif()

list(APPEND EXTRA_LIBS uv crypto ssl pthread)
list(APPEND EXTRA_LIBS jansson)

if(BUILD_EXAMPLES)

##
## Basic embedded router
##
add_executable(basic_embedded_router "${PROJECT_SOURCE_DIR}/examples/basic/basic_embedded_router.cc")
set_property(TARGET basic_embedded_router PROPERTY CXX_STANDARD 11)
set_property(TARGET basic_embedded_router PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries (basic_embedded_router  ${EXTRA_LIBS})

##
## Add more example build rules here
##

endif() # BUILD_EXAMPLES
