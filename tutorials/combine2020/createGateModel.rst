..  _combine_createGateModel:

Tutorial 1: Creating a model using the API
==========================================

.. container:: shortlist

    By the time you have worked through this tutorial you will be able to:

    - Assemble a model using the API; 
    - Use the diagnostic Validator class to identify errors in the model's syntax; 
    - Use the diagnostic Analyser class to identify errors in the model's mathematical construction; and
    - Serialise the model to CellML format for output.

.. container:: shortlist

    **Requirements (C++)**

    - :download:`CMakeLists.txt<CMakeLists_GateModel.txt>` The CMake file for building this tutorial;
    - :download:`createGateModel.cpp<createGateModel.cpp>` Either the skeleton code, or ..
    - :download:`createGateModel_completed.cpp<createGateModel_completed.cpp>` the completed tutorial code.

.. container:: shortlist

    **Requirements (Python)**

    - :download:`createGateModel_completed.py<createGateModel_completed.py>` Either the skeleton code, or ..
    - :download:`createGateModel_completed.py<createGateModel_completed.py>` the completed tutorial code.

.. contents:: Contents
    :local:

Overview
--------
This is the first tutorial in a series designed to walk the user through some of libCellML's functionality available in the API.
Its goal is to create from scratch a voltage-independent ion gate model.
The theory of this kind of gate can be found on the :ref:`Ion Gate theory page<theory_iongates>`.

One of the goals of the CellML format (which must be supported by libCellML) is the construction of models that are reusable.
Throughout these tutorials we will create entities in such a way as to enable their reuse as easily as possible.  
The basic structure of this model highlights that too.

.. container:: shortlist

    We will create a model with three components:
    - An *equations* component, which contains all of the working pieces and mathematics specific to the model;
    - A *parameters* component, which contains any hard-coded parameters or values specific to this model; and
    - A wrapper component, which is the parent of the other two.

This arrangement means that it's simple to import this model into others, as well as to locate or over-ride parameter values or reuse equations.
Each of the components created throughout this series of tutorials will follow this same structure.

Step 1: Set up the model
------------------------

.. container:: useful

    **Useful functions**

    :api:`Model class<Model>`

    - create
    - setName
    - addComponent

    :api:`Component class<Component>`

    - create
    - setName
    - addComponent

.. container:: dothis

    **1.a** The first step is to create a `Model` item which will later contain the component and the units it needs.
    Use the :code:`Model` create function to make a new instance.

.. container:: dothis

    **1.b** Each CellML element must have a name, which is set using the :code:`setName` function.
    Set the name of the model to be "GateModel".
    
We'll create a wrapper component whose only job is to encapsulate the other components.
This makes is a lot easier for this model to be reused, as the connections between components internal to this one won't need to be re-established.
Note that the constructor for all named CellML entities is overloaded, so you can pass it the name string at the time of creation.

.. container:: dothis

    **1.c** Create a new :code:`Component` item named "gate" using the overloaded constructor.

Finally we need to add the component to the model.  
This sets it at the top-level of the component encapsulation hierarchy.
All other components will then need to be added to this component, rather than to the model.

.. container:: dothis

    **1.d** Add the component to the model using the :code:`addComponent` function.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  1.a
        :end-before: // end 1

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  1.a
        :end-before: # end 1

Step 2: Create the gateEquations component
------------------------------------------
Inside the wrapper component you created in Step 1 we need to create two more: an equations component, and a parameters component.
In this step we'll construct the equations component.

.. container:: dothis

    **2.a** Create a new equations component named "gateEquations".

.. container:: dothis

    **2.b** Add the new gateEquations component to the gate component.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: //  2.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  2.a
        :end-before: #  2.c

Since this is an *equations*-flavoured component, it should contain the bulk of the calculations and mathematics for the gate.
Maths is added using MathML2 (no other levels are supported) strings.  

In this example we need to represent just one equation:

.. math::

    \frac{dX}{dt} = \alpha_{X}\left( 1 - X \right) - \beta_{X}.X

If you're happy to write your own MathML2 string then please go ahead, but if you'd rather not you can use the code provided under the code toggles further down the page.

.. container:: useful

    **Useful functions**

    :api:`Component class<Component>`

    - :code:`setMath`
    - :code:`appendMath`
    - :code:`math`

.. container:: dothis

    **2.c** Construct a string representing the MathML of the equation above.
    You will need to enclose the string with the appropriate header and footer.
    These are provided for you in the skeleton code, or simply copy them from below.
    Use the :code:`setMath` and :code:`appendMath` functions to add your strings to the equations component.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  2.c
        :end-before: // end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  2.c
        :end-before: # end 2

Step 3: Validate the model
--------------------------
Once the mathematics has been added to the component, and the component to the model, we can make use of the diagnostic messages within the :code:`Validator` class to tell us what else needs to be done.

.. container:: useful

    **Useful functions**

    :api:`Validator class<Validator>`

    - create
    - validateModel
    - issueCount
    - issue

    :api:`Issue class<Issue>`

    - description
    - url
    - referenceHeading
    - level
    - itemType

.. container:: dothis

    **3.a** Create a Validator instance, and pass it your model for processing using the :code:`validateModel` function.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: // end 3.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  3.a
        :end-before: # end 3.a

Calling the validator does not return anything: we have to go looking for issues that it found during processing.

.. container:: shortlist

    When a problem is found, an :code:`Issue` item is created containing:
    - a description string explaining the problem;
    - a URL at which more information is available;
    - an std::any item relevant to the problem, if available;
    - a level indicator; and
    - a cause indicator relevant to the stored item.

We can use these issues as we need to.
The simplest way is to print the descriptions to the terminal.

Two helper functions have been provided for this tutorial that will help printing enumerated values to the terminal.  These are:

.. container:: shortlist

    - getIssueLevelFromEnum; and
    - getItemTypeAsString. 

.. container:: dothis

    **3.b** Retrieve the number of issues encountered using the :code:`validator->issueCount()` function, then retrieve the issue items from the validator using their index and the validator->issue(index)function.
    Print the information from each issue to the terminal.
    
.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  3.b
        :end-before: //  3.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  3.b
        :end-before: #  3.c

.. code-block:: terminal

    The validator has found 6 issues.
    Issue [0] is an ERROR:
        description: MathML ci element has the child text 't' which does not correspond with any variable names present in component 'gateEquations'.
        see section 2.12.3 in the CellML specification.
        stored item type: MATHML
    Issue [1] is an ERROR:
        description: MathML ci element has the child text 'X' which does not correspond with any variable names present in component 'gateEquations'.
        see section 2.12.3 in the CellML specification.
        stored item type: MATHML
    Issue [2] is an ERROR:
        description: MathML ci element has the child text 'alpha_X' which does not correspond with any variable names present in component 'gateEquations'.
        see section 2.12.3 in the CellML specification.
        stored item type: MATHML
    Issue [3] is an ERROR:
        description: MathML ci element has the child text 'X' which does not correspond with any variable names present in component 'gateEquations'.
        see section 2.12.3 in the CellML specification.
        stored item type: MATHML
    Issue [4] is an ERROR:
        description: MathML ci element has the child text 'beta_X' which does not correspond with any variable names present in component 'gateEquations'.
        see section 2.12.3 in the CellML specification.
        stored item type: MATHML
    Issue [5] is an ERROR:
        description: MathML ci element has the child text 'X' which does not correspond with any variable names present in component 'gateEquations'.
        see section 2.12.3 in the CellML specification.
        stored item type: MATHML

Step 4: Add the variables
-------------------------
The issues reported by the validator are related to the MathML string that we entered in Step 2 requiring variables which don't yet exist.
These must be created, named, and added to their parent component.

.. container:: useful

    **Useful functions**

    :api:`Variable class<Variable>`

    - create
    - setName
    - setUnits

    :api:`Component class<Component>`

    - addVariable

.. container:: dothis

    **4.a** Create :code:`Variable` items whose names match those listed above.  
    Add these missing variables to the equations component.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  4.a 
        :end-before: //  4.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  4.a
        :end-before: #  4.b

.. container:: dothis

    **4.b** Validate the model again.
    Note that you can use the helper function :code:`printIssues` with the validator as the argument to save repeating the code from 3.b each time.
    Expect errors relating to missing units.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  4.b 
        :end-before: //  end 4

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  4.b
        :end-before: #  end 4

.. code-block:: terminal

    Issue [0] is an ERROR:
        description: CellML identifiers must contain one or more basic Latin alphabetic characters.
        see section 1.3.1.1 in the CellML specification.
        stored item type: UNDEFINED
    Issue [1] is an ERROR:
        description: Variable 't' in component 'gateEquations' does not have a valid units attribute. The attribute given is ''.
        see section 2.8.1.2 in the CellML specification.
        stored item type: VARIABLE
    ... etc ...

Step 5: Add the units
---------------------
The validator has reported that the four variables are missing units attributes.  
In this example none of the units exist yet, so we need to create all of them. 

.. container:: shortlist

The variables' units should be:

- t, time has units of *milliseconds*
- X, gate status has units of *dimensionless*
- alpha_X and beta_X, rates, have units of *per millisecond*.

.. container:: useful

    **Useful functions**

    :api:`Units class<Units>`

    - create
    - setName
    - addUnit
    - name

    :api:`Model class<Model>`

    - addUnits

    :api:`Variable class<Variable>`

    - setUnits
    - units

.. container:: dothis

    **5.a** Create two new :code:`Units` items with names of "ms" and "per_ms".
    These will represent units of milliseconds and per millisecond respectively.
    
Some basic units have been defined and built into libCellML, others you can define by combining the built-in ones using scaling factors and exponents, or you can define your own from scratch if need be.
Please read the :ref:`Understanding units<aside_units>` page for more detailed information, but for now let's look at a simple example that defines a :code:`Units` item representing millivolts.

.. tabs::

    .. code-tab:: c++

        // Declare, name, and define a millivolt units item.
        auto mV = libcellml::Units::create("mV");

        // The manner of specification here is agnostic: all three definitions are identical.
        mV->addUnit("volt", "milli");  // reference unit, built-in prefix string
        // OR
        mV->addUnit("volt", -3);       // reference unit, prefix as an integer
        // OR
        mV->addUnit("volt", 1.0, 1, 0.001);  // reference unit, prefix, exponent, multiplier

    .. code-block:: py

        from libcellml import Units

        # Declare, name, and define a millivolt units item.
        mV = Units("mV")

        # The manner of specification here is agnostic: all three definitions are identical.
        mV.addUnit("second", "milli")   # reference unit and built-in prefix
        # OR
        mV.addUnit("second", -3)        # reference unit, prefix
        # OR
        mV.addUnit("second", 1, 1.0, 0.001)   # reference unit, prefix, exponent, multiplier

.. container:: dothis

    **5.b** Following the example above, define the units of millisecond and per millisecond by adding the appropriate unit items.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  5.a 
        :end-before: //  5.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  5.a
        :end-before: #  5.c

.. container:: dothis

    **5.c** Add the units to the model (not the component) so that other components can make use of them too.

.. container:: dothis

    **5.d** Use the :code:`setUnits` function to associate the units you've created with the appropriate variables.

.. container:: dothis

    **5.e** Validate again, and expect no errors.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  5.c
        :end-before: //  end 5

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  5.c
        :end-before: #  end 5

Step 6: Analyse the mathematical construction of the model
----------------------------------------------------------

.. container:: useful

    **Useful functions**

    :api:`Analyser class<Analyser>`

    - create
    - analyseModel
    - issueCount
    - issue

.. container:: dothis

    **6.a** Create an :code:`Analyser` item and submit the model for processing. 

.. container:: dothis

    **6.b** Just like the Validator class, the Analyser class keeps track of issues. 
    Retrieve these and print to the terminal using the same helper function as earlier.
    Expect errors related to un-computed variables and missing initial values.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  6.a
        :end-before: //  end 6.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  6.a
        :end-before: #  end 6.b

.. code-block:: terminal

    Recorded 3 issues:
    Issue [0] is an ERROR:
        description: Variable 'X' in component 'gateEquations' is used in an ODE, but it is not initialised.
        stored item type: VARIABLE
    Issue [1] is an ERROR:
        description: Variable 'alpha_X' in component 'gateEquations' is not computed.
        stored item type: VARIABLE
    Issue [2] is an ERROR:
        description: Variable 'beta_X' in component 'gateEquations' is not computed.
        stored item type: VARIABLE

In order to avoid hard-coding values here, we will need to connect to external values to initialise the "X" variable and provide the value for "alpha_X" and "beta_X".

.. container:: shortlist

    This means three things need to happen:
    - we need to create an external component to hold variable values;
    - we need to create external variables in that component; 
    - we need to specify the connections between variables; and
    - we need to permit external connections on the variables.

This is the reason for the second internal component, the *parameters* component.

.. container:: dothis

    **6.c** Create a component which will store the hard-coded values for initialisation.
    Name it "gateParameters", and add it to the top-level gate component as a sibling of the gateEquations component.

.. container:: useful

    **Useful functions**

    :api:`Variable class<Variable>`

    - setInitialValue
    - addEquivalence

    :api:`Component class<Component>`

    - variable

    :api:`Model class<Model>`

    - component

.. container:: dothis

    **6.d** Create appropriate variables in this component, and set their units.
    Use the :code:`setInitialValue` function to initialise the variables.
    **TODO** What values to give?

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  4.c
        :end-before: //  4.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  4.c
        :end-before: #  4.e

So far in this tutorial we've only been creating items, defining them, and adding to their parent items.
Now for the first time we will need to retrieve a child item from its parent.
This can be done in one of two ways: either by the child's index or by its name.
There is more information about interacting with collections of items on the :ref:`Understanding collections of items<examples_understand_collections>` page.

Two particularly useful idioms are shown below.

.. tabs::

    .. code-tab:: c++

        // Retrieve Units named "myUnits" from a model and set as the units for a variable named "myVariable".
        myVariable->setUnits(myModel->units("myUnits"));

        // Retrieve a great-grandchild component by following the hierarchy of the encapsulation structure:
        auto grandson = model->component("grandfather")->component("daddy")->component("son");

        // Short-cut to retrieve the component with the given name from anywhere in the encapsulation hierarchy:
        auto granddaughter = model->component("granddaughter", true);

    .. code-tab:: py

        # Retrieve Units named "myUnits" from a model and set as the units for a variable named "myVariable".
        myVariable.setUnits(myModel.units('myUnits'))

        # Retrieve a great-grandchild component by following the hierarchy of the encapsulation structure:
        grandson = model->component('grandfather')->component('daddy')->component('son');

        # Short-cut to retrieve the component with the given name from anywhere in the encapsulation hierarchy:
        granddaughter = model->component('granddaughter', true)

.. container:: dothis

    **6.e** Specify a variable equivalence between the gate equations variables and the parameter variables of the same name.
    Validate the model again, expecting errors related to the variable interface types.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  4.e
        :end-before: //  4.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  4.e
        :end-before: #  4.f

.. code-block:: terminal

    Recorded 6 issues:
    Issue [0] is an ERROR:
        description: Variable 'alpha_X' in component 'gateEquations' has no interface type set. The interface type required is 'public'.
        see section 3.10.8 in the CellML specification.
        stored item type: VARIABLE
    ... etc ...

.. container:: useful

    **Useful functions**

    :api:`Model class<Model>`

    - fixVariableInterfaces

    :api:`Variable class<Variable>`

    - setInterfaceType

.. container:: dothis

    **6.f** Set the variable interface type according to the recommendation from the validator.
    This can either be done individually using the :code:`setInterfaceType` function on each variable, or en masse for all the model's variable interfaces using its :code:`fixVariableInterfaces` function.
    Validate and analyse again, expecting no errors. 

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  6.e
        :end-before: //  end 6.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  6.e
        :end-before: #  end 6.f

Step 7: Sanity check
--------------------

.. container:: useful

    **Useful functions**

    There's a helper function provided for these tutorials which will print the model to the terminal.
    Use :code:`printModel(Model, bool)` function to print the contents of the given :code:`Model`.
    The second optional argument indicates whether or not to print the MathML strings in the components too.

.. container:: dothis

    **7.a** Print the model to the terminal using the helper function :code:`printModel`.

.. code-block:: terminal

    MODEL: 'GateModel'
    UNITS: 2 custom units
        [0]: ms
        [1]: per_ms
    COMPONENTS: 1 components
        [0]: gate
            VARIABLES: 0 variables
            COMPONENT gate has 2 child components:
                [0]: gateEquations
                    VARIABLES: 4 variables
                        [0]: t [ms]
                        [1]: alpha_X [per_ms]
                              └──> gateParameters:alpha [per_ms]
                        [2]: beta_X [per_ms]
                              └──> gateParameters:beta [per_ms]
                        [3]: X [dimensionless]
                              └──> gateParameters:X [dimensionless]
                [1]: gateParameters
                    VARIABLES: 3 variables
                        [0]: X [dimensionless], initial = 0
                              └──> gateEquations:X [dimensionless]
                        [1]: alpha [per_ms], initial = 0.1
                              └──> gateEquations:alpha_X [per_ms]
                        [2]: beta [per_ms], initial = 0.5
                              └──> gateEquations:beta_X [per_ms]

Looking at the printout we see that the top-level component named "gate" has no variables.  
Even though this is clearly a valid situation (as proved by 6.f), it's not
going to make this model easy to reuse.
We need to make sure that any input and output variables are also connected into the top level gate component.  

.. container:: dothis

    **7.b** Create intermediate variables for time t and gate status X in the gate component, and ensure they have a public and private interface to enable two-way connection.
    You will also need to set a public and private connection onto t and X in the equations component too, or repeat the call to fix the model's interfaces as in step 6.f.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  7.b
        :end-before: //  7.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  7.b
        :end-before: #  7.c

.. container:: dothis

    **7.c** Connect the intermediate variables to their respective partners in the equations component, and recheck the model.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  7.c
        :end-before: //  end 7

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  7.c
        :end-before: #  end 7

Step 8: Serialise and output the model
--------------------------------------
The :code:`Printer` class in libCellML takes the stored instance of a :code:`Model` item and creates a string representing its serialisation into CellML code.  

.. container:: dothis

    **8.a** Create a :code:`Printer` instance and use it to serialise the model into a string.
    Write this string to a file called "GateModel.cellml".

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.cpp
        :language: c++
        :start-at: //  8.a
        :end-before: //  end 8

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel_completed.py
        :language: python
        :start-at: #  8.a
        :end-before: #  end 8

