# --------------- END_TO_END TESTING -------------------------

# Set the root for searching for files to test.
set(TEST_GROUP_NAME "hh_tutorials")

# Make a list of the files that need to be built and run.
set(SRC_CPP 
    "tut1/createGateModel_completed.cpp"
    "tut2/createPotassiumChannelModel_completed.cpp"
    "tut3/debugSodiumChannelModel_completed.cpp"
    "tut4/generateMembraneModel_completed.cpp"
    # "tut5/solveGeneratedCode_completed.cpp"
)

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
set(ORIG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorials/hh_tutorials")
set(WORKING_PATH "${CMAKE_BINARY_DIR}/${TEST_GROUP_NAME}")
set(EXPECTED_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tests/${TEST_GROUP_NAME}")

# ----------------------------------------------------------------

if(REBUILD)
    include(tests/make_all_tests.cmake)
endif()