# Expects command line inputs:
#    TEST
#    FILES
#    WORKING_PATH
#    EXPECTED_OUTPUT_PATH
#    TEST_EXE

# message(STATUS "WORKING_PATH = ${WORKING_PATH}")
# message(STATUS "EXPECTED_OUTPUT_PATH = ${EXPECTED_OUTPUT_PATH}")
# message(STATUS "TEST = ${TEST}")
# message(STATUS "FILES = ${FILES}")
# message(STATUS "TEST_EXE = ${TEST_EXE}")

include(colours.cmake)

get_filename_component(abs ${WORKING_PATH} ABSOLUTE)

set(test_dir "${abs}/${TEST}")
set(expected_dir "${EXPECTED_OUTPUT_PATH}/${TEST}")
set(executable "./${TEST_EXE}")

message("${Green}[ RUN      ]${ColourReset} ${TEST}.${TEST_EXE}")

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
        message("       ${Magenta}ERROR! Could not open expected output file: ${expected}${ColourReset}")
    elseif(NOT EXISTS "${testfile}")
        message("       ${Magenta}ERROR! Could not open test output file: ${testfile}${ColourReset}")
    else()

        execute_process(COMMAND diff -u -E ${expected} ${testfile} OUTPUT_FILE ${log})

        # Test log file contents.  If the log file is empty then the test has passed.
        file(READ ${log} errors)

        if("${errors}" STREQUAL "")
            file(REMOVE ${log})
        else()
            file(APPEND ${all_logs} 
                    "ERROR: ${file_name}\n"
                )
            file(APPEND ${all_logs} 
                "${errors}"
            )
            file(APPEND ${all_logs} "\n")
            math(EXPR error_count "${error_count}+1")
            message("                ERROR: ${file_name} does not match exemplar.")
        endif()

    endif()

endforeach()

if(${error_count} GREATER 0)
    # file(APPEND ${all_logs}
    #         "Failed: ${error_count} files do not match.\n\n"
    #     )
    message("${Magenta}[  FAILED  ]${ColourReset} ${TEST}.${TEST_EXE}")
else()
    file(APPEND ${all_logs}
            "All tests passed successfully."
        )
    message("${Green}[       OK ]${ColourReset} ${TEST}.${TEST_EXE}")
endif()
