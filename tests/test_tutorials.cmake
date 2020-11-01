
# --------------- TUTORIALS TESTING -------------------------

# Make a list of the files that need to be built and run.
set(tutorial_cpp 
    tutorial1/tutorial1_complete.cpp
    tutorial2/tutorial2_complete.cpp
    tutorial3/tutorial3_complete.cpp
    tutorial5/tutorial5_complete.cpp
)

set(ORIG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorials")
set(utils_directory "${ORIG_SRC_DIR}")
set(EXPECTED_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tests/tutorial_tests")

get_filename_component(TESTS_PATH ${EXPECTED_OUTPUT_PATH} ABSOLUTE)

# Copy utils into the temp directory
file(GLOB transit 
    "${utils_directory}/tutorial_utilities.cpp"
    "${utils_directory}/tutorial_utilities.h"
)
file(COPY ${transit} DESTINATION ${TEMP_WORKING_PATH})

# Process each tutorial source.
foreach(s ${tutorial_cpp})
    set(src "${ORIG_SRC_DIR}/${s}")
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
        message(WARNING "${BoldMagenta}Can't find ${src}. ${ColourReset}")
    endif()

endforeach()