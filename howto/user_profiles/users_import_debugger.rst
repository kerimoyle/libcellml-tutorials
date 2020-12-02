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

.. code-block:: text

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

.. code-block:: text

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

Validate the import dependencies
--------------------------------
The issues reported by the :code:`Validator` and :code:`Analyser` items in the previous step have been hidden from the original model, as they exist somewhere in its hierarchy of imported items.
We can make use of the library of import dependencies which is stored in the :code:`Importer` class to locate the specific models in which the issues occurred, and repair them.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // STEP 4
        :end-before: // STEP 5     

    .. tab:: Python 

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # STEP 4
        :end-before: # STEP 5

The code above will print the following to the terminal:

.. code-block:: text

    The validator found 0 issues in resources/importExample2a.cellml
    The validator found 0 issues in resources/importExample2b.cellml
    The validator found 2 issues in resources/importExample3.cellml
        - CellML identifiers must contain one or more basic Latin alphabetic characters.
        - Variable 'iNeedUnits' does not have a valid units attribute.

Fix the errors in the imported model
------------------------------------
Now that we've found where the error is (a variable named "iNeedUnits" within a component which is imported from a file at URL "resources/importExample3.cellml"), it's now time to fix it.
This could be done by parsing that file directly and amending it as needed, but since we already have a copy of this model inside the :code:`Importer` class, it's better to use that one.
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

Analyse the import dependencies
-------------------------------
This step repeats the two previous steps but uses the :code:`Analyser` class instead of the :code:`Validator` class to report on issues.
We can then fix the issues found in the same way, by accessing the library's version of the imported model and repairing the problem there.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // STEP 6
        :end-before: // STEP 7

    .. tab:: Python 

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # STEP 6
        :end-before: # STEP 7

Output the repaired models to CellML files
------------------------------------------
The final step in this process is to output the repaired models to CellML files.
The ability to use imports to combine model items is useful, so we want to retain that in the fixed models.  
For this reason, we will write all of the files involved - whether altered or not - to a new directory.
By doing this, we maintain the same relationship between the model files as was there in the beginning.

.. container:: gotcha

  For this example you will need to have created a directory structure ahead of time since the functions used here do not create those directories.  
  You will need to create two nested directories: :code:`repaired/resources` within your working directory before continuing.

.. tabs::

    .. tab:: C++ 

      .. literalinclude:: ../code/example_importDebugger.cpp
        :language: c++
        :start-after: // STEP 7
        :end-before: // END

    .. tab:: Python 

      .. literalinclude:: ../code/example_importDebugger.py
        :language: python
        :start-after: # STEP 7
        :end-before: # END