.. _users_simulation_tool_dev:

Simulation tool developer
=========================
The code generation functionality in libCellML can be used to transform CellML models into procedural code for simulation.
This example works through that process. 

**Contents**

.. contents::
   :local:

| **C++ resources**
|    ├ :download:`CMakeLists.txt<../code/simulation_tool_dev/CMakeLists.txt>`
|    ├ :download:`example_simulationToolDev.cpp<../code/example_simulationToolDev.cpp>`
|    └ resources/
|        └ :download:`simulationExample.cellml<../code/resources/simulationExample.cellml>`

| **Python resources**
|    ├ :download:`example_simulationToolDev.py<../code/example_simulationToolDev.py>`
|    └ resources/
|        └ :download:`simulationExample.cellml<../code/resources/simulationExample.cellml>`

Parse an existing model from a file
-----------------------------------
The following code will read a file called :code:`simulationExample.cellml` and deserialise its contents into a :code:`Model` instance.
Note that both the :code:`Parser` class and its opposite number, the :code:`Printer` class, deal with strings rather than files.
You'll need to read the file into a string, and then use the string as input to the :code:`Parser` item.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_simulationToolDev.cpp
        :language: c++
        :start-after: // STEP 1
        :end-before: // STEP 2

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_simulationToolDev.py
        :language: python
        :start-after: # STEP 1
        :end-before: # STEP 2
      
Resolve imports and flatten model
---------------------------------
The import functionality for :code:`Units` and :code:`Component` items is key to enabling the reuse and sharing of models.
The import statements are basically a recipe for how these imported items can be combined to make the present model.
While models which contain import dependencies are perfectly valid, they cannot be used to generate runnable code.
The process of resolving the imports (telling libCellML where to look for these ingredients) and flattening the model (creating instances of the ingredients and removing the dependency) is necessary before code generation can happen.

The :code:`Importer` class supports all functionality to do with imports, and contains its own logger which can be used to report anything that might have gone wrong. 

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_simulationToolDev.cpp
        :language: c++
        :start-after: // STEP 2
        :end-before: // STEP 3

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_simulationToolDev.py
        :language: python
        :start-after: # STEP 2
        :end-before: # STEP 3

Validate model
--------------
The :code:`Validator` is the equivalent of a spelling checker: it can check that each item in a model has all of the information it needs, but it can't check whether it means what you intend it to.
Thus even if a model is valid, it could still be the equivalent of correctly-spelled nonsense.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_simulationToolDev.cpp
        :language: c++
        :start-after: // STEP 3
        :end-before: // STEP 4

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_simulationToolDev.py
        :language: python
        :start-after: # STEP 3
        :end-before: # STEP 4

Once a model has been passed to a :code:`Validator` instance, the validator's internal logger will contain a list of any of the issues which have been encountered during the checking process. 
A model can be said to be valid - that is, conforming to the :cellml2:`CellML normative specification <>` - if the validator's logger contains no issues with a level of :code:`ERROR`.

For more information on how to use any of the classes which record issues, please see the :ref:`Get Issues<actions_get_issues>` section.

Generate code
-------------
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

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_simulationToolDev.cpp
        :language: c++
        :start-after: // STEP 4
        :end-before: // STEP 5

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_simulationToolDev.py
        :language: python
        :start-after: # STEP 4
        :end-before: # STEP 5

Use the generated code to simulate the model
--------------------------------------------
Instructions for a simple solver to solve the model can be found on the :ref:`Simulation tool developer: Solver<users_solver>` page.
