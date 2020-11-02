# This CMake file is intended to build and test the code from which 
# the libCellML Tutorials and How-to snippets are generated.  The 
# intention is that when these tests fail the developers of libCellML 
# will be alerted to the need to update the tutorials with changes in the API.
cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0007 NEW)

set(INSTALL_PREFIX "" CACHE PATH "Please enter the path to the libCellML install directory using the syntax -DINSTALL_PREFIX=your_path")



# General setup stuff
set(COMPARE_SCRIPT "compare_output.cmake")
include(tests/colours.cmake)

# Copy cmake scripts into the temp directory
file(GLOB transit 
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cmake"
)
file(COPY ${transit} DESTINATION ${CMAKE_BINARY_DIR})

# Make the tutorials tests
include(tests/test_tutorials.cmake)

# Make the how-to tests
include(tests/test_howto.cmake)

# Run "make test" to run all the tests.
add_custom_target(test
    COMMAND ${CMAKE_COMMAND} 
        -DTUTORIAL_CPP="${tutorial_cpp}"
        -DTEMP_WORKING_PATH=${TEMP_WORKING_PATH} 
        -DTESTS_PATH=${TESTS_PATH}
        -DCOMPARE_SCRIPT=${COMPARE_SCRIPT}
        -P run_all_tests.cmake
)

# Run "make cleanup" to delete all the files created by CMake and the tests, including the log files.
add_custom_target(cleanup
    COMMAND rm -rf ${TEMP_WORKING_PATH} CMakeCache.txt CMakeFiles Makefile *.cmake
)