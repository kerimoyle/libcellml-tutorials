# Expected command line inputs:
#   WORKING_PATH
#   TEST_LIST
#   TESTS_PATH

# message(STATUS "WORKING_PATH = ${WORKING_PATH}")
# message(STATUS "TESTS_PATH = ${TESTS_PATH}")
# message(STATUS "SRC_CPP = ${SRC_CPP}")

set(ORIG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_GROUP_NAME}")

cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0007 NEW)

include(colours.cmake)

# Splitting command line string.  NB: this means that files cannot have spaces in their names.
string(REPLACE " " ";" test_list ${TEST_LIST})

list(LENGTH test_list num_sets)

message("")
message("${Green}[==========]${ColourReset} Running ${num_sets} sets of tests.")

set(failed_count 0)
set(passed_count 0)
set(global_error_list "")

# Run through all tests.
foreach(t ${test_list})

    # Set the variables.  This includes the variables set in the .cmake file, but does
    # not trigger the build process.
    set(REBUILD OFF)
    include(${t})

    set(EXPECTED_OUTPUT_PATH "${TEST_ROOT}/${TEST_GROUP_NAME}")

    list(LENGTH SRC_CPP num)
    message("${Green}[----------] ${num} tests in ${TEST_GROUP_NAME}")

    # Clear previous report file.
    set(report_file "${WORKING_PATH}/test_report.txt")
    file(WRITE ${report_file} "")

    set(error_count 0)

    foreach(test_src ${SRC_CPP}) 

        get_filename_component(test "${test_src}" NAME_WE)
        get_filename_component(temp "${test_src}/.." ABSOLUTE)
        get_filename_component(test_to_run "${temp}" NAME) # Gets the folder containing the cpp file as the test name

        set(script_to_run "${EXPECTED_OUTPUT_PATH}/${test_to_run}/test.cmake")
        if(NOT EXISTS ${script_to_run})
            math(EXPR error_count "${error_count}+1")
            message(STATUS "    ${Magenta}ERROR: Cannot find test.cmake file at location: ${EXPECTED_OUTPUT_PATH}/${test_to_run}${ColourReset}")
            continue()
        endif()

        # Run the test.cmake script.  This will run the completed program, 
        # write any files, compare the outputs.  A file with extension "_report.txt" is 
        # created containing the results of the comparisons.
        execute_process(COMMAND ${CMAKE_COMMAND}
            -DWORKING_PATH=${WORKING_PATH} 
            -DTESTS_PATH=${TEST_ROOT} 
            -DTEST=${test_to_run}
            -DCOMPARE_SCRIPT=${COMPARE_SCRIPT}
            -DEXPECTED_OUTPUT_PATH=${EXPECTED_OUTPUT_PATH} 
            -P ${script_to_run}
        )

        # Compile the results of all of the tests into one file and one boolean flag.
        get_filename_component(abs ${WORKING_PATH} ABSOLUTE)
        set(tut_dir "${abs}/${test_to_run}")
        set(log_file "${tut_dir}/logs/${test_to_run}_report.txt")

        file(STRINGS ${log_file} log_list)
        list(LENGTH log_list list_len)
        math(EXPR list_last "${list_len} - 1")
        list(GET log_list ${list_last} log)

        if("${log}" STREQUAL "All tests passed successfully.")
            math(EXPR passed_count "${passed_count}+1")
        else()
            math(EXPR failed_count "${failed_count}+1")
            file(APPEND ${report_file} 
                "${test_to_run} : FAILED\n\n${log}\n\n"
            )
            string(APPEND global_error_list "${Magenta}[  FAILED  ]${ColourReset} ${test_to_run}.${test}\n")
        endif()

    endforeach()

    message("${Green}[----------]${ColourReset} ${num} tests ran.")
    message("")

endforeach()

math(EXPR total_count "${passed_count}+${failed_count}")
message("${Green}[==========]${ColourReset} ${total_count} tests in ${num_sets} sets ran.")
message("${Green}[  PASSED  ]${ColourReset} ${passed_count} tests.")
if(${failed_count} GREATER 1)
    message("${Magenta}[  FAILED  ]${ColourReset} ${failed_count} tests, listed below:")
elseif(${failed_count} GREATER 0)
    message("${Magenta}[  FAILED  ]${ColourReset} ${failed_count} test, listed below:")
else()
    message("${BoldGreen}[  PASSED  ]${ColourReset} Tests complete.")
endif()

message(${global_error_list})
message("")


