.. _examples_annotation_tool_dev:

Annotation tool developer
=========================
This example walks through the processes involved in preparing a CellML model for annotation.
Any item in the model which requires annotation needs to have a unique ID string, so the chief business of the :code:`Annotator` class is working with the id attributes.
In all other parts of libCellML, items are retrieved from their parents using either their index or their name.
In the :code:`Annotator`, the ID string is used instead.
Because an item of any type could have an ID attribute, there is no way of knowing what kind of will be returned.
The class makes use of casting to and from the (C++ only) :cpp_reference:`std::any type<w/cpp/utility/any>` to handle this uncertainty.

**Contents**
.. contents::
   :local:

| **C++ resources**
|    ├ :download:`CMakeLists.txt<../code/example_annotationToolDev_CMakeLists.txt>`
|    ├ :download:`example_annotationToolDev.cpp<../code/example_annotationToolDev.cpp>`
|    └ resources/
|        └ :download:`annotationExample.cellml<../code/resources/annotationExample.cellml>`

| **Python resources**
|    ├ :download:`example_annotationToolDev.py<../code/example_annotationToolDev.py>`
|    └ resources/
|        └ :download:`annotationExample.cellml<../code/resources/annotationExample.cellml>`



Parse an existing CellML model 
------------------------------
The first step in any annotation process is to open the model to be annotated.
Here we use the :code:`Parser` class to read the file, and deserialise the CellML into a :code:`Model` object.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_annotationToolDev.cpp
        :language: c++
        :start-after: // STEP 1
        :end-before: // STEP 2

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_annotationToolDev.py
        :language: python
        :start-after: # STEP 1
        :end-before: # STEP 2
      
Use an annotator to investigate the model 
-----------------------------------------
The :code:`Annotator` class is fundamentally different from other libCellML classes in that it used ID strings (as opposed to names or indices) to retrieve and manipulate items.  
It can be used to retrieve the ID string and item type to which it pertains, using the *dictionary* function.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_annotationToolDev.cpp
        :language: c++
        :start-after: // STEP 2
        :end-before: // STEP 3

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_annotationToolDev.py
        :language: python
        :start-after: # STEP 2
        :end-before: # STEP 3

      .. code-block:: terminal

         Existing id strings are:
            beige = connection
            black = component_ref
            blue = unit
            brown = encapsulation
            duplicateId1 = units
            duplicateId1 = component
            duplicateId2 = connection
            duplicateId2 = variable
            duplicateId3 = import
            duplicateId3 = units
            duplicateId3 = variable
            duplicateId4 = component
            duplicateId4 = map_variables
            green = units
            indigo = variable
            mauve = test_value
            orange = import
            puce = map_variables
            red = model
            taupe = reset_value
            violet = reset
            yellow = component
      
Retrieve items by id (known type) 
---------------------------------
When you know the type of item which is assigned a certain ID, it is straightforward to fetch that item from the :code:`Annotator` using any of the type-specific functions, as shown below.
A null pointer will be returned if the item does not have the type requested, or if its ID does not exist in the model. 

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_annotationToolDev.cpp
        :language: c++
        :start-after: // STEP 3
        :end-before: // STEP 4

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_annotationToolDev.py
        :language: python
        :start-after: # STEP 3
        :end-before: # STEP 4

      .. code-block:: terminal

         The name of the component with id of "yellow" is "component3".
      
Retrieve items by id (unknown type) 
-----------------------------------
It's more likely that you won't know the type of item to which a given ID relates, so will need to use the generic *item* function to retrieve it.  
This returns two pieces of information: the first is the appropriate :code:`CellMLElements` enum value for the type, and the second is the item itself cast into the :cpp_reference:`std::any type<w/cpp/utility/any>` (in C++).
In Python, the returned item requires no further transformation.

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_annotationToolDev.cpp
        :language: c++
        :start-after: // STEP 4
        :end-before: // STEP 5

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_annotationToolDev.py
        :language: python
        :start-after: # STEP 4
        :end-before: # STEP 5

      .. code-block:: terminal

         The item with id of "green" has type of "units".

      
Handle duplicated ID attributes
-------------------------------
Duplicated IDs in any XML document are not permitted, and CellML is no different.
There are several tools to support users in removing duplicates from their models.
The code snippet below uses the annotator to return a set of ID strings which have been duplicated within the model scope, and then uses the collective and generic function *items* to return all of them.
From here, the items can be assigned an automatically generated ID string which is guaranteed to be unique within the model scope.
Of course, you can also set the id of these items manually too if you'd rather.


.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_annotationToolDev.cpp
        :language: c++
        :start-after: // STEP 5
        :end-before: // STEP 6

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_annotationToolDev.py
        :language: python
        :start-after: # STEP 5
        :end-before: # STEP 6

      .. code-block:: terminal

         There are 4 duplicated ids in the model.
         - duplicateId1
         - duplicateId2
         - duplicateId3
         - duplicateId4

         Before assigning automatic ids there are 2 items with an id of "duplicateId1".
         After assigning automatic ids there are 0 items with an id of "duplicateId1".
         After fixing all duplicates there are 0 duplicated ids in the model.
      
Automatically assign unique IDs in bulk 
---------------------------------------
In addition to using the automatic ID generation on individual items as shown above, you can also use it for all items of a certain type.
The example below shows how calling the *assignIds* function for the :code:`CellMLElement COMPONENT` type fills in the missing ID from :code:`component2` without changing the IDs of the other components.
Finally, IDs for every item in a model can be set with the *assignAllIds* function, and cleared from every item using the *clearAllIds* function. 

.. tabs::

    .. tab:: C++ snippet

      .. literalinclude:: ../code/example_annotationToolDev.cpp
        :language: c++
        :start-after: // STEP 6
        :end-before: // END

    .. tab:: Python snippet

      .. literalinclude:: ../code/example_annotationToolDev.py
        :language: python
        :start-after: # STEP 6
        :end-before: # END
      
      .. code-block:: terminal

         Before automatic ids are assigned by type:
            Component 1: b4da56
            Component 2: 
            Component 3: yellow
            Component 4: b4da5c
         After automatic ids are assigned to component items:
            Component 1: b4da56
            Component 2: b4da5e
            Component 3: yellow
            Component 4: b4da5c

         Before assigning all automatic ids, there are 24 items with an id attribute.
         After assigning all automatic ids, there are 31 items with an id attribute.
         After clearing all ids, there are 0 items with an id attribute.
      
