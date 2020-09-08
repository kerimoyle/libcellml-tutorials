.. _examples_annotation_tool_dev:

=========================
Annotation tool developer
=========================

Full context: :download:`example_annotationToolDev.cpp<../code/example_annotationToolDev.cpp>`
Full context: :download:`example_annotationToolDev.py<../code/example_annotationToolDev.py>`

Parse an existing CellML model 
------------------------------

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
      
Use an Annotator item to investigate the model 
----------------------------------------------

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
      
Automatically assign unique IDs 
-------------------------------

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
      
