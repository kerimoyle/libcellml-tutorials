.. _solve_model:

Interact with generated code
============================

.. contents::
    :local:
    
Build the generated code
------------------------
The generated code only represents the mathematical formulation of the model, so in order to find its solution it must be connected to an integration algorithm.
This integration is not provided, but some basic functions to interact with the model are.
The generated code must be compiled into whatever algorithm will be used to solve it.

.. include :: ../code_snippets/snippet_build_solver.rst

Initialise the solution variables
---------------------------------

.. include:: ../code_snippets/snippet_initialise_solver.rst



