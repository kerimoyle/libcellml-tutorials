..  _tutorial5:

Tutorial 5: Annotation tools
============================

.. container:: shortlist

    By the time you have worked through this tutorial you will be able to:

    - Parse a CellML file into a :code:`Model` instance;
    - Determine the type of item which has a given id;
    - Use the :code:`Annotator` class to retrieve an item using only its id string;
    - Repair duplicated id strings within the model scope; and
    - Automatically generate and assign unique ids to any or all items.

.. container:: shortlist

    **Code (C++)**

    - :download:`CMakeLists.txt<CMakeLists.txt>` The CMake file for building this tutorial;
    - :download:`annotateModel.cpp<annotateModel.cpp>` Either the skeleton code, or ..
    - :download:`annotateModel_completed.cpp<annotateModel_completed.cpp>` the completed tutorial code;

.. container:: shortlist

    **Code (Python)**

    - :download:`annotateModel.py<annotateModel.py>` Either the skeleton code, or ..
    - :download:`annotateModel_completed.py<annotateModel_completed.py>` the completed tutorial code;

.. container:: shortlist

    **Resources**

    - :download:`MysteryModel.cellml<code/resources/MysteryModel.cellml>` file to read; and
    - :download:`AnotherMysteryModel.cellml<code/resources/AnotherMysteryModel.cellml>` import dependency.

.. contents:: Contents
    :local:

Background
----------

"Marco Polo" is a game played with many people in a swimming pool.
One person calls "Marco" with their eyes closed.
Others answer "Polo" and the first person must find them by following the sound.
In this tutorial you are given id strings and a mystery CellML model file.
We will work through how the :code:`Annotator` class can be used to locate the desired objects.

Step 1: Parse the mystery model
-------------------------------

.. container:: dothis

    **1.a** Read the file "MysteryModel.cellml" into a string.

.. container:: dothis

    **1.b** Create a Parser item.

.. container:: dothis

    **1.c** Use the parser to deserialise the contents of the string you've read and return the model.

.. container:: dothis

    **1.d** Check that the parser has not raised any issues.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  1.a
        :end-before: //  end 1

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  1.a
        :end-before: #  end 1


Step 2: Retrieve an item with a unique id
-----------------------------------------

.. container:: useful

    :api:`Annotator class<Annotator>`

    - create
    - setModel
    - item
    - issueCount
    - issue

    Tutorial functions

    - C++: getCellmlElementTypeFromEnum will return a string version of the CellmlElementType enumeration
    - Python: get_cellml_element_type_from_enum

.. container:: dothis

    **2.a** Create an :code:`Annotator` item and use its :code:`setModel` function to pass in the parsed mystery model.

    - In C++: The :code:`item` function returns a :code:`libcellml::AnyItem`, a :code:`std::pair` whose first attribute is a :code:`libcellml::CellmlElementType` enumeration; and second attribute is a :code:`std::any` cast of the item itself.
    - In Python: The :code:`item` function returns a tuple.
      The first item is a :code:`CellmlElementType` enumeration, the second is the item itself.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: //  end 2.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  2.a
        :end-before: #  end 2.a

.. container:: dothis

    **2.b** Retrieve the item with an id of "marco".
    Use the helper function to convert the enumeration of its type into a string, and print to the terminal.
    
.. code-block:: terminal

    The item with ID 'marco' is a VARIABLE

.. container:: dothis

    **2.c** Check that the annotator has not reported any issues.

.. container:: dothis

    **2.d** (C++ only) Cast the second attribute of the macro item into a libcellml::VariablePtr item using :code:`std::any_cast`.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  2.b
        :end-before: //  end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  2.b
        :end-before: #  end 2

Step 3: Retrieve items whose id are not unique
----------------------------------------------

.. container:: useful

    :api:`Annotator class<Annotator>`

    - items
    - isUnique
    - assignId

.. container:: dothis

    **3.a** Now try the same procedure to find the item with id of "polo".
    Retrieve the item and print its type to the terminal.
    
.. code-block:: terminal

    The type of item with ID "polo" is UNDEFINED

.. container:: dothis

    **3.b** The item type returned is :code:`UNDEFINED` ... so we need to check what the annotator has to say about it. 
    Retrieve the issues from the annotator and print them to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: //  end 3.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  3.a
        :end-before: #  end 3.b

.. code-block:: terminal

    Recorded 1 issues:
    Issue [0] is a WARNING:
        description: The id 'polo' occurs 6 times in the model so a unique item cannot be located.
        stored item type: UNDEFINED

.. container:: dothis

    **3.c** Since the id is not unique, we need to retrieve all items with that id for investigation.
    Use the :code:`items` function to retrieve the vector of items with id "polo", and iterate through it printing the different types to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  3.c
        :end-before: //  end 3.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  3.c
        :end-before: #  end 3.c
    
.. code-block:: terminal

    The items with an id of 'polo' have types of:
      - [0] UNITS
      - [1] UNITS
      - [2] UNIT
      - [3] VARIABLE
      - [4] RESET
      - [5] RESET_VALUE

The item we want has type :code:`UNIT`, and we'd like it to be unique so that we can annotate it properly.
We need to change the other items to have other (also unique) ids.
The :code:`Annotator` class can create a unique id for an item using the :code:`assignId` function.

.. container:: dothis

    **3.d** Assign an automatic id to all of the items with id "polo", except for the one whose type is :code:`UNIT`.

.. container:: dothis

    **3.e** Check that the id of "polo" is now unique in the model by calling the :code:`isUnique` function.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  3.d
        :end-before: //  end 3.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  3.d
        :end-before: #  end 3.e

Now we know that there is only one item in the model with id "polo", and we also know that it has type :code:`UNIT`.
In circumstances where you know the type of the item with the id you're fetching ahead of time, you can retrieve it without the need to cast using the direct functions according to type: these are listed below.

.. container:: useful

    :api:`Annotator class<Annotator>`

    - component
    - componentRef
    - connection
    - encapsulation
    - import
    - mapVariables
    - model
    - reset
    - resetValue
    - testValue
    - unit
    - units
    - variable

    :api:`Unit class<Unit>`

    - units
    - index

.. container:: dothis

    **3.f** Retrieve the :code:`Unit` with id "polo" directly.
    The :code:`Unit` class has two attributes:
         - :code:`units()` returns the parent :code:`Units` item; and
         - :code:`index()` returns the index of this unit within its parent.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  3.f
        :end-before: //  end 3

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  3.f
        :end-before: #  end 3
    
Step 4: Discover items whose ids are unknown
--------------------------------------------
Now that we've found Marco and fixed the duplicates of Polo, we'd like to know what other ids are being used in this model.

.. container:: useful

    :api:`Annotator class<Annotator>`

    - ids
    - duplicateIds
    - itemCount

.. container:: dothis

    **4.a** Use the :code:`ids` function to return a vector of id strings used in the model, and print them to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  4.a
        :end-before: //  end 4.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  4.a
        :end-before: #  end 4.a

.. code-block:: terminal

    The id strings used in the model are:
        - "b4da55"
        - "b4da56"
        - "b4da57"
        - "b4da58"
        - "b4da59"
        - "i_am_a_component"
        - "marco"
        - "me_too"
        - "polo"
        - "someOtherDuplicatedId"
        - "someOtherId"
        - "whoAmIAndWhereDidIComeFrom"

The hex strings printed are those which have been automatically generated by the :code:`assignId` function; we can also see the "marco" and "polo" ids as expected.

.. container:: dothis

    **4.b** Use the :code:`duplicateIds` function to return a vector of those ids which have been duplicated in the model.
    Use the :code:`itemCount` function to return the number of times each occurs, and print to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  4.b
        :end-before: //  end 4

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  4.b
        :end-before: #  end 4

.. code-block:: terminal

    Duplicated id strings are:
    - "someOtherDuplicatedId" occurs 3 times

Step 5: Trace provenance of imported items
------------------------------------------

The final step is to make sure that imported items can have their annotations tracked back to their sources too.  

.. container:: useful

    :api:`Importer class<Importer>`

    - create
    - resolveImports

    :api:`ImportedEntity class<ImportedEntity>` (applies to both the :code:`Units` and the :code:`Component` class)

    - isImport
    - importReference
    - importSource

    :api:`ImportSource class<ImportSource>`

    - model
    - url

.. container:: dothis

    **5.a** Retrieve an item with id of "whoAmIAndWhereDidIComeFrom" and print its item type to the terminal.
    
.. code-block:: terminal

    The type of item with ID "whoAmIAndWhereDidIComeFrom" is UNITS

.. container:: dothis

    **5.b** Cast it into a CellML item of the appropriate type.

.. container:: dothis

    **5.c** Use its :code:`isImport()` function to verify that it is imported.

.. container:: dothis

    **5.d** Create an :code:`Importer` instance and use it to resolve this model's imports.
    Check that it has not raised any issues.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  5.a
        :end-before: //  5.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  5.a
        :end-before: #  5.e

.. container:: dothis

    **5.e** Retrieve all the information needed to locate any annotations on the original item:

    - the URL from which it was imported; and
    - the id of the item in the original model.
    
    Print these to the terminal.
    
.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  5.e
        :end-before: //  end 5

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  5.e
        :end-before: #  end 5

.. code-block:: terminal

    The units with id "whoAmIAndWhereDidIComeFrom" came from:
    - url: AnotherMysteryModel.cellml
    - id: i_am_a_units_item

Step 6: Bulk operations
-----------------------

.. container:: dothis

    **6.a** Loop through all of the model's components and print their id to the terminal.
    Use the :code:`assignIds` function with an item type of :code:`libcellml::CellmlElementType::COMPONENT` to give all of the items of that type a new unique id.
    Print the ids again and notice that the blanks have been filled with automatically generated strings, but existing ids are unchanged. 

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  6.a
        :end-before: //  6.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  6.a
        :end-before: #  6.b

.. code-block:: terminal

    Before automatic assignment the components have ids:
        - "i_am_a_component"
        - ""
        - ""
        - ""
        - "me_too"
        - ""

    After automatic assignment the components have ids:
        - "i_am_a_component"
        - "b4da5a"
        - "b4da5b"
        - "b4da5c"
        - "me_too"
        - "b4da5d"

Finally, we decide that it's too cold for swimming, and want to nuke all the ids and go home.

.. container:: useful

    :api:`Annotator class<Annotator>`

    - clearAllIds
    - assignAllIds

.. container:: dothis

    **6.b** Use the :code:`clearAllIds` function to completely remove all id strings from the model.
    Check that they have gone by repeating step 4.a to print any ids to the terminal.

.. code-block:: terminal

    There are 0 ids in the model.

Go looking for Marco, but he's gone home already.

.. container:: dothis

    **6.c** Retrieve the item with id "marco" and print its type to the terminal.
    Retrieve and print any issues in the annotator to the terminal.

.. code-block:: terminal

    The type of item with ID "marco" is UNDEFINED

    The Annotator has found 1 issues:
    Warning[0]:
        Description: Could not find an item with an id of 'marco' in the model.

Now you regret nuking our friends and make plans to return tomorrow and annotate everything.  

.. container:: dothis

    **6.d** Use the :code:`assignAllIds` function to give an automatic id to everything which doesn't already have one (which is everything now!).

.. container:: dothis

    **6.e** Try to retrieve duplicated ids from the annotator as in step 4.b, and check that it returns an empty list.
    
.. code-block:: container

    There are 0 duplicated ids in the model.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: annotateModel_completed.cpp
        :language: c++
        :start-at: //  6.b
        :end-before: //  end 6

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: annotateModel_completed.py
        :language: python
        :start-at: #  6.b
        :end-before: #  end 6