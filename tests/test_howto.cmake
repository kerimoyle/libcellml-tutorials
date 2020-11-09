# --------------- END_TO_END TESTING -------------------------

# Set the root for searching for files to test.
set(TEST_GROUP_NAME "howto")

# Make a list of the files that need to be built and run.
set(SRC_CPP 
    "annotation_tool_dev/annotationToolDev.cpp"
    "add_things/addThings.cpp"
    "import_debugger/importDebugger.cpp"
    "model_debugger/modelDebugger.cpp"
    "simulation_tool_dev/simulationToolDev.cpp"
)
# example_solveGeneratedModel.cpp

# Make a list of extra source files which need to be included in the 
# build.  These should be common for all files to be tested in the list
# above.
set(EXTRA_CPP 
    "${CMAKE_CURRENT_SOURCE_DIR}/utilities/utilities.cpp"
)

# Set the path(s) to any header files that are needed.
set(EXTRA_H 
    "${CMAKE_CURRENT_SOURCE_DIR}/utilities/utilities.h"
)

# These are the defaults. They can be changed.
set(ORIG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/howto/code")
set(WORKING_PATH "${CMAKE_BINARY_DIR}/${TEST_GROUP_NAME}")
set(EXPECTED_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tests/${TEST_GROUP_NAME}")

# ----------------------------------------------------------------

if(REBUILD)
    include(tests/make_all_tests.cmake)
endif()