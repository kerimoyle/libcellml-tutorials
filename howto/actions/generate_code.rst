.. _actions_generate_code:

Generate runnable code from a model
===================================

Code generation is the process of representing the CellML model in another language format.
At the time of writing, two profiles are available: C (default) and Python.
There are four steps to code generation:

1. Create a :code:`Generator` item and select the profile language.
   (The default profile is C).
2. Pass a model to the generator for processing.
3. Retrieve the generated implementation code. 
   This is the contents of the :code:`*.c` file (if C is the profile) or :code:`*.py` if Python is selected.
4. (optional) Retrieve the generated interface code. 
   This is the contents of the :code:`*.h` file, and is not required for the Python profile.

.. include:: ../code_snippets/snippet_generate_code.rst
