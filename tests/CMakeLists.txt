# This CMake file is intended to build and test the code from which 
# the libCellML Tutorials and How-to snippets are generated.  The 
# intention is that when these tests fail the developers of libCellML 
# will be alerted to the need to update the tutorials with changes in the API.
cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0007 NEW)

set(INSTALL_PREFIX "" CACHE PATH "Please enter the path to the libCellML install directory using the syntax -DINSTALL_PREFIX=your_path")

set(EXPECTED_OUTPUT_REL_PATH "./tutorial_tests")
set(COMPARE_SCRIPT "./compare_output.cmake")
set(TEMP_WORKING_PATH "./temp_testing")

set(TUTORIAL_SRC_DIR "../tutorials")

set(utils_directory
    "${TUTORIAL_SRC_DIR}"
)

include(colours.cmake)

get_filename_component(TESTS_PATH ${EXPECTED_OUTPUT_REL_PATH} ABSOLUTE)

# # Make a list of the files that need to be built and run
set(tutorial_cpp 
    tutorial1/tutorial1_complete.cpp 
    tutorial2/tutorial2_complete.cpp 
    tutorial3/tutorial3_complete.cpp 
    # ../tutorials/tutorial4/tutorial4_complete.cpp
    tutorial5/tutorial5_complete.cpp
)

# Make working directory locally.  This is where the source code and the 
# resources will be temporarily copied to.  It should be deleted at the end.
file(MAKE_DIRECTORY ${TEMP_WORKING_PATH})

# Copy utils into the temp directory
file(GLOB transit 
    "${utils_directory}/tutorial_utilities.cpp"
    "${utils_directory}/tutorial_utilities.h"
)
file(COPY ${transit} DESTINATION ${TEMP_WORKING_PATH})

foreach(s ${tutorial_cpp})

    set(src "${TUTORIAL_SRC_DIR}/${s}")
    get_filename_component(src_file "${src}" NAME)
    get_filename_component(src_path "${src}" DIRECTORY)
    get_filename_component(test_name "${src}" NAME_WE)
    get_filename_component(test_dir "${src_path}" DIRECTORY)

    get_filename_component(a_second_dir "${src}/.." ABSOLUTE)
    get_filename_component(test_dir "${a_second_dir}" NAME)

    file(MAKE_DIRECTORY "${TEMP_WORKING_PATH}/${test_dir}")

    # Retrieve tutorial source and expected output from list.
    if(EXISTS "${src}")
        message(STATUS "Processing ${src}")

        # Copy all code and resources into the local test folder.
        file(GLOB transit 
            "${src_path}/*.cpp"
            "${src_path}/*.c"
            "${src_path}/*.h"
            "${src_path}/*.cellml"
        )
        file(COPY ${transit} DESTINATION "${TEMP_WORKING_PATH}/${test_dir}/")
        
        # Build this tutorial.
        set(project_name "${test_name}")
        project(${project_name} VERSION 0.1.0)
        set(libCellML_DIR "${INSTALL_PREFIX}/lib/cmake/libCellML")
        find_package(libCellML REQUIRED)

        set(project_src
                "${TEMP_WORKING_PATH}/${test_dir}/${src_file}"
                "${TEMP_WORKING_PATH}/tutorial_utilities.cpp"
            )
        include_directories("${TEMP_WORKING_PATH}/")

        add_executable(${project_name} ${project_src})
        set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEMP_WORKING_PATH}/${test_dir}")
        target_link_libraries(${project_name} PUBLIC cellml)

    else()
        message(FATAL_ERROR "${BoldRed}Can't find ${src}${ColorReset}")
    endif()

endforeach()


message("COMMAND ${CMAKE_COMMAND} '-DTUTORIAL_CPP=${tutorial_cpp}' -DTEMP_WORKING_PATH=${TEMP_WORKING_PATH} 
-DTESTS_PATH=${TESTS_PATH}
-DCOMPARE_SCRIPT=${COMPARE_SCRIPT}
-P run_all_tests.cmake")


# Run "make test" to run all the tests.
add_custom_target(hello
    COMMAND ${CMAKE_COMMAND} 
        -DTUTORIAL_CPP="${tutorial_cpp}"
        -DTEMP_WORKING_PATH=${TEMP_WORKING_PATH} 
        -DTESTS_PATH=${TESTS_PATH}
        -DCOMPARE_SCRIPT=${COMPARE_SCRIPT}
        -P run_all_tests.cmake
)

# Run "make cleanup" to delete all the files created by CMake and the tests.
add_custom_target(cleanup
    COMMAND rm -rf ${TEMP_WORKING_PATH} CMakeCache.txt CMakeFiles Makefile cmake_install.cmake
)