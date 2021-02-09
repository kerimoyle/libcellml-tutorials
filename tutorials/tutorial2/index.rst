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

    - Read and deserialise a CellML model from a file (as in :ref:`Tutorial 1<tutorial1>`);
    - Retrieve the name and id of models, components, and variables;
    - Navigate through the hierarchy of the model and its subsidiary elements (components, variables, units and MathML blocks); and
    - Serialise and print a :code:`Model` structure to a CellML file.


Requirements
------------

.. container:: directorylist

    **C++ resources**

    - :download:`CMakeLists.txt<CMakeLists.txt>` The CMake file for building;
    - :download:`tutorial2.cpp<tutorial2.cpp>` The skeleton code or
    - :download:`tutorial2_complete.cpp<tutorial2_complete.cpp>` the completed code; and
    - :download:`utilities.cpp<../utilities/utilities.cpp>` and :download:`utilities.h<../utilities/utilities.h>` Utility functions for use in the tutorials.

    **Python resources**

    - :download:`tutorial2.py` The skeleton code or
    - :download:`tutorial2_complete.py` or the completed code; and
    - :download:`utilities.py<../utilities/utilities.py>` Utility functions for use in the tutorials.

    **CellML resources**

    - :download:`tutorial2.cellml<tutorial2.cellml>` the input CellML file.


Step 0: Set-up
--------------
The instructions here build and test the skeleton code.
If you'd rather not work through the tutorial yourself and want to skip to the completed version, please rename the files removing the :code:`_complete` from their names.

.. tabs::

    .. tab:: C++

        Navigate into the directory and check that you can build the template against the libCellML library successfully:

        .. code-block:: text

            cmake -DINSTALL_PREFIX=../../install
            make -j

        Running the template:

        .. code-block:: text

            ./tutorial2

        ... should give the output:

        .. code-block:: text

            -----------------------------------------------
            TUTORIAL 2: ERROR CHECKING AND VALIDATION
            -----------------------------------------------

    .. tab:: Python

        Confirm that you're able to run the :code:`tutorial2.py` template against the libCellML library.

        .. code-block:: text

            python3 tutorial2.py

        This should give the output:

        .. code-block:: text

            ------------------------------------------------------------
                TUTORIAL 2: ERROR CHECKING AND VALIDATION
            ------------------------------------------------------------

Step 1: Parse a CellML file into a model
----------------------------------------

.. container:: useful

    :api:`Parser class <Parser>`

    - :api:`create <Parser?fName=create>`
    - :api:`parseModel <Parser?fName=parseModel>`

    Utility functions (C++)

    - printModel

    Utility functions (Python)

    - print_model

.. container:: dothis

    **1.a** As you did in :ref:`Tutorial 1<tutorial1>`, create a :code:`Parser` and use it to create a model from the :code:`tutorial2.cellml` file provided.

Instead of duplicating the work you did throughout the middle steps of :ref:`Tutorial 1<tutorial1>`, we've provided you with a set of utility functions in the :code:`utilities.[cpp,h]` files which will help with some of the repeated bits of these tutorials.

.. container:: dothis

    **1.b** Use the utility function :code:`printModel(yourModelHere)` (in C++) or :code:`print_model(your_model_here)` to output the contents of the model you just created to the terminal so that you can see it all properly.

.. code-block:: text

    MODEL: 'tutorial_2_model', id: 'tutorial 2 id has spaces'
    UNITS: 1 custom units
        [0]: i_am_a_units_item
    COMPONENTS: 1 components
        [0]: i_am_a_component id: my_component_id
            VARIABLES: 4 variables
                [0]: 1st [dimensionless]
                [1]: b
                [2]: c [dimensionless], initial = this_variable_doesnt_exist
                [3]: d [i_dont_exist]
            Maths in the component is:
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

Step 2: Validate the model
--------------------------
Within the libCellML library is the :code:`Validator` class.
This has one job: to make sure that what you give it is *valid* in terms of its CellML2.0 compliance.
Does it mean that your simulations will work the way you expect?
Nope, you can still write rubbish!
You can think of the validation being the spell-checker for your model.
It doesn't judge the meaning of *what* you've written, just the nitty-gritty of *how* you've written it.

.. container:: useful

    :api:`Validator class <Validator>`

    - :api:`create <Validator?fName=create>`
    - :api:`validateModel <Validator?fName=validateModel>`

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

    :api:`Validator class <Validator>`

    - :api:`issueCount <Validator?fName=>issueCount`
    - :api:`issue <Validator?fName=issue>`
    - :api:`errorCount <Validator?fName=errorCount>`
    - :api:`error <Validator?fName=error>`
    - :api:`warningCount <Validator?fName=warningCount>`
    - :api:`warning <Validator?fName=warning>`
    - :api:`hintCount <Validator?fName=hintCount>`
    - :api:`hint <Validator?fName=hint>`
    - :api:`messageCount <Validator?fName=messageCount>`
    - :api:`message <Validator?fName=message>`

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
        :end-before: //  2.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial2_complete.py
        :language: python
        :start-at: #  2.a
        :end-before: #  2.c

.. code-block:: text

    The validator has found 5 issues!

Now we need to create an iterative loop to retrieve all the issues (and there should be a few in this particular model!) from the validator.
Again following the same retrieval idiom as in Tutorial 1 for items in sets, we can access the issues using an index:

.. tabs::

  .. code-block:: c++

      auto theFifteenthIssue = validator->issue(14);

  .. code-tab:: python 

      the_15th_issue = validator.issue(14)

.. container:: useful

    :api:`Issue class <Issue>`

    - :api:`description <Issue?fName=description>`
    - :api:`url <Issue?fName=url>`
    - :api:`referenceHeading <Issue?fName=referenceHeading>`
    - :api:`cellmlElementType <Issue?fName=cellmlElementType>`
    - :api:`cellmlElement <Issue?fName=cellmlElement>`

    Utility functions (C++)

    - getCellmlElementTypeFromEnum
    - getIssueLevelFromEnum

    Utility functions (Python)

    - get_cellml_element_type_from_enum
    - get_issue_level_from_enum

Inside an :code:`Issue` structure are three fields which are really useful.
These are the :code:`description` (which does what you'd think) and the :code:`referenceHeading`, which points you to the section in the CellML2.0 specification document for reference, and a :code:`url` with links to a relevant website for more information.

Each issue also has a level indicator, one of:

.. container:: shortlist 

    - :code:`ERROR`,
    - :code:`WARNING`,
    - :code:`HINT`, and
    - :code:`MESSAGE`.

As well as storing text-based information, the issue also keeps track of which item has the problem.  
This can be accessed using the :code:`item` function, and has a type given by the enumeration in :code:`cellmlElementType` function.
The :code:`CellmlElementType` enumeration contains:

.. container:: shortlist 

    - :code:`COMPONENT` ,
    - :code:`COMPONENT_REF` ,
    - :code:`CONNECTION` ,
    - :code:`ENCAPSULATION` ,
    - :code:`IMPORT` ,
    - :code:`MAP_VARIABLES` ,
    - :code:`MATH` ,
    - :code:`MODEL` ,
    - :code:`RESET` ,
    - :code:`RESET_VALUE` ,
    - :code:`TEST_VALUE` ,
    - :code:`UNDEFINED` ,
    - :code:`UNIT` ,
    - :code:`UNITS` , and
    - :code:`VARIABLE` .

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

    .. literalinclude:: tutorial2_complete.py
        :language: python
        :start-at: #  2.c
        :end-before: #  end 2

.. code-block:: text

    Validator issue[0]:
        Description: Variable '1st' in component 'i_am_a_component' does not have a valid name attribute. CellML identifiers must not begin with a European numeric character [0-9].
        Type of item stored: VARIABLE
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.1
        See section 2.8.1.1 in the CellML specification.

    Validator issue[1]:
        Description: Variable 'b' in component 'i_am_a_component' does not have any units specified.
        Type of item stored: VARIABLE
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
        See section 2.8.1.2 in the CellML specification.

    Validator issue[2]:
        Description: Variable 'c' in component 'i_am_a_component' has an invalid initial value 'this_variable_doesnt_exist'. Initial values must be a real number string or a variable reference.
        Type of item stored: VARIABLE
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.2.2
        See section 2.8.2.2 in the CellML specification.

    Validator issue[3]:
        Description: Variable 'd' in component 'i_am_a_component' has a units reference 'i_dont_exist' which is neither standard nor defined in the parent model.
        Type of item stored: VARIABLE
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
        See section 2.8.1.2 in the CellML specification.

    Validator issue[4]:
        Description: MathML ci element has the child text 'a' which does not correspond with any variable names present in component 'i_am_a_component'.
        Type of item stored: MATH
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB12.html?issue=2.12.3
        See section 2.12.3 in the CellML specification.

Step 3: Fix the issues raised
-----------------------------
Now that we know what's wrong with the model the next steps are to fix it!
A useful feature of the :code:`Issue` items is as well as the textual information (which is more valuable to a *reader*), we also have a pointer to the item itself (which is more valuable to a *programmer* or user).
This section will work through the issues reported by the validator, and demonstrate different ways of accessing and repairing each of the problems.

.. container:: useful

    :api:`Issue class <Issue>`

    - :api:`item <Issue?fName=item>`
    - :api:`variable <Issue?fName=variable>`
    - :api:`math <Issue?fName=math>`

    :api:`Variable class <Variable>`

    - :api:`setName <Variable?fName=setName>`
    - :api:`setUnits <Variable?fName=setUnits>`
    - :api:`setInitialValue <Variable?fName=setInitialValue>`

    :api:`Model class <Model>`

    - :api:`component(name, True) <Model?fName=component>` Retrieving a component by its name with the optional second argument true will search the entire component tree for the component name.

The first issue raised involves the name of a variable.
Note that even though the name is invalid (as per CellML specification), it can still be used to access the item.
Our first step is to retrieve the badly named variable from the model, then we can use the :code:`setName` function to repair it.
You'll notice that the name of the component is given too.
Because component names are unique in the model, this means that we can use the combination of component name and variable name to retrieve the variable.
The :code:`component` function of the :code:`Model` class takes an optional second argument: this is a boolean indicating whether to search for the given component name in the model's top level components (:code:`false`, the default), or the entirety of the component tree (:code:`true`).

.. code-block:: text

    Validator issue[0]:
        Description: Variable '1st' in component 'i_am_a_component' does not have a valid name attribute. CellML identifiers must not begin with a European numeric character [0-9].
        Type of item stored: VARIABLE
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.1
        See section 2.8.1.1 in the CellML specification.

.. container:: dothis

    **3.a** Retrieve the variable named "1st" from the component named "i_am_a_component" and change its name to "a".

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial2_complete.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: //  end 3.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial2_complete.py
        :language: python
        :start-at: #  3.a
        :end-before: #  end 3.a

.. code-block:: text

    Validator issue[1]:
        Description: Variable 'b' in component 'i_am_a_component' does not have any units specified.
        Type of item stored: VARIABLE
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
        See section 2.8.1.2 in the CellML specification.

Inside the :code:`Issue` class are helper functions which allow you to access the item which needs to be fixed.
The naming of these functions is pretty straightforward, but there's a catch.
Not all of the "items" returned actually exist as independent libCellML entities; some are referenced by their parent item instead.
For example, calling the :code:`math()` function on an issue which reports storing an item with type :code:`MATH` returns a pointer to the component item that the maths sits within.
The functions and the types they return are shown below.

+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| enumeration value | function to call | type returned from function                                                                          |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| COMPONENT         | component()      | :code:`ComponentPtr` a pointer to a component.                                                       |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| COMPONENT_REF     | componentRef()   | :code:`ComponentPtr` a pointer to the component referenced via a :code:`component_ref`.              |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| CONNECTION        | connection()     | :code:`VariablePair` containing pointers to two :code:`VariablePtr` items which span the connection. |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| ENCAPSULATION     | encapsulation()  | :code:`ModelPtr` a pointer to the model containing the encapsulation.                                |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| IMPORT            | importSource()   | :code:`ImportSource` pointer to an import source item.                                               |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| MAP_VARIABLES     | mapVariables()   | :code:`VariablePair` containing the two :code:`VariablePtr` items connected by a variable            |
|                   |                  | equivalence.                                                                                         |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| MODEL             | model()          | :code:`ModelPtr` a pointer to a model.                                                               |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| RESET             | reset()          | :code:`ResetPtr` a pointer to a reset.                                                               |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| RESET_VALUE       | resetValue()     | :code:`ResetPtr` a pointer to the parent reset item.                                                 |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| TEST_VALUE        | testValue()      | :code:`ResetPtr` a pointer to the parent reset item.                                                 |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| UNIT              | unit()           | :code:`UnitPtr` a pointer to a unit item.                                                            |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| UNITS             | units()          | :code:`UnitsPtr` a pointer to a units item.                                                          |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+
| VARIABLE          | variable()       | :code:`VariablePtr` a pointer to a variable item.                                                    |
+-------------------+------------------+------------------------------------------------------------------------------------------------------+

.. container:: dothis

    **3.b** Retrieve the variable directly from the issue using the :code:`variable` function.
    Set its units to be "dimensionless".

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial2_complete.cpp
        :language: c++
        :start-at: //  3.b
        :end-before: //  end 3.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial2_complete.py
        :language: python
        :start-at: #  3.b
        :end-before: #  end 3.b

.. code-block:: text

    Validator issue[2]:
        Description: Variable 'c' in component 'i_am_a_component' has an invalid initial value 'this_variable_doesnt_exist'. Initial values must be a real number string or a variable reference.
        Type of item stored: VARIABLE
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.2.2
        See section 2.8.2.2 in the CellML specification.

For this next issue we're going to show how to use the generic :code:`item()` function on an issue.
This differs between C++ and Python so please refer to the appropriate tab for information.

.. tabs::

    .. tab:: C++

        In C++ we need to know the types of everything we're dealing with, all the time.
        A recent workaround is the provision of the :code:`std::any` type, which can be used to store an object of arbitrary type.
        The caveat is that in order to use it, you need to cast it back into its original type using :code:`std::any_cast`.
        The items in the :code:`Issue` class are stored as :code:`std::any` objects, and can either be retrieved and cast in one step using the functions listed above; or the :code:`std::any` pointer itself can be retrieved using the :code:`item()` function.

        You will need to also call the :code:`cellmlElementType()` function to verify the correct type to cast the item to.

        .. code-block:: c++

            //  Retrieve an issue pointer from the validator.
            auto myFirstIssue = validator->issue(0);

            // Retrieve the std::any item from the issue.
            auto anyItem = myFirstIssue->item();

            // Check the type of the item stored.  If you don't know ahead of time this would be a
            // switch statement to check them all.
            assert(myFirstIssue->cellmlElementType() == libcellml::CellmlElementType::VARIABLE);

            // Cast into a VariablePtr for use as normal.
            auto myVariable = std::any_cast<libcellml::VariablePtr>(anyItem);

    .. tab:: Python

        Since Python doesn't care about types the same way that C++ does, the :code:`item()` function will return the correct item.  
        No casting is needed!  
        It can still be useful to know the type that's returned, as your options for how to deal with it may vary.

        .. code-block:: python

            #  Retrieve an issue pointer from the validator.
            my_first_issue = validator.issue(0)

            # Retrieve the item from the issue.
            item = my_first_issue.item()

            # Check the type of the item stored.  If you don't know ahead of time this would be a
            # switch statement to check them all.
            assert(my_first_issue->cellmlElementType() == CellmlElementType.VARIABLE)

            # The item is available for use as a variable already.

.. container:: dothis

    **3.c** Retrieve the third issue and its item from the validator.
    This should be a :code:`VARIABLE` item, so in C++ you will need to cast it appropriately.
    Set the variable's initial conditions to 20.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial2_complete.cpp
        :language: c++
        :start-at: //  3.c
        :end-before: //  end 3.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial2_complete.py
        :language: python
        :start-at: #  3.c
        :end-before: #  end 3.c

.. code-block:: text

    Validator issue[3]:
        Description: Variable 'd' in component 'i_am_a_component' has a units reference 'i_dont_exist' which is neither standard nor defined in the parent model.
        Type of item stored: VARIABLE
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
        See section 2.8.1.2 in the CellML specification.

This error is similar in implication to that in 3.b: the validator is reporting that it can't find the units required by a variable.
It could be fixed in two different ways: either by supplying units called "i_dont_exist"; or by changing the name of the units which the variable requires to one that is available.

.. container:: dothis

    **3.d** Retrieve the units named "i_am_a_units_item" from the model, and set them to be used by variable "d". 

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial2_complete.cpp
        :language: c++
        :start-at: //  3.d
        :end-before: //  end 3

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial2_complete.py
        :language: python
        :start-at: #  3.d
        :end-before: #  end 3

This issue was actually also caught by the parser, which, like the validator, is a :code:`Logger` class.
This means that it will keep track of anything it encounters when parsing a model.
You can try calling the :code:`issueCount` and :code:`issue` functions on the parser and iterating through them (just like in 2.c) to see what you find.

.. code-block:: text

    Validator issue[4]:
        Description: MathML ci element has the child text 'a' which does not correspond with any variable names present in component 'i_am_a_component'.
        Type of item stored: MATH
        URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB12.html?issue=2.12.3
        See section 2.12.3 in the CellML specification.

As discussed earlier, the type of item stored doesn't always match the type of item returned.
In this final example, the type stored is :code:`MATH` but, according to the table above, the type returned from both the :code:`math()` and :code:`item()` functions is (after casting, if required) a :code:`ComponentPtr`.
We don't need to take action to resolve this issue, since our earlier change of the variable name to become "a" will have sorted out the problem already.

Step 4: Check and output the model
----------------------------------
Now that (we hope) the issues have been resolved, it's time to check that the model is free of validation errors.

.. container:: dothis

    **4.a** Validate the model again, and check that there are no more issues.

.. container:: dothis

    **4.b** Print the corrected model to the terminal so that you can see your changes.

.. container:: dothis

    **4.c** Just as you have done in :ref:`Tutorial 1<tutorial1>`, create a :code:`Printer` instance and use it to serialise the model into a string.
    Print the string to a .cellml file.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial2_complete.cpp
        :language: c++
        :start-at: //  4.a
        :end-before: //  end 4

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial2_complete.py
        :language: python
        :start-at: #  4.a
        :end-before: #  end 4

.. container:: dothis

    **4.d** Go and have a cuppa, you're done!
