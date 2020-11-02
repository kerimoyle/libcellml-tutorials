# This CMake file is intended to build and test the code from which 
# the libCellML Tutorials and How-to snippets are generated.  The 
# intention is that when these tests fail the developers of libCellML 
# will be alerted to the need to update the tutorials with changes in the API.
cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0007 NEW)

set(INSTALL_PREFIX "" CACHE PATH "Please enter the path to the libCellML install directory using the syntax -DINSTALL_PREFIX=your_path")

set(TEST_LIST 
    test_howto.cmake
    test_tutorials.cmake
)
set(TEST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/tests")

# General setup stuff
set(COMPARE_SCRIPT "compare_output.cmake")
include(tests/colours.cmake)

# Copy cmake scripts into the temp directory
file(GLOB transit 
    "${TEST_ROOT}/*.cmake"
)
file(COPY ${transit} DESTINATION ${CMAKE_BINARY_DIR})

# Make the tests.  Tests listed here should follow the template
# in these files.
foreach(t ${TEST_LIST})
    set(REBUILD ON)
    include("${TEST_ROOT}/${t}")
endforeach()

set(REBUILD OFF)

# Run "make test" to run all the tests.
add_custom_target(test
    COMMAND ${CMAKE_COMMAND} 
        -DTEST_LIST="${TEST_LIST}"
        -DTEST_ROOT=${TEST_ROOT}
        -DCOMPARE_SCRIPT=${COMPARE_SCRIPT}
        -P run_all_tests.cmake
)
