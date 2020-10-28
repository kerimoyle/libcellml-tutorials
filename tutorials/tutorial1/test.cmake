# This is a script that will be run within an external build directory
# when the tests are executed.  Tutorial users can ignore this file.

message(STATUS "Hello world, I am tutorial 1")

execute_process(
    COMMAND ./tutorial1_complete 
    OUTPUT_FILE output.txt 
    WORKING_DIRECTORY /Users/kmoy001/libcellml/libcellml-tutorials/tests/build/tutorial1/)

# Compare to expected output




