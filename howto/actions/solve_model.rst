.. _solve_model:

Interact with generated code
============================

.. contents::
    :local:
    
Interface with the generated code
---------------------------------
The generated code only represents the mathematical formulation of the model, so in order to find its solution it must be connected to an integration algorithm.
This integration is not provided, but some basic functions to interact with the model are.
Depending on whether you're using C++ or Python, the generated code must either be compiled with your solver, or imported as a module.

.. include :: ../code_snippets/snippet_build_solver.rst

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

.. include:: ../code_snippets/snippet_initialise_variables.rst



