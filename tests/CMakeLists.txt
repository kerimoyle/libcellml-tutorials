# This CMake file is intended to build and test the code from which 
# the libCellML Tutorials and How-to snippets are generated.  The 
# intention is that when these tests fail the developers of libCellML 
# will be alerted to the need to update the tutorials with changes in the API.
cmake_minimum_required(VERSION 3.12)

set(INSTALL_PREFIX "" CACHE PATH "Please enter the path to the libCellML install directory using the syntax -DINSTALL_PREFIX=your_path")
set(BUILD_PATH "" CACHE PATH "Please enter the path to the directory in which the tests should be built using the syntax -DBUILD_PATH=your_path")

set(EXPECTED_OUTPUT_REL_PATH "./expected_outputs")
get_filename_component(EXPECTED_OUTPUT_PATH ${EXPECTED_OUTPUT_REL_PATH} ABSOLUTE)

message(STATUS "Build path is ${BUILD_PATH}")
message(STATUS "Expected outputs are in directory: ${EXPECTED_OUTPUT_PATH}")

# # Make a list of the files that need to be built and run
set(tutorial_src 
    ../tutorials/tutorial1/tutorial1_complete.cpp
    # ../tutorials/tutorial2/tutorial2_complete.cpp
    # ../tutorials/tutorial3/tutorial3_complete.cpp
    # ../tutorials/tutorial4/tutorial4_complete.cpp
    # ../tutorials/tutorial5/tutorial5_complete.cpp
)

set(utils_directory
    ../tutorials
)

# Loop through tutorial list of files, build each, run each, test outcome against
# expected outcomes

list(LENGTH tutorial_src len)
math(EXPR num_tests "${len} - 1")

# Make working directory locally.  This is where the source code and the 
# resources will be temporarily copied to.  It should be deleted at the end.
file(MAKE_DIRECTORY ${BUILD_PATH})

# Copy utils into the temp directory
file(GLOB transit 
    "${utils_directory}/tutorial_utilities.cpp"
    "${utils_directory}/tutorial_utilities.h"
)
file(COPY ${transit} DESTINATION ${BUILD_PATH})


get_filename_component(abs "." ABSOLUTE)
message(STATUS "My absolute path is: ${abs}")


foreach(src ${tutorial_src})

    message(STATUS "Starting with: ${src}")

    get_filename_component(src_file "${src}" NAME)
    get_filename_component(src_path "${src}" DIRECTORY)
    get_filename_component(test_name "${src}" NAME_WE)
    get_filename_component(test_dir "${src_path}" DIRECTORY)

    get_filename_component(a_second_dir "${src}/.." ABSOLUTE)
    get_filename_component(test_dir "${a_second_dir}" NAME)

    message(STATUS "test dir = ${test_dir}")

    file(MAKE_DIRECTORY "${BUILD_PATH}/${test_dir}")

    # Retrieve tutorial source and expected output from list
    if(EXISTS "${src}")
        message(STATUS "Processing ${src}")

        # Copy all code and resources into the local test folder.
        file(GLOB transit 
            "${src_path}/*.cpp"
            "${src_path}/*.c"
            "${src_path}/*.h"
            "${src_path}/*.cellml"
            "${src_path}/test.cmake"
        )
        file(COPY ${transit} DESTINATION "${BUILD_PATH}/${test_dir}/")
        
        # Build this tutorial.
        set(project_name "${test_name}")
        project(${project_name} VERSION 0.1.0)
        set(libCellML_DIR "${INSTALL_PREFIX}/lib/cmake/libCellML")
        find_package(libCellML REQUIRED)

        set(project_src
                "${BUILD_PATH}/${test_dir}/${src_file}"
                "${BUILD_PATH}/tutorial_utilities.cpp"
            )
        include_directories("${BUILD_PATH}/")

        add_executable(${project_name} ${project_src})
        set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}/${test_dir}")
        target_link_libraries(${project_name} PUBLIC cellml)

    else()
        message(FATAL_ERROR "Can't find ${src}")

    endif()

endforeach()

# Run with the "test" argument.
add_custom_target(test
      COMMAND ${CMAKE_COMMAND} 
        -DTUTORIAL_SRC=${tutorial_src} 
        -DBUILD_PATH=${BUILD_PATH} 
        -DEXPECTED_OUTPUT_PATH=${EXPECTED_OUTPUT_PATH}
        -P run_tests.cmake
    )
