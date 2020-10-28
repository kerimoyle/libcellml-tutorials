foreach(test_src ${TUTORIAL_SRC}) 
    message(STATUS ${src})

    # get_filename_component(test_name "${test_src}" NAME_WE)
    get_filename_component(temp "${test_src}/.." ABSOLUTE)
    get_filename_component(test_dir "${temp}" NAME)

    set(script_to_run "build/${test_dir}/test.cmake")

    # ADD_EXECUTABLE(main main.c)
    # ADD_CUSTOM_COMMAND(TARGET main
    #     POST_BUILD
    #     COMMAND main
    #     WORKING_DIRECTORY /var/tmp)

    # Run the test.cmake script in the test_dir
    execute_process(COMMAND ${CMAKE_COMMAND} -P ${script_to_run})

endforeach()


