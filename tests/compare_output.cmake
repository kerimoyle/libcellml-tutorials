# Expects command line inputs:
#    TEST
#    FILES
#    BUILD_PATH
#    TESTS_PATH

message(STATUS "BUILD_PATH = ${BUILD_PATH}")
message(STATUS "TESTS_PATH = ${TESTS_PATH}")
message(STATUS "TEST = ${TEST}")
message(STATUS "FILES = ${FILES}")

get_filename_component(abs ${BUILD_PATH} ABSOLUTE)
set(test_dir "${abs}/${TEST}")

get_filename_component(abs2 ${TESTS_PATH} ABSOLUTE)
set(expected_dir "${abs2}/${TEST}")

set(executable "./${TEST}_complete")

set(all_logs "${test_dir}/logs/${TEST}_report.txt")

message(STATUS "TEST = ${TEST}")

# Run the executable and collect the stdout in 'stdout'.
execute_process(
    COMMAND ${executable} 
    OUTPUT_FILE stdout
    WORKING_DIRECTORY "${test_dir}/"
)

# Compare any files in the output_files list.
set(error_count 0)
file(WRITE ${all_logs} "")



foreach(file_name ${FILES})

    message(STATUS "    ${file_name}")

    set(log "${test_dir}/logs/${file_name}.diff")
    set(expected "${expected_dir}/${file_name}")
    set(testfile "${test_dir}/${file_name}")

    message(STATUS "log = ${log}")
    message(STATUS "expected = ${expected}")
    message(STATUS "testfile = ${testfile}")

    
    file(REMOVE ${log})
    execute_process(COMMAND diff -u -E ${expected} ${testfile} OUTPUT_FILE ${log})

    # Test log file contents.  If the log file is empty then the test has passed.
    file(READ ${log} errors)

    if("${errors}" STREQUAL "")
        file(APPEND ${all_logs} 
            "OK: ${file}\n\n"
        )
        # file(REMOVE ${log})
    else()
        file(APPEND ${all_logs} 
                "ERROR: ${file}\n    See ${log} for details.\n\n"
            )
        math(EXPR error_count "${error_count}+1")
    endif()

endforeach()

if(${error_count} GREATER 0)
    file(APPEND ${all_logs}
            "Failed: ${error_count} files do not match.\n\n"
        )
else()
    file(APPEND ${all_logs}
            "All tests passed successfully.\n\n"
        )
endif()
