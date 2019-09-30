foreach(_API_HEADER_FILE ${API_HEADER_FILES})
  get_filename_component(TEST_HEADER_NAME ${_API_HEADER_FILE} NAME_WE)
  get_filename_component(TEST_HEADER ${_API_HEADER_FILE} NAME)
  set(CURRENT_TEST test_api_headers_${TEST_HEADER_NAME})

  set(TEST_SRC ${CMAKE_CURRENT_BINARY_DIR}/api_header_${TEST_HEADER_NAME}.cpp)
  set(TEST_CMAKE_FILE ${CMAKE_CURRENT_BINARY_DIR}/api_header_${TEST_HEADER_NAME}.cmake)
  configure_file(api_headers/api_header_test.in.cpp ${TEST_SRC})
  configure_file(api_headers/api_header_test.in.cmake ${TEST_CMAKE_FILE})
  include(${TEST_CMAKE_FILE})
endforeach()

message(STATUS "TEST_LIST: ${TEST_LIST}")
