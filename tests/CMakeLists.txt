if(BUILD_SHARED_LIBS)
set(EXTRA_LIBS ${EXTRA_LIBS} wampcc_shared wampcc_json_shared)
else()
set(EXTRA_LIBS ${EXTRA_LIBS} wampcc_static wampcc_json_static)
endif()

list(APPEND EXTRA_LIBS uv crypto ssl pthread)

list(APPEND EXTRA_LIBS jansson)

if(BUILD_TESTS)

add_executable(test_misc
  "${PROJECT_SOURCE_DIR}/tests/test_misc.cc"
  "${PROJECT_SOURCE_DIR}/tests/test_common.h" )
set_property(TARGET test_misc PROPERTY CXX_STANDARD 11)
set_property(TARGET test_misc PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(test_misc ${EXTRA_LIBS})

##
## Add more test build rules here
##

endif() # BUILD_TESTS
