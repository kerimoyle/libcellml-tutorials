# Set the names of the output files in this folder which should be compared.
# If you want to compare what's written to the console, then include a file named 'stdout'
# with the expected console output.
set(output_files 
    stdout  
)
set(test_to_run tutorial5_complete)

# -------------------------- DO NOT CHANGE ANYTHING BELOW THIS LINE -------------------------------

# Expecting command line inputs:
#     - TEMP_WORKING_PATH
#     - TESTS_PATH
#     - COMPARE_SCRIPT

# Run the processes launch this executable and compare the files.
execute_process(COMMAND ${CMAKE_COMMAND}
        -DTEMP_WORKING_PATH=${TEMP_WORKING_PATH} 
        -DTESTS_PATH=${TESTS_PATH} 
        -DTEST=${TEST}
        -DTEST_EXE=${test_to_run}
        "-DFILES=${output_files}"  # The quotes are needed in order to pass a list.
        -P ${COMPARE_SCRIPT}
    )