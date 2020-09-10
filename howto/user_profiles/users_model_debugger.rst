.. _users_model_debugger:

Model debugger
==============
This example walks through the process of using the :code:`Validator` and :code:`Analyser` classes to check a CellML model's syntax and mathematical composition.

**Contents**

.. contents::
   :local:

| **C++ resources**
|    ├ :download:`CMakeLists.txt<../code/example_modelDebugger_CMakeLists.txt>`
|    ├ :download:`example_modelDebugger.cpp<../code/example_modelDebugger.cpp>`
|    └ resources/
|        ├ :download:`debugAnalysisExample.cellml<../code/resources/debugAnalysisExample.cellml>`
|        └ :download:`debugAnalysisExampleImport.cellml<../code/resources/debugAnalysisExampleImport.cellml>`

| **Python resources**
|    ├ :download:`example_modelDebugger.py<../code/example_modelDebugger.py>`
|    └ resources/
|        ├ :download:`debugAnalysisExample.cellml<../code/resources/debugAnalysisExample.cellml>`
|        └ :download:`debugAnalysisExampleImport.cellml<../code/resources/debugAnalysisExampleImport.cellml>`

Parse an existing CellML model 
------------------------------
Here we use the :code:`Parser` class to read the file, and deserialise the CellML into a :code:`Model` object.
The CellML file used in this example makes use of imports, so we need to have the main file (debugAnalysisExample.cellml) as well as the file which it imports (debugAnalysisExampleImport.cellml).

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_modelDebugger.cpp
        :language: c++
        :start-after: // STEP 1
        :end-before: // STEP 2

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_modelDebugger.py
        :language: python
        :start-after: # STEP 1
        :end-before: # STEP 2

Resolve the imports and flatten the model
-----------------------------------------
At present the :code:`Analyser` class ignores the contents of imported items.  
This example shows a model which has imports, so we need to resolve these and flatten the model before any analysis can happen.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_modelDebugger.cpp
        :language: c++
        :start-after: // STEP 2
        :end-before: // STEP 3

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_modelDebugger.py
        :language: python
        :start-after: # STEP 2
        :end-before: # STEP 3

Validate the model
------------------
The :code:`Validator` class process is like a spelling checker: it will check the syntax of the model ahead of analysing its mathematical formulation in the :code:`Analyser` later.
Here we create a validator, use it to check the model, and retrieve the descriptions of any issues it found.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_modelDebugger.cpp
        :language: c++
        :start-after: // STEP 3
        :end-before: // STEP 4

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_modelDebugger.py
        :language: python
        :start-after: # STEP 3
        :end-before: # STEP 4

The validation issues raised can be used to fix any "spelling" problems with the model.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_modelDebugger.cpp
        :language: c++
        :start-after: // STEP 4
        :end-before: // STEP 5

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_modelDebugger.py
        :language: python
        :start-after: # STEP 4
        :end-before: # STEP 5

Analyse the model
-----------------
If the :code:`Validator` is the spell-checker then the :code:`Analyser` is the grammar-checker.
It will check for errors of logic or mathematical definition in the model formulation, and also make sure that all of the information which a solver needs is available.
The :code:`Analyser` works in the same way as the other service classes: we create an instance, pass a model for checking, and then retrieve any issues it may have found.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_modelDebugger.cpp
        :language: c++
        :start-after: // STEP 5
        :end-before: // STEP 6

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_modelDebugger.py
        :language: python
        :start-after: # STEP 5
        :end-before: # STEP 6

.. container:: gotcha

    In some situations both the :code:`Validator` and :code:`Analyser` classes may encounter errors that mean they're unable to continue processing the model. 
    For this reason, you may need several iterations of checking and fixing before all of the issues are addressed.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_modelDebugger.cpp
        :language: c++
        :start-after: // STEP 6
        :end-before: // STEP 7

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_modelDebugger.py
        :language: python
        :start-after: # STEP 6
        :end-before: # STEP 7

Print the repaired model to a CellML file
-----------------------------------------
Finally, we can serialised the repaired model for output to a CellML file using the :code:`Printer` class.
Note that this prints the flattened model, so will not contain the import dependencies of the original one.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_modelDebugger.cpp
        :language: c++
        :start-after: // STEP 7
        :end-before: // END

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_modelDebugger.py
        :language: python
        :start-after: # STEP 7
        :end-before: # END

