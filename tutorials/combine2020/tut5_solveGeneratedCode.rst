..  _combine_solveGeneratedCode:

Tutorial 5: Interact with generated code
========================================

.. container:: shortlist

    By the time you have worked through this tutorial you will be able to:

    - Investigate and understand the contents of files created by the Generator; and
    - Integrate generated code into a simple solver to run a simulation.

    This tutorial assumes that you are comfortable with:

    - Interacting with a model and its entities using the API;
    - Using the :code:`Generator` functionality to output files in C or Python; and
    - The basic idea of numerical integration using Euler's method.

.. container:: shortlist

    **Code (C++)**

    - :download:`CMakeLists.txt<code/tut5/CMakeLists.txt>` The CMake file for building this tutorial;
    - :download:`solveGeneratedCode.cpp<code/tut5/solveGeneratedCode.cpp>` Either the skeleton code, or ..
    - :download:`solveGeneratedCode_completed.cpp<code/tut5/solveGeneratedCode_completed.cpp>` the completed tutorial code;
    - :download:`HodgkinHuxleyModel.cpp<code/tut5/HodgkinHuxleyModel.cpp>` Generated implementation code from Tutorial 4; and
    - :download:`HodgkinHuxleyModel.h<code/tut5/HodgkinHuxleyModel.h>` Generated interface code from Tutorial 4.

.. container:: shortlist

    **Code (Python)**

    - :download:`solveGeneratedCode.py<code/tut5/solveGeneratedCode.py>` Either the skeleton code, or ..
    - :download:`solveGeneratedCode_completed.py<code/tut5/solveGeneratedCode_completed.py>` the completed tutorial code;
    - :download:`HodgkinHuxleyModel.py<code/tut5/HodgkinHuxleyModel.py>` Generated implementation code from Tutorial 4

.. contents:: Contents
    :local:

Step 1: Link to the generated code
----------------------------------
The first step in interacting with the output from the code generation is including the generated files in the project.       
There are necessarily big differences between C++ and Python for this tutorial!

.. tabs:: 

    .. tab:: C++ instructions

        .. container:: dothis
        
            **1.a** Enter the path to the generated header/interface *.h file in the #include block above.

        .. container:: dothis
        
            **1.b** Add the name and path of the implementation *.cpp file in the CMakeLists.txt file, (or whatever your local toolchain requires).

        .. container:: dothis
        
            **1.c** Open the implementation file *.cpp file and verify that the #include statement in line 3 has the filename of your interface *.h file.  
            Amend if needed and close the file.

        .. container:: dothis

            **1.d** Call cmake to create the Makefile.
            Call make -j to build the executable.
            Run the code so far to check that the libCellML versions match.

        .. container:: toggle

            .. container:: header

                Show C++ snippet

            .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
                :language: c++
                :start-at: //  1.a
                :end-before: //  end 1

    .. tab:: Python instructions

        .. container:: dothis

            **1.a** Use the importlib functionality to open the generated code file.

        .. container:: dothis

            **1.b** Load into a module.
        
        .. container:: toggle

            .. container:: header

                Show Python snippet

            .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
                :language: python
                :start-at: #  1.a
                :end-before: #  end 1

Step 2: Access the variables in the generated files
---------------------------------------------------
Probably the best way to understand the contents of the generated files is o open them and look!
The implementation file (*.cpp) has two types of items:

- information structures (in all-caps); and
- access functions.

It's important to remember that in the generated code we don't have the notion of separate components: they are listed here with the variables only in order to give the correct context to the variable names.

"Variables" are anything which does not require integration as part of the solution, and could have types :code:`COMPUTED_CONSTANT` (needs to be calculated but doesn't need integration), :code:`CONSTANT` (no calculation needed), or :code:`ALGEBRAIC` (**TODO**) as defined in the :code:`VariableType` enum.
They are stored in an array of :code:`VariableInfoWithType` structs called :code:`VARIABLE_INFO` which is :code:`VARIABLE_COUNT` long.  

.. container:: shortlist

    The :code:`VariableInfoWithType` contains:

    - name,
    - units,
    - component, and
    - VariableType.

.. container:: dothis

    **2.a** Get the number of variables and iterate through the :code:`VARIABLE_INFO` structure to retrieve and print each variable's information to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: //  end 2.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
        :language: python
        :start-at: #  2.a
        :end-before: #  end 2.a

"State variables" are those which need integration.
They are stored in an array of :code:`VariableInfo` structs called :code:`STATE_INFO` which
is :code:`STATE_COUNT` long.  

.. container:: shortlist

    The :code:`VariableInfo` struct contains:

    - name,
    - units, and
    - component.

.. container:: dothis

    **2.b** Get the number of state variables and iterate through the :code:`STATE_INFO` structure to retrieve and print each state variable's information to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
        :language: c++
        :start-at: //  2.b
        :end-before: //  2.c 

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
        :language: python
        :start-at: #  2.b
        :end-before: #  2.c

.. container:: dothis

    **2.c** Get the integration variable and print its information to the terminal.
    This is stored in a :code:`VariableInfo` struct called :code:`VOI_INFO`.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
        :language: c++
        :start-at: //  2.c
        :end-before: //  end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
        :language: python
        :start-at: #  2.c
        :end-before: #  end 2

Step 3: Access the functions in the generated files
---------------------------------------------------

.. tabs::

    .. tab:: C++

        The generated code contains seven functions:

        - :code:`createStatesArray()` to allocate an array of length :code:`STATE_COUNT`.
          This can be used to allocate the "rates" or gradient function array too as they're the same length;
        - :code:`createVariablesArray()` to allocate an array of length :code:`VARIABLE_COUNT`;
        - :code:`deleteArray()` to free memory used by the given array;
        - :code:`initialiseStatesAndConstants(states, variables)` will do what it says on the tin, and populate the given pre-allocated arrays with the initial values for all of the model's state variables and constants.
        - :code:`computeComputedConstants(variables)` will fill in values for any variables that do not change in value throughout the solution, but still need to be calculated;
        - :code:`computeRates(VOI, states, rates, variables)` updates the rates array with the gradients of the state variables, given the values of the other variables and the variable of integration (VOI);
        - :code:`computeVariables(VOI, states, rates, variables)` updates any non-integrated variables whose values do not affect the integration.
          Since this doesn't affect the solution process it only needs to be called whenever the values need to be output; not necessarily each integration timestep.

    .. tab:: Python

        The generated code contains seven functions:

        - :code:`create_states_array()` to allocate an array of length :code:`STATE_COUNT`.
          This can be used to allocate the "rates" or gradient function array too as they're the same length;
        - :code:`create_variables_array()` to allocate an array of length :code:`VARIABLE_COUNT`;
        - :code:`delete_array()` to free memory used by the given array;
        - :code:`initialise_states_and_constants(states, variables)` will do what it says on the tin, and populate the given pre-allocated arrays with the initial values for all of the model's state variables and constants.
        - :code:`compute_computed_constants(variables)` will fill in values for any variables that do not change in value throughout the solution, but still need to be calculated;
        - :code:`compute_rates(VOI, states, rates, variables)` updates the rates array with the gradients of the state variables, given the values of the other variables and the variable of integration (VOI);
        - :code:`compute_variables(VOI, states, rates, variables)` updates any non-integrated variables whose values do not affect the integration.
          Since this doesn't affect the solution process it only needs to be called whenever the values need to be output; not necessarily each integration timestep.

.. container:: dothis

    **3.a** Create three arrays representing:
    
    - the variables (which here includes constants)
    - the states (the integrated variables)
    - the rates 
    Create and initialise a variable of integration, time. 

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: //  3.b 

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
        :language: python
        :start-at: #  3.a
        :end-before: #  3.b

.. container:: dothis

    **3.b** Use the functions provided to initialise the states array you created, then print them to the screen for checking.
    
.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
        :language: c++
        :start-at: //  3.b
        :end-before: //  3.c 

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
        :language: python
        :start-at: #  3.b
        :end-before: #  3.c

.. container:: dothis

    **3.c** Compute the constants, compute the variables, and print them to the screen for checking.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
        :language: c++
        :start-at: //  3.c
        :end-before: //  end 3

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
        :language: python
        :start-at: #  3.c
        :end-before: #  end 3

Step 4: Iterate through the solution
------------------------------------
This part will make use of a simple routine to step through the solution iterations using the Euler method to update the state variables.

.. container:: dothis

    **4.a** Create variables which control how the solution will run, representing:

    - step size; and
    - the number of steps to take.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
        :language: c++
        :start-at: //  4.a
        :end-before: //  4.b 

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
        :language: python
        :start-at: #  4.a
        :end-before: #  4.b

.. container:: dothis

    **4.b** Create a file for output and open it.
    You can use the information to name columns with the variables, component, and units so you can keep track later.

The Euler update method is: :math:`x[n+1] = x[n] + x'[n].dx`

.. container:: shortlist

    At each step you will need to:
        - Compute the rates;
        - Compute the state variables using the update method above; 
        - Compute the variables; **
        - Print to a file.
    ** We only need to compute these each timestep here because we're also writing the values to the file at each timestep.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
        :language: c++
        :start-at: //  4.b
        :end-before: //  end 4.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
        :language: python
        :start-at: #  4.b
        :end-before: #  end 4.b

.. container:: dothis

    **4.c** Iterate through the time domain, calculate and write the solution at each step.
    
.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.cpp
        :language: c++
        :start-at: //  4.c
        :end-before: //  end 4

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut5/solveGeneratedCode_completed.py
        :language: python
        :start-at: #  4.c
        :end-before: #  end 4

Step 5: Sanity check
--------------------
The file that results from running the completed version of this tutorial can be downloaded from :download:`HodgkinHuxleyModelSolution.txt<code/resources/HodgkinHuxleyModelSolution.txt>`, and is a tab-delimited file that can be easily read into your favourite plotting program.

