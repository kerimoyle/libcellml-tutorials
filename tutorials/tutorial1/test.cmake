# This is a script that will be run within an external build directory
# when the tests are executed.  Tutorial users can ignore this file.


# Change the name of the tutorial here, and add the names of any output files to 
# compare to the list.  Other than that, the rest of the file should not need to be altered
# unless you're dealing with code generation.

set(tut "tutorial1")

set(output_files 
    stdout  # The list should include a "stdout" file if the console output is to be tested.
    tutorial1_printed.cellml
)

#######

get_filename_component(abs ${BUILD_PATH} ABSOLUTE)
set(tut_dir "${abs}/${tut}")

get_filename_component(abs2 ${EXPECTED_OUTPUT_PATH} ABSOLUTE)
set(expected_dir "${abs2}/${tut}")

set(executable "./${tut}_complete")

set(all_logs "${tut_dir}/logs/${tut}_report.txt")

# Run the executable and collect the stdout in 'stdout'.
execute_process(
    COMMAND ${executable} 
    OUTPUT_FILE stdout
    WORKING_DIRECTORY "${tut_dir}/"
)

# Compare any files in the output_files list.
set(error_count 0)
file(WRITE ${all_logs} "")

foreach(file ${output_files})

    message(STATUS "Testing ${file}:")

    get_filename_component(b ${file} NAME_WE)
    get_filename_component(e ${file} EXT)

    set(log "${tut_dir}/logs/${file}.diff")
    set(expected "${expected_dir}/${file}")
    set(testfile "${tut_dir}/${file}")

    file(REMOVE ${log})
    execute_process(COMMAND diff -u -E ${expected} ${testfile} OUTPUT_FILE ${log})

    # Add log file contents (if they exist) to global log file.  If the log file is
    # empty or does not exist then the test has passed.
    file(READ ${log} errors)

    if("${errors}" STREQUAL "")
        file(APPEND ${all_logs} 
            "OK: ${file}\n\n"
        )
        file(REMOVE ${log})
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





