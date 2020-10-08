'''
    COMBINE2020 libCellML Tutorial 5: INTERACT WITH GENERATED CODE
  
    By the time you have worked through this tutorial you will be able to:
      - Investigate and understand the contents of files created by the Generator and
      - Integrate generated code into a simple solver to run a simulation.
  
    This tutorial assumes that you are comfortable with:
      - Interacting with a model and its entities using the API (see )
      - Using the Generator functionality to output files in C or Python (see ) and
      - The basic idea of numerical integration using Euler's method (see ).
'''

import sys
import importlib

def module_from_file(input):

    # Shouldn't need to check this if the extension is stripped during input
    if input[-3:] != '.py':
        module_file = input + '.py'
        module_name = input.split('/')[-1]
    else:
        module_file = input
        module_name = '.'.join(input.split('/')[-1].split('.')[:-1])

    print(module_file)
    print(module_name)

    spec = importlib.util.spec_from_file_location(module_name, module_file)
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module


if __name__ == '__main__':

    print('-----------------------------------------------------------')
    print('   Step 1: Link to the generated code                      ')
    print('-----------------------------------------------------------')


    model = module_from_file('HodgkinHuxleyModel.py')
    # model = importlib.import_module('HodgkinHuxleyModel.py')


    print('-----------------------------------------------------------')
    print('   Step 2: Access the variables in the generated files     ')
    print('-----------------------------------------------------------')

    # STEP 2: Accessing the contents of the generated files.
    #      Probably the best way to understand the contents of the generated files is
    #      to open them and look!  The implementation file (*.cpp) has two types of items:
    #      - information structures (in all-caps) and
    #      - access functions.
    #  It's important to remember that in the generated code we don't have the notion of
    #  separate components: they are listed here with the variables only in order to give
    #  the correct context to the variable names.

    # 'Variables' are anything which does not require integration as part of the
    # solution, and could have types COMPUTED_CONSTANT (needs to be calculated
    # but doesn't need integration), CONSTANT (no calculation needed), or
    # ALGEBRAIC (TODO??) as defined in the VariableType enum.
    # They are stored in an array of VariableInfoWithType structs called
    # VARIABLE_INFO which is VARIABLE_COUNT long.  The VariableInfoWithType contains:
    #     - name,
    #     - units,
    #     - component, and
    #     - VariableType.

    #  2.a
    #       Get the number of variables and iterate through the VARIABLE_INFO structure to
    #       retrieve and print each variable's information to the terminal.
    print('VARIABLE_COUNT = {}'.format(model.VARIABLE_COUNT))

    for v in range (0, model.VARIABLE_COUNT):
        print('Variable {}:'.format(v))
        print('  name = {}'.format(model.VARIABLE_INFO[v]['name']))
        print('  units = {}'.format(model.VARIABLE_INFO[v]['units']))
        print('  component = {}'.format(model.VARIABLE_INFO[v]['component']))
        print('  type = {}'.format(model.VARIABLE_INFO[v]['type']))
    print()
    #      'State variables' are those which need integration.
    #      They are stored in an array of VariableInfo structs called STATE_INFO which
    #      is STATE_COUNT long.  The VariableInfo struct contains:
    #          - name,
    #          - units, and
    #          - component.

    #  2.b
    #       Get the number of state variables and iterate through the STATE_INFO structure to
    #       retrieve and print each state variable's information to the terminal.
    print('STATE_COUNT = {}'.format(model.STATE_COUNT))
    for s in range(0, model.STATE_COUNT):
        print('State variable {}:'.format(s))
        print('  name = {}'.format(model.STATE_INFO[s]['name']))
        print('  units = {}'.format(model.STATE_INFO[s]['units']))
        print('  component = {}'.format(model.STATE_INFO[s]['component']))
    print()

    #  2.c 
    #       Get the integration variable and print its information to the terminal. This
    #       is stored in a VariableInfo struct called VOI_INFO.
    print('VOI_INFO')
    print('  name = {}'.format(model.VOI_INFO['name']))
    print('  units = {}'.format(model.VOI_INFO['units']))
    print('  component = {}'.format(model.VOI_INFO['component']))
    print()

    #  end 2

    print('-----------------------------------------------------------')
    print('   Step 3: Access the functions in the generated files     ')
    print('-----------------------------------------------------------')

    #   The generated code contains seven functions:
    #      - createStatesArray() to allocate an array of length STATE_COUNT.  This can be
    #        used to allocate the 'rates' or gradient function array too as they're the 
    #        same length
    #      - createVariablesArray() to allocate an array of length VARIABLE_COUNT
    #      - deleteArray() to free memory used by the given array
    #      - initialiseStatesAndConstants(states, variables) will do what it says on the tin,
    #        and populate the given pre-allocated arrays with the initial values for all of the
    #        model's state variables and constants.
    #      - computeComputedConstants(variables) will fill in values for any variables that 
    #        do not change in value throughout the solution, but still need to be calculated
    #      - computeRates(VOI, states, rates, variables) updates the rates array with the 
    #        gradients of the state variables, given the values of the other variables and the 
    #        variable of integration (VOI)
    #      - computeVariables(VOI, states, rates, variables) updates any non-integrated variables
    #        whose values do not affect the integration.  Since this doesn't affect the solution
    #        process it only needs to be called whenever the values need to be output not 
    #        necessarily each integration timestep.

    #  3.a 
    #       Create two arrays and use the functions to allocate them.  One will represent the
    #       variables, and one will represent the state variables. 
    my_variables = model.create_variables_array()
    my_state_variables = model.create_states_array()

    #  3.b 
    #       Use the functions provided to initialise the arrays you created, then print them 
    #       to the screen for checking.
    model.initialise_states_and_constants(my_state_variables, my_variables)

    print('The initial values for variables are:')
    for v in range(0, model.VARIABLE_COUNT):
        print('  {} = {} ({})'.format(
            model.VARIABLE_INFO[v]['name'],
            my_variables[v],
            model.VARIABLE_INFO[v]['units']
            ))
    print()

    print('The initial conditions for state variables are:')
    for v in range(0, model.STATE_COUNT):
        print('  {} = {} ({})'.format(
            model.STATE_INFO[v]['name'],
            my_variables[v],
            model.STATE_INFO[v]['units']
            ))
    print()

    #  3.c 
    #       Compute the computed constants and print them to the screen for checking.
    print('The initial values including all computed constants are:')
    model.compute_computed_constants(my_variables)
    for v in range(0, model.VARIABLE_COUNT):
        print('  {} = {} ({})'.format(
            model.VARIABLE_INFO[v]['name'],
            my_variables[v],
            model.VARIABLE_INFO[v]['units']
            ))
    print()

    #  end 3

    print('-----------------------------------------------------------')
    print('   Step 4: Iterate through the solution                    ')
    print('-----------------------------------------------------------')

    # STEP 4: This part will make use of a simple routine to step through the solution
    #         iterations using the Euler method to update the state variables.

    #  4.a 
    #       Create variables which control how the solution will run, representing:
    #       - variable of integration (time)
    #       - step size and
    #       - the number of steps to take.
    time = 0.0
    step_size = 0.01
    step_count = 2000
    incr = int(step_count/60) + 1

    #  4.b Create an array for the rates.  You can use the same createStatesArray() 
    #      function to allocate this as the number of rates will always equal the 
    #      number of state variables.
    my_rates = model.create_states_array()

    #  4.c Create a file for output and open it. You can use the information to name columns
    #      with the variables, component, and units so you can keep track later.
    write_file = open('HodgkinHuxleyModelSolution.txt', 'w')
    row = 'iteration\t{}({})'.format(
        model.VOI_INFO['name'], model.VOI_INFO['units'])
    for s in range(0, model.STATE_COUNT):
        row += '\t{}({})'.format(model.STATE_INFO[s]
                                 ['name'], model.STATE_INFO[s]['units'])
    for s in range(0, model.VARIABLE_COUNT):
        row += '\t{}({})'.format(model.VARIABLE_INFO[s]
                                 ['name'], model.VARIABLE_INFO[s]['units'])
    row += '\n'
    write_file.write(row)

    #  end 4.c
    #      The Euler update method is: x[n+1] = x[n] + x'[n]*stepSize
    #      At each step you will need to:
    #          - Compute the variables; **
    #          - Compute the rates;
    #          - Compute the state variables using the update method above; and
    #          - Print to a file.
    #      ** We only need to compute these each timestep here because we're also 
    #         writing the values to the file at each timestep.

    #  4.d 
    #      Iterate through the time domain and write the solution at each step. 
    for step in range(0, step_count):
        time = step * step_size

        # Compute the rates at this step using the given function.
        model.compute_rates(time, my_state_variables, my_rates, my_variables)

        row = '{}\t{}'.format(step, time)

        # Compute the states.
        for s in range(0, model.STATE_COUNT):
            my_state_variables[s] = my_state_variables[s] + my_rates[s] * step_size
            row += '\t{}'.format(my_state_variables[s])

        # Compute the variables.
        model.compute_variables(time, my_state_variables, my_rates, my_variables)
        for s in range(0, model.VARIABLE_COUNT):
            row += '\t{}'.format(my_variables[s])

        row += '\n'
        write_file.write(row)

        # Print progress bar.
        if step % incr == 0:
            print('.', end='', flush=True)

    write_file.close()
   
   #  end 4

    print()
    print()

    print('The results have been written to \'HodgkinHuxleyModelSolution.txt\'')
