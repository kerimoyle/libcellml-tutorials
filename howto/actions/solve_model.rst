.. _solve_model:

:orphan:

Interact with generated code
============================

.. contents::
    :local:
    
Interface with the generated code
---------------------------------
The generated code only represents the mathematical formulation of the model, so in order to find its solution it must be connected to an integration algorithm.
This integration is not provided, but some basic functions to interact with the model are.
Depending on whether you're using C++ or Python, the generated code must either be compiled with your solver, or imported as a module.

.. include:: ../code_snippets/snippet_build_solver.rst

Set solution parameters
-----------------------
In this very simple example we take the parameters for the integration step size and number of steps from command line arguments.
Calling the function below at the start of your program retrieves the command line arguments for use at runtime.

.. include:: ../code_snippets/snippet_command_line.rst

The specific parameters related to the model are available within the generated code, and can be retrieved from their structures as shown below.

.. include:: ../code_snippets/snippet_get_parameters.rst

Initialise solution variables
-----------------------------
As well as setting the number and size of the steps, the basic integration algorithm needs to store the state variables and their rates, as well as any other variables that are independent of the integration.

.. include:: ../code_snippets/snippet_allocate_arrays.rst

Note that since the number of state variables must be identical to the number of rates, the same *create states array* function can be used to allocate storage for both of them.

Following allocation, the states and constants can be initialised using the function provided.

Compute the starting values of variables
----------------------------------------
There are two kinds of variables stored in the generated code: those that require updating each step (the rates, but also any other variables which these depend upon); and those which do not affect the progress of the solution (everything else).
The former kind is updated when the *compute rates* function is called, and the latter when the *compute variables* function is called.
Constants clearly need be calculated only once, using the *compute computed constants* function.

In this example we compute everything at the initial point so as to be able to output the starting values to the terminal.

.. include:: ../code_snippets/snippet_initialise_solver.rst

Prepare files for output
------------------------
The solution is to be written directly to an output file during the iteration process, and this step is simply the preparation for that.
The file is created and opened, and the columns labelled with information from the model.

.. include:: ../code_snippets/snippet_create_output.rst

Perform the integration
-----------------------
There are myriad stepping schemes for numerical integration, but here we use the very basic Euler method.
At each step, new rates are calculated by calling the *compute rates* function.
The states are then extrapolated from the rates using the Euler approximation step. 
In this example we are writing the output at every step, so we also need to update the variables too using the *compute variables* function, and everything is written to the output file.

.. include:: ../code_snippets/snippet_solve.rst
