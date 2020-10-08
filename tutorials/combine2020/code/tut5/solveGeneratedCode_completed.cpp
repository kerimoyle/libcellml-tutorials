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

#include <libcellml>

#include "tutorial_utilities.h"

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

    //  1.d Check that we can access the version string in both places: the libCellML library 
    //      and the generated code.
    std::cout << "The generated code used libCellML version " << LIBCELLML_VERSION << std::endl;
    std::cout << "The library version of libCellML is " << libcellml::versionString() << std::endl 
              << std::endl;

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

    std::cout << "VARIABLE_COUNT = " << VARIABLE_COUNT << std::endl;
    for (size_t v = 0; v < VARIABLE_COUNT; ++v) {
        std::cout << "Variable " << v << ": " << std::endl;
        std::cout << "  name = " << VARIABLE_INFO[v].name << std::endl;
        std::cout << "  units = " << VARIABLE_INFO[v].units << std::endl;
        std::cout << "  component = " << VARIABLE_INFO[v].component << std::endl;
        std::cout << "  type = " << VARIABLE_INFO[v].type << std::endl;
    }

    //  end 2.a

    //      "State variables" are those which need integration.
    //      They are stored in an array of VariableInfo structs called STATE_INFO which
    //      is STATE_COUNT long.  The VariableInfo struct contains:
    //          - name,
    //          - units, and
    //          - component.

    //  2.b
    //      Get the number of state variables and iterate through the STATE_INFO structure to
    //      retrieve and print each state variable's information to the terminal.
    std::cout << std::endl;
    std::cout << "STATE_COUNT = " << STATE_COUNT << std::endl;
    for (size_t s = 0; s < STATE_COUNT; ++s) {
        std::cout << "State variable " << s << ": " << std::endl;
        std::cout << "  name = " << STATE_INFO[s].name << std::endl;
        std::cout << "  units = " << STATE_INFO[s].units << std::endl;
        std::cout << "  component = " << STATE_INFO[s].component << std::endl;
    }

    //  2.c 
    //      Get the integration variable and print its information to the terminal. This
    //      is stored in a VariableInfo struct called VOI_INFO.
    std::cout << std::endl;
    std::cout << "VOI_INFO" << std::endl;
    std::cout << "  name = " << VOI_INFO.name << std::endl;
    std::cout << "  units = " << VOI_INFO.units << std::endl;
    std::cout << "  component = " << VOI_INFO.component << std::endl;
    std::cout << std::endl;

    //  end 2

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
    auto myVariables = createVariablesArray();
    auto myStateVariables = createStatesArray();

    //  3.b 
    //      Use the functions provided to initialise the arrays you created, then print them 
    //      to the screen for checking.
    initialiseStatesAndConstants(myStateVariables, myVariables);

    std::cout << "The initial conditions for variables are:" << std::endl;
    for (size_t v = 0; v < VARIABLE_COUNT; ++v) {
        std::cout << "  " << VARIABLE_INFO[v].name << " = " << myVariables[v] << " (" << VARIABLE_INFO[v].units << ")"<<std::endl;
    }
    std::cout << std::endl;

    std::cout << "The initial conditions for state variables are:" << std::endl;
    for (size_t v = 0; v < STATE_COUNT; ++v) {
        std::cout << "  " << STATE_INFO[v].name << " = " << myStateVariables[v] << " (" << STATE_INFO[v].units << ")"<< std::endl;
    }
    std::cout << std::endl;

    //  3.c 
    //      Compute the computed constants and print them to the screen for checking.
    std::cout << "The initial values including all computed constants are:" << std::endl;
    computeComputedConstants(myVariables);
    for (size_t v = 0; v < VARIABLE_COUNT; ++v) {
        std::cout << "  " << VARIABLE_INFO[v].name << " = " << myVariables[v] << " (" << VARIABLE_INFO[v].units << ")"<<std::endl;
    }
    std::cout << std::endl;

    //  end 3

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
    double time = 0.0;
    double stepSize = 0.01;
    int stepCount = 2000;
    int incr = (int)(stepCount/60) + 1;

    //  4.b 
    //      Create an array for the rates.  You can use the same createStatesArray() 
    //      function to allocate this as the number of rates will always equal the 
    //      number of state variables.
    auto myRates = createStatesArray();

    //  4.c 
    //      Create a file for output and open it. You can use the information to name columns
    //      with the variables, component, and units so you can keep track later.
    std::ofstream outFile("HodgkinHuxleyModelSolution.txt");
    outFile << "iteration";
    outFile << "\t" << VOI_INFO.name << " (" << VOI_INFO.units << ")";
    for (size_t v = 0; v < VARIABLE_COUNT; ++v) {
        outFile << "\t" << VARIABLE_INFO[v].component<<":"<<VARIABLE_INFO[v].name << " (" << VARIABLE_INFO[v].units << ")";
    }
    for (size_t s = 0; s < STATE_COUNT; ++s) {
        outFile << "\t" << STATE_INFO[s].component<<":"<<STATE_INFO[s].name << " (" << STATE_INFO[s].units << ")";
    }
    outFile << std::endl;
   
    //  end 4.c
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
    for (size_t step = 0; step < stepCount; ++step) {
        time = step * stepSize;

        // Compute the variables at this step using the given function.
        computeVariables(time, myStateVariables, myRates, myVariables);

        // Compute the rates at this step using the given function.
        computeRates(time, myStateVariables, myRates, myVariables);
        
        // Compute the solution at the next step using whatever numerical integration 
        // method you choose. In this example we've used Euler, as given above.
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            myStateVariables[s] = myStateVariables[s] + myRates[s] * stepSize;
        }

        // Write everything to the output file.  Keep the order of columns consistent with
        // whatever you've used in step 4.c.
        outFile << step << "\t " << time;
        for(size_t v = 0; v < VARIABLE_COUNT; ++v){
            outFile << myVariables[v] << "\t ";
        }
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            outFile << "\t" << myStateVariables[s];
        }
        outFile << "\n";
        if(step % incr == 0) {
            std::cout << "." << std::flush;
        }
    }
    std::cout << std::endl << "Finished!" << std::endl;
    outFile.close();

    //  end 4

    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "   Step 5: Housekeeping                                    " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;

    //  5.a 
    //  Housekeeping - delete the allocated arrays.
    deleteArray(myStateVariables);
    deleteArray(myVariables);
    deleteArray(myRates);

    //  end 5

    std::cout << "The results have been written to 'HodgkinHuxleyModelSolution.txt'" << std::endl;
}
