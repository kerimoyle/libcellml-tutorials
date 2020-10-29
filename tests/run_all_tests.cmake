# Expected command line inputs:
#   BUILD_PATH
#   TUTORIAL_SRC
#   TESTS_PATH

# Clear previous report file.
set(report_file "${BUILD_PATH}/test_report.txt")
file(WRITE ${report_file} "")

set(error_count 0)

# Run through all tests.
foreach(test_src ${TUTORIAL_SRC}) 

    get_filename_component(temp "${test_src}/.." ABSOLUTE)
    get_filename_component(test_to_run "${temp}" NAME)

    message(STATUS "Lauching test: ${test_to_run}")

    set(script_to_run "${TESTS_PATH}/${test_to_run}/test.cmake")

    message(${script_to_run})

    # Run the test.cmake script.  This will run the completed program, 
    # write any files, compare the outputs.  A file with extension "_report.txt" is 
    # created containing the results of the comparisons.
    execute_process(COMMAND ${CMAKE_COMMAND}
        -DBUILD_PATH=${BUILD_PATH} 
        -DTESTS_PATH=${TESTS_PATH} 
        -DTEST=${test_to_run}
        -DCOMPARE_SCRIPT=${COMPARE_SCRIPT}
        -P ${script_to_run}
    )

    # Compile the results of all of the tests into one file and one boolean flag.
    get_filename_component(abs ${BUILD_PATH} ABSOLUTE)
    set(tut_dir "${abs}/${test_to_run}")
    set(log_file "${tut_dir}/logs/${test_to_run}_report.txt")

    file(READ ${log_file} log)
    if("${log}" STREQUAL "All tests passed successfully.")
        file(APPEND ${report_file} 
            "${test_to_run} : PASSED\n\n"
        )
    else()
        math(EXPR error_count "${error_count}+1")
        file(APPEND ${report_file} 
"
${test_to_run} : FAILED\n\n
${log}\n\n
"
)
    endif()

    

endforeach()

message(STATUS "Tests finished: ${error_count} errors.")
