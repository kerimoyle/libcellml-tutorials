.. _tutorial1:

Tutorial 1: Reading and writing CellML files
============================================

Welcome to the introductory tutorial for the use of the libCellML library.
Using these tutorials in sequence will walk you through the functionality and build your knowledge step by step.
If, however, you just need to jump in and figure out one specific idea, you can find a tutorial related to that subject using the :ref:`How-to pages<howto>`.

After completing this tutorial you will be able to:

- Read the contents of a CellML file;
- Deserialise its contents using the :code:`Parser` to create a :code:`Model` structure;
- Investigate the hierarchical contents of the :code:`Model`, including the :code:`Components`, :code:`Variables`, and MathML blocks; and
- Serialise the model and write to another file using the :code:`Printer`.

Requirements
------------

.. container:: directorylist

    **C++ resources**
    
    - :download:`CMakeLists.txt<CMakeLists.txt>` The CMake file for building;
    - :download:`tutorial1.cpp<tutorial1.cpp>` The skeleton code or
    - :download:`tutorial1_complete.cpp<tutorial1_complete.cpp>` the completed code.

    **Python resources**

    - :download:`tutorial1.py` The skeleton code or
    - :download:`tutorial1_complete.py` the completed code.

    **CellML resources**

    - :download:`tutorial1.cellml<tutorial1.cellml>` the input CellML file.


Step 0: Set-up
--------------
For each of the tutorials, there is template code provided for you to get started.
If you'd rather see the completed code instead of working through the tasks yourself, simply rename the completed files so that they match the skeleton file names (that is, remove the :code:`_completed` from their names).

.. tabs::

    .. tab:: C++

        .. container:: dothis

            **0.a** Navigate into the :code:`tutorial1` folder and confirm that you're able to compile and run this template against the libCellML library.

        .. code-block:: text

            cmake -DINSTALL_PREFIX=../../install
            make -j

        Running the template:

        .. code-block:: text

            ./tutorial1

        ... should give the output:

        .. code-block:: text

            -----------------------------------------------
             TUTORIAL 1: READING AND WRITING CELLML FILES
            -----------------------------------------------

    .. tab:: Python

        For each of the tutorials, there is template code provided for you to get started in each of the :code:`tutorial*.py` files.

        .. container:: dothis

            **0.a** Confirm that you're able to run the :code:`tutorial1.py` template against the libCellML library.
            Navigate into the directory and run the skeleton code file, :code:`tutorial1.py`.

        .. code-block:: text

            cd tutorial1
            python3 tutorial1.py

        This should give the output:

        .. code-block:: text

            -----------------------------------------------------
                TUTORIAL 1: READING AND WRITING CellML2 FILES
            -----------------------------------------------------

Step 1: Deserialise a CellML2 file
----------------------------------
CellML is a format in which all the information is stored in a strict hierarchical way.
In order for this information to be transformed into a format which can be *used* for modelling and simulation it needs to be
*deserialised* into the internal :code:`Model` format.
The hard work of this transformation is done by the :code:`Parser` object which will take a raw CellML string which represents a model and return its corresponding :code:`Model` item.

.. container:: useful

    :api:`Parser class <Parser>`

    - :api:`create <Parser?fName=create>`
    - :api:`parseModel <Parser?fName=parseModel>`

.. container:: dothis

    **1.a** You've been provided with a CellML file which we'll be reading in this tutorial, :code:`tutorial1.cellml`.
    Open the file and read its entire contents into a string.

.. container:: dothis

    **1.b** Create a :code:`Parser` instance.
    Pass the string you read above into the parser using the :code:`parseModel` function.
    This will return a newly-created :code:`Model` instance.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial1_complete.cpp
        :language: c++
        :start-at: //  1.a
        :end-before: //  end 1

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial1_complete.py
        :language: python
        :start-at: #  1.a
        :end-before: #  end 1

Now we have a *deserialised* CellML model which we can manipulate using the libCellML library.

Step 2: Investigate the contents of the model
---------------------------------------------
Now that we have a model, let's see what's inside it.
All retrieval functions - where you want to read something about any item - are callable from simple functions naming the thing you want.

.. container:: useful

    :api:`Model class <Model>`

    - :api:`name <Model?fName=name>`
    - :api:`id <Model?fName=id>`
    - :api:`componentCount <Model?fName=componentCount>`
    - :api:`unitsCount <Model?fName=unitsCount>`

    :api:`Component class <Component>`

    - :api:`name <Component?fName=name>`
    - :api:`id <Component?fName=id>`
    - :api:`variableCount <Component?fName=variableCount>`

    :api:`Variable class <Variable>`

    - :api:`name <Variable?fName=name>`
    - :api:`id <Variable?fName=id>`

For example, to find the name of the model we simply call its :code:`name()` function to return the string of its name.

.. container:: dothis

    **2.a** Find out the name and id of your model and print them to the terminal.

.. code-block:: text

    The model name is: tutorial_1_model
    The model id is: tutorial_1_model_id_is_here

The :code:`Model` itself stores two kinds of objects: a set of :code:`Units` objects, and a set of :code:`Component` objects.
There are generic :code:`somethingCount()` functions which will return the number of :code:`Something` items within that object:

.. tabs::

    .. code-tab:: cpp

        auto numberOfComponents = myFirstModel->componentCount();

    .. code-tab:: python

        number_of_components = my_first_model.componentCount()

.. container:: dothis

    **2.b** Find out the number of :code:`Component` items in the model, and print it to the terminal.

.. code-block:: text

    The tutorial_1_model model has 1 component(s)

Items like components and units (and later, variables) which are stored in sets can be accessed by their index as well as by their name.
At this stage, since we don't *know* the name of the components, we'll have to use their indices to access them for printing to the terminal.
The indices start at :code:`0`.

Each :code:`Component` itself (or later, :code:`Units` or :code:`Variable`) is retrieved from its parent item as a pointer (that is, a :code:`ComponentPtr` type) using the same idiom as the names and ids:

.. tabs::

    .. code-tab:: cpp

        // Retrieve the 33rd component from myFirstModel.  Note the indexing from zero.
        auto thirtyThirdComponent = myFirstModel->component(32);

    .. code-tab:: python

        # Retrieve the 33rd component from my_first_model.  Note the indexing from zero.
        thirty_third_component = my_first_model.component(32)

.. container:: dothis

    **2.c** Retrieve the first component from the model, and print its name and id to the terminal.

.. code-block:: text

    The tutorial_1_model model has 1 component(s):
        Component[0] has name: i_am_a_component
        Component[0] has id: my_component_id

:code:`Component` items in turn contain four kinds of items, these being:

- a set of :code:`Variable` items;
- a :code:`MathML` string which controls the governing mathematical equations of the model behaviour;
- (for later) a set of :code:`Reset` items; and
- (also for later) an optional nested subset of :code:`Component` items too.

In this tutorial we'll only look at the variables and maths.

.. container:: dothis

    **2.d** Retrieve the number of variables inside your component, and print it to the screen.

.. container:: dothis

    **2.e** Create a loop through the variables, retrieve each, and print their names to the screen.

.. code-block:: text

    The i_am_a_component component has 3 variable(s):
        Variable[0] has name: a
        Variable[1] has name: b
        Variable[2] has name: c

.. container:: dothis

    **2.f** We will look more at what the MathML string actually *means* in later tutorials, but for now, retrieve the MathML string from your component using the :code:`math()` function, and print it to the screen.

.. code-block:: text

    Component i_am_a_component has a MathML string of: 
        <math xmlns="http://www.w3.org/1998/Math/MathML">
        <apply>
            <eq/>
            <ci>a</ci>
            <apply>
            <plus/>
            <ci>b</ci>
            <ci>c</ci>
            </apply>
        </apply>
        </math>

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial1_complete.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: //  end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial1_complete.py
        :language: python
        :start-at: #  2.a
        :end-before: #  end 2

Step 3: Serialise the model and output to a file
------------------------------------------------
Now that you've seen how to *deserialise* a CellML file into a model using the :code:`Parser`, it's time to go in the other direction.
This reverse operation is handled by the :code:`Printer`, which will transform a model into a string for output to a file.

.. container:: useful

    :api:`Printer class <Printer>`

    - :api:`create <Printer?fName=create>`
    - :api:`printModel <Printer?fName=printModel>`

.. tabs::

    .. code-tab:: cpp

        auto printer = libcellml::Printer::create();
        std::string serialisedModelString = printer->printModel(myFirstModel);

    .. code-tab:: python

        printer = Printer()
        serialised_model_string = printer.printModel(my_first_model)

.. container:: dothis

    **3.a** Create a printer and use it to serialise your model.

.. container:: dothis

    **3.b** Write that serialised string to a new :code:`.cellml` file.

.. container:: dothis

    **3.c** Go and have a cuppa, you're done :)

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial1_complete.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: //  3.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial1_complete.py
        :language: python
        :start-at: #  3.a
        :end-before: #  3.c
