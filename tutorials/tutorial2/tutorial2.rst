.. _tutorial2:

Tutorial 2: Debugging, issue checking and validating
====================================================
In this tutorial you'll explore how the :code:`Validator` object can be put to use as you create and interact with your models to help you debug and find issues ahead of submitting it for simulation.
You'll be taking a CellML2.0 file which has some mistakes in it, using the validation functionality to identify them, and then using the access and editing functions to fix them.

.. container:: shortlist

  By the time you have worked through Tutorial 2 you will be able to:

  - Use the :code:`Parser` to report issues encountered when reading a file or deserialising a string; and
  - Use the :code:`Validator` to check for issues related to a model's description as compared to the CellML2.0 specifications.

.. container:: shortlist

  This tutorial assumes that you can already:

  - Read and deserialise a CellML model from a file (as in :ref:`Tutorial 1<tutorial2>`);
  - Retrieve the name and id of models, components, and variables;
  - Navigate through the hierarchy of the :code:`Model` and its subsidiary elements (:code:`Components`, :code:`Variables`, :code:`Units`, and MathML blocks); and
  - Serialise and print a :code:`Model` structure to a CellML file.

.. container:: shortlist

    **Requirements (C++)**
    
    :download:`The whole folder<tutorial2.zip>` Download the whole folder, or:

    - :download:`CMakeLists.txt<CMakeLists.txt>` The CMake file for building;
    - :download:`tutorial2.cpp<tutorial2.cpp>` The skeleton code or
    - :download:`tutorial2_complete.cpp<tutorial2_complete.cpp>` the completed code; and
    - :download:`tutorial2.cellml<tutorial2.cellml>` the input CellML file.

.. container:: shortlist

    **Requirements (Python)**

    :download:`The whole folder<tutorial2.zip>` Download the whole folder, or:

    - :download:`tutorial2.py` The skeleton code or
    - :download:`tutorial2_complete.py` or the completed code; and
    - :download:`tutorial2.cellml<tutorial2.cellml>` the input CellML file.

.. contents:: Contents
    :local:

Step 0: Set-up
--------------
The instructions here build and test the skeleton code.
If you'd rather not work through the tutorial yourself and want to skip to the completed version, please rename the files removing the :code:`_complete` from their names.

.. tabs::

  .. code-tab:: cpp

    Navigate into the directory and check that you can build the template against the libCellML library successfully:

    .. code-block:: console

        cmake -DINSTALL_PREFIX=../../install
        make -j

    Running the template:

    .. code-block:: console

        ./tutorial2

    ... should give the output:

    .. code-block:: console

        -----------------------------------------------
          TUTORIAL 2: ERROR CHECKING AND VALIDATION
        -----------------------------------------------

  .. code-tab:: python

    Confirm that you're able to run the :code:`tutorial2.py` template against the libCellML library.

  .. code-block:: console

      python3 tutorial2.py

  This should give the output:

  .. code-block:: console

      ------------------------------------------------------------
          TUTORIAL 2: ERROR CHECKING AND VALIDATION
      ------------------------------------------------------------

Step 1: Parse a CellML file into a model
----------------------------------------

.. container:: useful

    :api:`Parser class<Parser>`

    - create
    - parseModel

    Utility functions (C++)

    - printModel

    Utility functions (Python)

    - print_model

.. container:: dothis

    **1.a** As you did in :ref:`Tutorial 1<tutorial1>`, create a :code:`Parser` and use it to create a model from the :code:`tutorial2.cellml` file provided.

Instead of duplicating the work you did throughout the middle steps of :ref:`Tutorial 1<tutorial1>`, we've provided you with a set of utility functions in the :code:`tutorial_utilities.[cpp,h]` files which will help with some of the repeated bits of these tutorials.

.. container:: dothis

    **2.a** Use the utility function :code:`printModel(yourModelHere)` (in C++) or :code:`print_model(your_model_here)` to output the contents of the model you just created to the terminal so that you can see it all properly.

.. code-block:: console

    TODO

Step 2: Validate the model
--------------------------
Within the libCellML library is the :code:`Validator` class.
This has one job: to make sure that what you give it is *valid* in terms of its CellML2.0 compliance.
Does it mean that your simulations will work the way you expect?
Nope, you can still write rubbish!
You can think of the validation being the spell-checker for your model.
It doesn't judge the meaning of *what* you've written, just the nitty-gritty of *how* you've written it.

.. container:: useful

    :api:`Validator class<Validator>`

    - create
    - validateModel

The validator can also help you as you create and edit models by pointing out what's missing or incorrect as you go.
This is really easy:

.. tabs::

  .. code-tab:: cpp

      auto validator = libcellml::Validator::create();
      validator->validateModel(yourModelHere);

  .. code-tab:: python

      from libcellml import Validator

      validator = Validator()
      validator.validateModel(your_model_here)

.. container:: dothis

    **2.a** Create a validator instance and pass your model to it, as above.

When you've created a :code:`Validator` object and called it to check a model, a record of any issues is stored inside the validator.
To figure out what's going on, you need to retrieve the pointers to these :code:`Issue` objects.

.. container:: useful

    :api:`Validator class<Validator>`

    - issueCount
    - issue
    - errorCount
    - error
    - warningCount
    - warning
    - hintCount
    - hint
    - messageCount
    - message

As in :ref:`Tutorial 1<tutorial1>`, we can call a :code:`count` function (in the case of a validator, this is the :code:`issueCount()` function to determine whether any issues have been raised.
Note that an issue a can have different levels: errors, warnings, hints, and messages.
In the :code:`Validator`, only those issues which are errors indicate validation problems, but it's usually a good idea to check all the issues anyway.

.. container:: dothis

    **2.b** Retrieve the number of issues from the validator, and print it to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial2_complete.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: // 2.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial1_complete.py
        :language: python
        :start-at: #  2.a
        :end-before: #  2.c

.. code-block:: terminal

    TODO

Now we need to create an iterative loop to retrieve all the issues (and there should be a few in this particular model!) from the validator.
Again following the same retrieval idiom as in Tutorial 1 for items in sets, we can access the issues using an index:

.. tabs::

  .. code-block:: C++

      auto theFifteenthIssue = validator->issue(14);

  .. code-tab:: Python 

      the_15th_issue = validator.issue(14)

.. container:: useful

    :api:`Issue class<Issue>`

    - description
    - url
    - referenceHeading
    - cellmlElementType
    - cellmlElement

    Utility functions (C++)

    - getCellmlElementTypeFromEnum
    - getIssueLevelFromEnum

    Utility functions (Python)

    - get_cellml_element_type_from_enum
    - get_issue_level_from_enum

Inside an :code:`Issue` structure are three fields which are really useful.
These are the :code:`description` (which does what you'd think) and the :code:`referenceHeading`, which points you to the section in the CellML2.0 specification document for reference, and a :code:`url` with links to a relevant website for more information.

Each issue also has a level indicator, one of:

  - ERROR,
  - WARNING,
  - HINT, and
  - MESSAGE.

As well as storing text-based information, the issue also keeps track of which item has the problem.  
This can be accessed using the :code:`item` function, and has a type given by the enumeration in :code:`cellmlElementType` function.
The :code:`CellmlElementType` enumeration contains:

    - COMPONENT,
    - COMPONENT_REF,
    - CONNECTION,
    - ENCAPSULATION,
    - IMPORT,
    - MAP_VARIABLES,
    - MATH,
    - MODEL,
    - RESET,
    - RESET_VALUE,
    - TEST_VALUE,
    - UNDEFINED,
    - UNIT,
    - UNITS, and
    - VARIABLE.

Two utility functions have been provided which will convert the enums for error level or element type into a string for printing.

.. container:: dothis

    **2.c** Create a loop (to the number of issues found in 2.c) to retrieve each issue pointer.
    For each issue, retrieve and print as much information as you can.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial2_complete.cpp
        :language: c++
        :start-at: //  2.c
        :end-before: //  end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial1_complete.py
        :language: python
        :start-at: #  2.a
        :end-before: #  end 2



