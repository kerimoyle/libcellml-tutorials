# Expects command line inputs:
#    TEST
#    FILES
#    TEMP_WORKING_PATH
#    TESTS_PATH
#    TEST_EXE

# message(STATUS "TEMP_WORKING_PATH = ${TEMP_WORKING_PATH}")
# message(STATUS "TESTS_PATH = ${TESTS_PATH}")
# message(STATUS "TEST = ${TEST}")
# message(STATUS "FILES = ${FILES}")
# message(STATUS "TEST_EXE = ${TEST_EXE}")

include(colours.cmake)

get_filename_component(abs ${TEMP_WORKING_PATH} ABSOLUTE)
set(test_dir "${abs}/${TEST}")

get_filename_component(abs2 ${TESTS_PATH} ABSOLUTE)
set(expected_dir "${abs2}/${TEST}")

set(executable "./${TEST_EXE}")

message("     - testing ${executable}:")

set(all_logs "${test_dir}/logs/${TEST}_report.txt")

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

    set(log "${test_dir}/logs/${file_name}.diff")
    set(expected "${expected_dir}/${file_name}")
    set(testfile "${test_dir}/${file_name}")

    file(REMOVE ${log})

    # Check that the files to compare exist:
    if(NOT EXISTS "${expected}")
        message("       ${Magenta}${expected}: ERROR! Could not open expected output file.${ColourReset}")
    elseif(NOT EXISTS "${testfile}")
        message("       ${Magenta}${testfile}: ERROR! Could not open test output file.${ColourReset}")
    else()

        execute_process(COMMAND diff -u -E ${expected} ${testfile} OUTPUT_FILE ${log})

        # Test log file contents.  If the log file is empty then the test has passed.
        file(READ ${log} errors)

        if("${errors}" STREQUAL "")
            message("       ${Green}${file_name}: OK${ColourReset}")
            file(APPEND ${all_logs} 
                "OK: ${file_name}\n\n"
            )
            file(REMOVE ${log})
        else()
            file(APPEND ${all_logs} 
                    "ERROR: ${file}\n    See ${log} for details.\n\n"
                )
            math(EXPR error_count "${error_count}+1")
            message("       ${Magenta}${file_name}: ERRORS${ColourReset}")
        endif()
    endif()

endforeach()

if(${error_count} GREATER 0)
    file(APPEND ${all_logs}
            "Failed: ${error_count} files do not match.\n\n"
        )
else()
    file(APPEND ${all_logs}
            "All tests passed successfully."
        )
endif()
