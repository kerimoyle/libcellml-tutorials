.. _users_importer_debugger:

Import debugger
===============
This example walks through the processes involved with debugging models using the :code:`Importer` class in conjunction with the diagnostic classes, :code:`Validator` and :code:`Analyser`.
For more general debugging use cases, please see the :ref:`Model debugger<users_model_debugger>` example instead.

**Contents**

.. contents::
   :local:

| **C++ resources**
|   ├ :download:`CMakeLists.txt<../code/example_importDebugger_CMakeLists.txt>`
|   ├ :download:`example_importDebugger.cpp<../code/example_importDebugger.cpp>`
|   └ resources/
|       ├ :download:`importerExample1.cellml<../code/resources/importerExample1.cellml>`
|       ├ :download:`importerExample2a.cellml<../code/resources/importerExample2a.cellml>`
|       ├ :download:`importerExample2b.cellml<../code/resources/importerExample2b.cellml>`
|       └ :download:`importerExample3.cellml<../code/resources/importerExample3.cellml>`

| **Python resources**
|   ├ :download:`example_importDebugger.py<../code/example_importDebugger.py>`
|   └ resources/
|       ├ :download:`importerExample1.cellml<../code/resources/importerExample1.cellml>`
|       ├ :download:`importerExample2a.cellml<../code/resources/importerExample2a.cellml>`
|       ├ :download:`importerExample2b.cellml<../code/resources/importerExample2b.cellml>`
|       └ :download:`importerExample3.cellml<../code/resources/importerExample3.cellml>`


Parse an existing CellML model 
------------------------------
The first step is to parse and instantiate a model from a CellML file.
Here we use the :code:`Parser` class to read the file, and deserialise the CellML into a :code:`Model` object.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // STEP 1
        :end-before: // STEP 2

    .. tab:: Python 

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # STEP 1
        :end-before: # STEP 2

Create an Importer and resolve the model's imports 
--------------------------------------------------
The :code:`Importer` class is a utility which can be used to handle models which import components or units from other models.
Passing the model and a base directory location will enable the :code:`Importer` to resolve any imports required by the model, and add those dependencies to the importer's internal library.
Since the :code:`Importer` class contains a logger, we can check that the process has completed properly by printing any issues inside the importer to the terminal.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // STEP 2
        :end-before: // STEP 3

    .. tab:: Python 

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # STEP 2
        :end-before: # STEP 3

The code above prints to the terminal:

.. code-block:: terminal

    The importer found 0 issues.

Validate and analyse the model
------------------------------
The analysis tools - the :code:`Validator` and :code:`Analyser` - will read only the highest level of the model they are given; they do not look into any of the imported items, so they can't check them.
In order to retain the import structure but be able to use the diagnostic tools, we can create a flattened copy of the model for testing.
This can be used to identify mistakes in the original version of the model.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // STEP 3
        :end-before: // STEP 4

    .. tab:: Python 

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # STEP 3
        :end-before: # STEP 4

.. code-block:: terminal

    Investigating the original model:
    The validator found 0 issues.
    The analyser found 0 issues.

    Investigating the flattened model:
    The validator found 4 issues.
      - CellML identifiers must contain one or more basic Latin alphabetic characters.
      - Variable 'i_need_units' does not have a valid units attribute.
      - CellML identifiers must contain one or more basic Latin alphabetic characters.
      - Variable 'i_need_units' does not have a valid units attribute.
    The analyser found 4 issues.
      - CellML identifiers must contain one or more basic Latin alphabetic characters.
      - Variable 'i_need_units' does not have a valid units attribute.
      - CellML identifiers must contain one or more basic Latin alphabetic characters.
      - Variable 'i_need_units' does not have a valid units attribute.

Print the model's import dependencies
-------------------------------------
The issues reported by the :code:`Validator` and :code:`Analyser` items in the previous step are really one single issue: that somewhere in the import hierarchy is a variable named "i_need_units" which doesn't have any units specified.
We need to create a recursive call to traverse the import hierarchy so that we can locate the model in which this variable is located.

.. tabs::

    .. tab:: C++ 

      Call a recursive function from the main function.

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // STEP 4
        :end-before: // STEP 5

      Define the importing function recursion.
      This, and the following snippet, should occur above the main function. 

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // START IMPORT FUNCTION
        :end-before: // END IMPORT FUNCTION
      
      Simple function to print variables within a component.

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // START PRINT COMPONENT
        :end-before: // END PRINT COMPONENT
      

    .. tab:: Python 

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # STEP 4
        :end-before: # STEP 5

      Define the importing function recursion.
      This, and the following snippet, should occur above the main function.

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # START IMPORT FUNCTION
        :end-before: # END IMPORT FUNCTION
      
      Simple function to print variables within a component.

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # START PRINT COMPONENT
        :end-before: # END PRINT COMPONENT

The code above will print the following to the terminal:

.. code-block:: terminal

    The import dependency hierarchy is:
    - Component: 'importedSideA' imports 'sideA' from importExample2a.cellml
        - Component: 'sideA' imports 'shared' from importExample3.cellml
            - Component: 'shared'
                - Variable: 'i_need_units' has no units
    - Component: 'importedSideB' imports 'sideB' from importExample2b.cellml
        - Component: 'sideB' imports 'shared' from importExample3.cellml
            - Component: 'shared'
                - Variable: 'i_need_units' has no units

Fix the errors in the imported model
------------------------------------
Now that we've found where the error is (a variable named "i_need_units" within a component named "shared" which is imported from a file at URL "resources/importExample3.cellml"), it's now time to fix it.
This could be done by parsing that file directly and amending it as needed, but since we already have a copy of this model inside the :code:`Importer` class, it's easier to use that one.
When the importer resolves imports, any dependencies it needs are instantiated inside its library.
These models can be accessed within the library by their key (the URL from which they were read) or by their index.
Here we print the list of the importer's library keys to demonstrate, and then retrieve the model containing the error so it can be fixed.

.. container:: gotcha

    Note that altering a model instance returned from the :code:`Importer` changes the model instance which is used to resolve the original model's dependencies. 
    The :code:`Importer` checks its own library first before opening and parsing external files, so once we've fixed the error in the library's model, re-flattening the original model uses the fixed version too.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // STEP 5
        :end-before: // STEP 6

    .. tab:: Python 

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # STEP 5
        :end-before: # STEP 6

Print the repaired model to a CellML file
-----------------------------------------
Finally, we can print the repaired version of the "importExample3.cellml" model to another file.
Note that in normal circumstances you would need to over-write the file with the fixed one, in order that it can be located by models which import it.
In this case - and for demonstration purposes only - we print it to a separate name, so that running this example code will give the same results each time.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // STEP 6
        :end-before: // END

    .. tab:: Python 

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # STEP 6
        :end-before: # END
