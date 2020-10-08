/**
 *  COMBINE2020 libCellML Tutorial 5: INTERACT WITH GENERATED CODE
 *
 *  By the time you have worked through this tutorial you will be able to:
 *    - Investigate and understand the contents of files created by the Generator; and
 *    - Integrate generated code into a simple solver to run a simulation.
 *
 *  This tutorial assumes that you are comfortable with:
 *    - Interacting with a model and its entities using the API (see );
 *    - Using the Generator functionality to output files in C or Python (see ); and
 *    - The basic idea of numerical integration using Euler's method (see ).
 */

#include <fstream>
#include <iostream>
#include <sstream>

// Change this to the name of your header file if need be.
#include "HodgkinHuxleyModel.h"

int main()
{
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "   Step 1: Link to the generated code                      " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;

    //  Include the generated code in this project.  Note that many of these
    //  steps occur in other files.

    //  1.a Enter the path to the generated header/interface *.h file in the #include block above.

    //  1.b Add the name and path of the implementation *.cpp file in the CMakeLists.txt file, 
    //      (or whatever your local toolchain requires).

    //  1.c Open the implementation file *.cpp file and verify that the #include statement in
    //      line 3 has the filename of your interface *.h file.  Amend if needed and close the file.

    //  1.e Call cmake to create the Makefile.  Call make -j to build the executable.
    //      Run the code so far to check that the libCellML versions match.

    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "   Step 2: Access the variables in the generated files     " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;

    //      Probably the best way to understand the contents of the generated files is
    //      to open them and look!  The implementation file (*.cpp) has two types of items:
    //      - information structures (in all-caps); and
    //      - access functions.
    //  It's important to remember that in the generated code we don't have the notion of
    //  separate components: they are listed here with the variables only in order to give
    //  the correct context to the variable names.

    //      "Variables" are anything which does not require integration as part of the
    //      solution, and could have types COMPUTED_CONSTANT (needs to be calculated
    //      but doesn't need integration), CONSTANT (no calculation needed), or
    //      ALGEBRAIC (TODO??) as defined in the VariableType enum.
    //      They are stored in an array of VariableInfoWithType structs called
    //      VARIABLE_INFO which is VARIABLE_COUNT long.  The VariableInfoWithType contains:
    //          - name,
    //          - units,
    //          - component, and
    //          - VariableType.

    //  2.a
    //  Get the number of variables and iterate through the VARIABLE_INFO structure to
    //  retrieve and print each variable's information to the terminal.

    //      "State variables" are those which need integration.
    //      They are stored in an array of VariableInfo structs called STATE_INFO which
    //      is STATE_COUNT long.  The VariableInfo struct contains:
    //          - name,
    //          - units, and
    //          - component.

    //  2.b
    //      Get the number of state variables and iterate through the STATE_INFO structure to
    //      retrieve and print each state variable's information to the terminal.

    //  2.c 
    //      Get the integration variable and print its information to the terminal. This
    //      is stored in a VariableInfo struct called VOI_INFO.

    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "   Step 3: Access the functions in the generated files     " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;

    //   The generated code contains seven functions:
    //      - createStatesArray() to allocate an array of length STATE_COUNT.  This can be
    //        used to allocate the "rates" or gradient function array too as they're the 
    //        same length;
    //      - createVariablesArray() to allocate an array of length VARIABLE_COUNT;
    //      - deleteArray() to free memory used by the given array;
    //      - initialiseStatesAndConstants(states, variables) will do what it says on the tin,
    //        and populate the given pre-allocated arrays with the initial values for all of the
    //        model's state variables and constants.
    //      - computeComputedConstants(variables) will fill in values for any variables that 
    //        do not change in value throughout the solution, but still need to be calculated;
    //      - computeRates(VOI, states, rates, variables) updates the rates array with the 
    //        gradients of the state variables, given the values of the other variables and the 
    //        variable of integration (VOI);
    //      - computeVariables(VOI, states, rates, variables) updates any non-integrated variables
    //        whose values do not affect the integration.  Since this doesn't affect the solution
    //        process it only needs to be called whenever the values need to be output; not 
    //        necessarily each integration timestep.

    //  3.a 
    //      Create two arrays and use the functions to allocate them.  One will represent the
    //      variables, and one will represent the state variables. 

    //  3.b 
    //      Use the functions provided to initialise the arrays you created, then print them 
    //      to the screen for checking.

    //  3.c 
    //      Compute the computed constants and print them to the screen for checking.

    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "   Step 4: Iterate through the solution                    " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;

    //  This part will make use of a simple routine to step through the solution
    //  iterations using the Euler method to update the state variables.

    //  4.a 
    //      Create variables which control how the solution will run, representing:
    //      - variable of integration (time);
    //      - step size; and
    //      - the number of steps to take.

    //  4.b 
    //      Create an array for the rates.  You can use the same createStatesArray() 
    //      function to allocate this as the number of rates will always equal the 
    //      number of state variables.
   
    //  4.c 
    //      Create a file for output and open it. You can use the information to name columns
    //      with the variables, component, and units so you can keep track later.

    //      The Euler update method is: x[n+1] = x[n] + x'[n]*stepSize
    //      At each step you will need to:
    //          - Compute the variables; **
    //          - Compute the rates;
    //          - Compute the state variables using the update method above; and
    //          - Print to a file.
    //      ** We only need to compute these each timestep here because we're also 
    //         writing the values to the file at each timestep.

    //  4.d 
    //      Iterate through the time domain and write the solution at each step. 

    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "   Step 5: Housekeeping                                    " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;

    //  5.a 
    //  Housekeeping - delete the allocated arrays.
   
}
