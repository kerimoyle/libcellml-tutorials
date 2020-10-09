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

if __name__ == '__main__':

    print('-----------------------------------------------------------')
    print('   Step 1: Link to the generated code                      ')
    print('-----------------------------------------------------------')

    #  1.a
    #       Use the importlib functionality to open the generated code file.
    spec = importlib.util.spec_from_file_location('HodgkinHuxleyModel', 'HodgkinHuxleyModel.py')
    module = importlib.util.module_from_spec(spec)

    #  1.b
    #       Load into a module.
    sys.modules['HodgkinHuxleyModel'] = module
    spec.loader.exec_module(module)

    #  1.c Change the module to be callable as 'model'. 
    model = module

    print('-----------------------------------------------------------')
    print('   Step 2: Access the variables in the generated files     ')
    print('-----------------------------------------------------------')

    #  Probably the best way to understand the contents of the generated files is
    #  to open them and look!  The implementation file (*.cpp) has two types of items:
    #   - information structures (in all-caps) and
    #   - access functions.
    #  It's important to remember that in the generated code we don't have the notion of
    #  separate components: they are listed here with the variables only in order to give
    #  the correct context to the variable names.

    #  'Variables' are anything which does not require integration as part of the
    #  solution, and could have types COMPUTED_CONSTANT (needs to be calculated
    #  but doesn't need integration), CONSTANT (no calculation needed), or
    #  ALGEBRAIC (TODO??) as defined in the VariableType enum.
    #  They are stored in an array of dictionaries called
    #  VARIABLE_INFO which is VARIABLE_COUNT long.  This contains:
    #     - name,
    #     - units,
    #     - component, and
    #     - type.

    #  2.a
    #       Get the number of variables and iterate through the VARIABLE_INFO dict to
    #       retrieve and print each variable's information to the terminal.
   
    #      'State variables' are those which need integration.
    #      They are stored in an array of dicts called STATE_INFO which
    #      is STATE_COUNT long.  The dict contains:
    #          - name,
    #          - units, and
    #          - component.

    #  2.b
    #       Get the number of state variables and iterate through the STATE_INFO dict to
    #       retrieve and print each state variable's information to the terminal.
    
    #  2.c 
    #       Get the integration variable and print its information to the terminal. This
    #       is stored in a dict called VOI_INFO.
    
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
    #       Create three arrays representing:
    #       - the variables (which here includes constants)
    #       - the states (the integrated variables)
    #       - the rates 
    #       Create and initialise a variable of integration, time.
    
    #  3.b 
    #       Use the functions provided to initialise the arrays you created, then print them 
    #       to the screen for checking.

    #  3.c 
    #       Compute the constants, compute the variables, and print them to the screen for checking.

    print('-----------------------------------------------------------')
    print('   Step 4: Iterate through the solution                    ')
    print('-----------------------------------------------------------')

    #  This part will make use of a simple routine to step through the solution
    #  iterations using the Euler method to update the state variables.

    #  4.a 
    #       Create variables which control how the solution will run, representing:
    #       - step size and
    #       - the number of steps to take.

    #  4.b Create a file for output and open it. You can use the information to name columns
    #      with the variables, component, and units so you can keep track later.

    #      The Euler update method is: x[n+1] = x[n] + x'[n]*stepSize
    #      At each step you will need to:
    #          - Compute the variables; **
    #          - Compute the rates;
    #          - Compute the state variables using the update method above; and
    #          - Print to a file.
    #      ** We only need to compute these each timestep here because we're also 
    #         writing the values to the file at each timestep.

    #  4.c 
    #      Iterate through the time domain and write the solution at each step. 
    