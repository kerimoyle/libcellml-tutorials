.. _users_solver:


Simulation tool developer: Solver
=================================
The code generation functionality in libCellML can be used to transform CellML models into procedural code for simulation.
This example works through that process. 

**Contents**

.. contents::
   :local:

| **C++ resources**
|    ├ :download:`CMakeLists.txt<../code/example_solveGeneratedModel_CMakeLists.txt>`
|    ├ :download:`example_solveGeneratedModel.cpp<../code/example_solveGeneratedModel.cpp>`
|    └ resources/
|        └ :download:`example_solveGeneratedModel.cellml<../code/resources/simulationExample.cellml>`

| **Python resources**
|    ├ :download:`example_solveGeneratedModel.py<../code/example_solveGeneratedModel.py>`
|    └ resources/
|        └ :download:`simulationExample.cellml<../code/resources/simulationExample.cellml>`

Create a placeholder for the solver
-----------------------------------
Interacting with procedural code means that we need a program, so the first step is to create what will soon become our solver.
To start with, the program only reads and interprets command line arguments; the size and number of timesteps to simulate.
This is done using a simple function, as below.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_solveGeneratedModel.cpp
        :language: c++
        :start-after: // COMMAND LINE FUNCTION
        :end-before: // END COMMAND LINE FUNCTION

    .. tab:: Python 

      .. literalinclude:: ../code/example_solveGeneratedModel.py
        :language: python
        :start-after: # MODULE FROM FILE
        :end-before: # END MODULE FROM FILE

Once the function exists, we can call it from the main function to process the command line arguments.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_solveGeneratedModel.cpp
        :language: c++
        :start-after: // STEP 0 
        :end-before: // STEP 1 

    .. tab:: Python 

      .. literalinclude:: ../code/example_solveGeneratedModel.py
        :language: python
        :start-after: # STEP 0
        :end-before: # STEP 1

Connect with the generated code
-------------------------------
The profile language will affect how you need to interact with the generated code.

For Python, the generated code must be converted into a module, and then imported.
This is shown in the example code below.  

For C it's a little more complicated, as you will need to compile the solver whilst including the generated interface :code:`*.h` file and linking to the implementation :code:`*.c` file.
A CMake file is provided which will do this for you.
The build process needs to know the name of the implementation file containing the generated code, so this is given as the :code:`-DINPUT=yourBaseFileName` argument (without extension).
This file and the corresponding interface :code:`.h` file are copied by CMake to two files named :code:`modelToSolve.cpp` and :code:`modelToSolve.h` respectively.
This step is required so that within the solver code we can :code:`#include` a known file name.

.. tabs::

    .. tab:: C building and linking

        Navigate into the folder containing 
        .. code-block:: terminal
        
            cmake -DINPUT=example_solveGeneratedModel .

        .. code-block:: terminal

            -- The C compiler identification is AppleClang 10.0.1.10010046
            -- The CXX compiler identification is AppleClang 10.0.1.10010046
            -- Check for working C compiler: /Library/Developer/CommandLineTools/usr/bin/cc
            -- Check for working C compiler: /Library/Developer/CommandLineTools/usr/bin/cc -- works
            -- Detecting C compiler ABI info
            -- Detecting C compiler ABI info - done
            -- Detecting C compile features
            -- Detecting C compile features - done
            -- Check for working CXX compiler: /Library/Developer/CommandLineTools/usr/bin/c++
            -- Check for working CXX compiler: /Library/Developer/CommandLineTools/usr/bin/c++ -- works
            -- Detecting CXX compiler ABI info
            -- Detecting CXX compiler ABI info - done
            -- Detecting CXX compile features
            -- Detecting CXX compile features - done

            1) First use 'make -j' to build the executable
            2) Then solve by running: ./solve_example_solveGeneratedModel with the arguments:
            -n  step_total
            -dt step_size

            -- Configuring done
            -- Generating done
            -- Build files have been written to: your/file/location/here

    .. tab:: Python 

      .. literalinclude:: ../code/example_solveGeneratedModel.py
        :language: python
        :start-after: # MODULE FROM FILE
        :end-before: # END MODULE FROM FILE

Retrieve the generated model information
----------------------------------------
The generated code contains model information in dictionaries, as well as functions to interface with the model's mathematics.
These are:

- :code:`VOI_INFO`: a dictionary with the :code:`.name`, :code:`.units`, and :code:`.component` attributes related to the variable of integration,
- :code:`STATE_INFO`: a list of similar information for the state variables,
- :code:`VARIABLE_INFO`: a list of similar information for the non-state variables. 

.. tabs::

    .. tab:: Python 

      .. literalinclude:: ../code/example_solveGeneratedModel.cpp
        :language: c++
        :start-after: // STEP 1
        :end-before: // STEP 2

    .. tab:: Python 

      .. literalinclude:: ../code/example_solveGeneratedModel.py
        :language: python
        :start-after: # STEP 1
        :end-before: # STEP 2

