# --------------- END_TO_END TESTING -------------------------

# Set the root for searching for files to test.
set(ORIG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/howto/code")

# Make a list of the files that need to be built and run.
set(SRC_CPP 
    annotation_tool_dev/annotationToolDev.cpp
    # example_importDebugger.cpp
    # example_modelDebugger.cpp
    # example_simulationToolDev.cpp
    # example_solveGeneratedModel.cpp
)

# Make a list of extra source files which need to be included in the 
# build.  These should be common for all files to be tested in the list
# above.
set(EXTRA_CPP )

# Set the path(s) to any include directories that are needed.
set(EXTRA_INCLUDE_DIR )

# For each of the files in the SRC_CPP list you must supply a corresponding
# set of resources to test against.  These will be in the same directory structure
# from the ORIG_SRC_DIR downwards, under the EXPECTED_OUTPUT_PATH root.  
set(EXPECTED_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tests/howto_tests")

# Make working directory locally.  This is where the source code and the 
# resources will be temporarily copied to.  It will be deleted by the 'make cleanup' command.
set(TEMP_WORKING_DIR "howto")

# ----------------------------------------------------------------

include(tests/make_all_tests.cmake)