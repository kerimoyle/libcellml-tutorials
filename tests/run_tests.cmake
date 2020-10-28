foreach(test_src ${TUTORIAL_SRC}) 
    message(STATUS ${src})

    get_filename_component(temp "${test_src}/.." ABSOLUTE)
    get_filename_component(test_dir "${temp}" NAME)

    set(script_to_run "${BUILD_PATH}/${test_dir}/test.cmake")

    # Run the test.cmake script in the test_dir.  This will run the completed program, 
    # write any files, compare the outputs.  A file with extension "_report.txt" is 
    # created containing the results of the comparisons.
    execute_process(COMMAND ${CMAKE_COMMAND}
        -DBUILD_PATH=${BUILD_PATH} 
        -DEXPECTED_OUTPUT_PATH=${EXPECTED_OUTPUT_PATH} 
        -P ${script_to_run}
    )

endforeach()


