
# --------------- END_TO_END TESTING -------------------------

# Set the name of this group of tests.  This will be used as the folder
# name inside the current source directory to look for tests, as well
# as the place where they'll end up in the temporary build directory.
set(TEST_GROUP_NAME "tutorials")

# Make a list of the files that need to be built and run.
set(SRC_CPP 
    tutorial1/tutorial1_complete.cpp
    tutorial2/tutorial2_complete.cpp
    tutorial3/tutorial3_complete.cpp
    tutorial5/tutorial5_complete.cpp
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
set(ORIG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_GROUP_NAME}")
set(WORKING_PATH "${CMAKE_BINARY_DIR}/${TEST_GROUP_NAME}")
set(EXPECTED_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tests/${TEST_GROUP_NAME}")

# ------------------ Launch the build ---------------------------
if(REBUILD)
    include(tests/make_all_tests.cmake)
endif()
