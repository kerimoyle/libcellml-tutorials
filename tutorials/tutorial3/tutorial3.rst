.. _tutorial3:

Tutorial 3: Create a model and generate code using the API
==========================================================

.. container:: shortlist 

    By the time you have worked through Tutorial 3 you will be able to:

    - Create a new model and its child entities from scratch using the API;
    - Write your own MathML syntax to construct governing equations;
    - Define custom combinations of built-in units;
    - Define your own custom units independent from the built-in units; and
    - Use the :code:`Generator` functionality to transform the model into other languages.

.. container:: shortlist

    This tutorial assumes that you are comfortable with:

    - Accessing and adjusting names of items inside a model hierarchy (see :ref:`Tutorial 2<tutorial2>`);
    - Creating a validator and using it to check a model for errors (see :ref:`Tutorial 2<tutorial2>`);
    - Accessing the errors produced by a validator and using them to correct the model (see :ref:`Tutorial 2<tutorial2>`); and
    - Serialising and printing a model to a CellML2 file (see :ref:`Tutorial 1<tutorial1>`).

Requirements
------------
Either :download:`download the whole folder<tutorial3.zip>`, or:

.. container:: shortlist

    **C++**

    - :download:`CMakeLists.txt` The CMake file for building this tutorial;
    - :download:`tutorial3.cpp` The skeleton code; or
    - :download:`tutorial3_complete.cpp` the completed tutorial code; and
    - :download:`utilities.h<../utilities/utilities.h>` and :download:`utilities.cpp<../utilities/utilities.cpp>` Utility functions for use in the tutorials.

.. container:: shortlist

    **Python**
    
    - :download:`tutorial3.py` Either the skeleton code; or
    - :download:`tutorial3_complete.py` the completed tutorial code; and
    - :download:`utilities.py<../utilities/utilities.py>`  Utility functions for use in the tutorials.

.. contents:: Contents
    :local:
    
Overview
--------
During this tutorial you will create a simple model representing the population dynamics of two species - one a predator (sharks), and the other their prey (fish).
The population of fish can only grow when they are not being constantly eaten by sharks, and the rate at which is grows will depend on how many fish are available for breeding.
At the same time, the population of sharks will depend on how much food is available in the fish population.
In maths this relationship can be written:

.. math::

    \frac{dy_s}{dt} =f(sharks, fishes, time) = a y_s + b y_s y_f

    \frac{dy_f}{dt} =f(sharks, fishes, time) = c y_f + d y_s y_f

where the constants :math:`(a, b, c, d)=(-0.8, 0.3, 1.2, -0.6)` and we'll use the initial conditions of :math:`y_s(t=0)=1.0` and
:math:`y_f(t=0)=2.0`.

In order to model these unusual populations you'll need to create your own custom units, to enter and check these governing equations in MathML syntax, and to use the :code:`Generator` functionality to create files able to be solved using a numerical integrator in C++ or Python.

Step 1: Set up the governing equations
--------------------------------------
Just as you did in :ref:`Tutorial 2<tutorial2_cpp>`, we need to start by setting up a :code:`Model` instance, and creating a :code:`Component` inside it.

.. container:: dothis

    **1.a** Create a new :code:`Model` and give it a name.
    This can be done in a single step using the overloaded constructor with a name string as its argument.

.. container:: dothis

    **1.b** Create a new :code:`Component` with a name, and add it to the model you created in 1.a.


.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  1.a
        :end-before: //  1.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  1.a
        :end-before: #  1.c

Now for the real bit.
In order to actually model anything, we need to include the mathematical equations which represent the physical situation of interest.
As you saw in :ref:`Tutorial 2<tutorial2_cpp>`, the maths and the :code:`Variable` items which it references live inside a parent :code:`Component` item.

.. container:: gotcha

    At this point it should be noted that the *order* in which you add your components, or models, or variables (or anything) is not important to the final product, but it *can* affect how quickly you're able to find and fix bugs along the way.
    In these tutorials, we have suggested that you add the mathematics first and use a :code:`Validator` to notify you of the
    outstanding items, but you can really do this in any order you choose.

The system of equations which describe the populations are given by:

.. math::

    c = a + 2.0

    \frac{dy_s}{dt} =f(sharks, fish, time) = a y_s + b y_s y_f

    \frac{dy_f}{dt} =f(sharks, fish, time) = c y_f + d y_s y_f


where :math:`y_s` and :math:`y_f` are the number of sharks and thousands of fish respectively, and the constants :math:`(a, b, d)=(-0.8, 0.3, -0.6)` govern their behaviour.
It's clear that the value of constant :math:`c` is easily calculable from the first equation, but we will leave it in this form
to better illustrate the operation of the :code:`Analyser` later on.

In order to use this in our model we need to write it as a MathML2 string.
The basic structure for these is described in the :mathml2help:`W3 resource pages regarding MathML2 <>`.

It's highly unlikely that you - the user - will actually be required to write MathML2 code directly, so this part of the tutorial is more about understanding what's going on under the hood than practising fundamentally necessary skills.

.. container:: gotcha

    **Note** that libCellML will **only** accept MathML2 markup, even though later versions (3 and 4) are now available.

Looking at the top equation first, the MathML2 representation of :math:`c = a - 2.0` is:

.. code-block:: xml

    <apply><eq/>
       <ci>c</ci>
       <apply><plus/>
           <ci>a</ci>
           <cn>2.0</cn>
       </apply>
    </apply>

Four things can be seen here:

- The :code:`<apply>` opening and :code:`</apply>` closing tags which surround the *operations*;
- The *operations* tags like :code:`<eq/>` and :code:`<plus/>` (or :code:`<minus/>`, :code:`<times/>`, :code:`<divide/>`) which stand alone rather than in an open/close pair;
- The :code:`<ci>` opening and :code:`</ci>` closing tags which surround the variable names; and
- The :code:`<cn>` opening and :code:`</cn>` closing tags which surround the constant :math:`2.0` value.

.. container:: dothis

  **1.c** Create a string containing the MathML which represents equation 1 above.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  1.c
        :end-before: //  1.d

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  1.c
        :end-before: #  1.d

Differential terms, such as those on the left-hand side of equations 2 and 3 :math:`\frac{dx}{dt}` in MathML become:

.. code-block:: xml

    <apply><diff/>
        <bvar>
            <ci>t</ci>
        </bvar>
        <ci>x</ci>
    </apply>

Two further items to note:

- The base variable for the integration is identified by the :code:`<bvar> ... </bvar>` tags.
  These variables are referred to as *variables of integration*, *VOI* or *base variables*.
- The :code:`<diff/>` operation signifies differentiation with respect to the base variable.

The right-hand side becomes a collection of nested operations, all bracketed by :code:`<apply>...</apply>` tags for each operation:

.. code-block:: xml

    <apply><plus/>
      <apply><times/>
        <ci>a</ci>
        <ci>y_s</ci>
      </apply>
      <apply><times/>
        <ci>b</ci>
        <ci>y_s</ci>
        <ci>y_f</ci>
      </apply>
    </apply>

When both sides are defined we need to equate them by :code:`<apply>` -ing the :code:`<eq/>` equals operator, and turn it into a string.

.. container:: dothis

    **1.d** Create (or copy from the snippet below) the string representing equation 2 into your code.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  1.d
        :end-before: //  1.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  1.d
        :end-before: #  1.e

.. container:: dothis

    **1.e** Create (or copy from the snippet below) a third string representing equation 3.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  1.e
        :end-before: //  1.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  1.e
        :end-before: #  1.f

Next you need to define the namespace in which the maths will be applied by enclosing it in the :code:`<math> ... </math>` tags with the two namespaces:

- the MathML2 namespace :code:`xmlns` at http://www.w3.org/1998/Math/MathML
- the CellML2 namespace :code:`xmlns:cellml` at http://www.cellml.org/cellml/2.0#

It's simple to do this once in your code using a string to represent the opening attributes and namespaces; this string can be reused easily throughout your code as needed later.

.. container:: dothis

    **1.f** Create the strings representing the opening and closing tags of the maths block.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  1.f
        :end-before: //  1.g

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  1.f
        :end-before: #  1.g

.. container:: useful

    :api:`Component class<Component>`

    - setMath
    - appendMath

    Utility functions (C++)

    - printIssues will write information on any issues from a given Logger to the terminal.  
      Logger classes include the :code:`Validator`, :code:`Analyser`, and :code:`Parser`.

    Utility functions (Python)

    - print_issues will write information on any issues from a given Logger to the terminal.  
      Logger classes include the :code:`Validator`, :code:`Analyser`, and :code:`Parser`.

Our last step in defining the mathematics is to link it into the component.
The functions available to manipulate maths are:

- A :code:`setMath(yourMathsHere)` function, which overwrites any existing MathML strings stored in the :code:`Component` item; and
- An :code:`appendMath(yourMathsHere)` function, which performs a straightforward string concatenation with the current contents of the maths string in the :code:`Component`.
- There is no specific function to remove maths from a component, but this can be accomplished by using the :code:`setMath` function with an empty string.

.. container:: dothis

    **1.g** Use the functions above to include the MathML strings you've created into your component.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  1.g
        :end-before: //  1.h

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  1.g
        :end-before: #  1.h

.. container:: gotcha

    Note that the order in which strings are added might be important, as they are stored as a single concatenated string.
    However, the order in which complete MathML equations occur in the overall MathML string is not important.

.. container:: dothis

    **1.h** Create a :code:`Validator` and use it to check for errors in the model at this point.
    Use the utility function :code:`printIssues` to output the messages to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  1.h
        :end-before: //  end 1

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  1.h
        :end-before: #  end 1

You should see an output similar to that shown below:

.. code-block:: console

    Recorded 17 issues:

    Issue 0 is an ERROR:
        description: MathML ci element has the child text 'c' which does not correspond with any variable names present in component 'predator_prey_component'.
        see section 2.12.3 in the CellML specification.
        more information at: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB12.html?issue=2.12.3
        stored item type: MATH

    Issue 1 is an ERROR:
        description: MathML ci element has the child text 'a' which does not correspond with any variable names present in component 'predator_prey_component'.
        see section 2.12.3 in the CellML specification.
        more information at: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB12.html?issue=2.12.3
        stored item type: MATH

    Issue 2 is an ERROR:
        description: Math cn element with the value '2.0' does not have a valid cellml:units attribute. CellML identifiers must contain one or more basic Latin alphabetic characters.
        see section 2.13.4 in the CellML specification.
        more information at: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB13.html?issue=2.13.4
        stored item type: MATH
    
    ... etc ...

Running the validator will alert you variables in the MathML that don't (yet) exist in your component.
This was explained in :ref:`Tutorial 2<tutorial2>`, and we'll add them below.

Step 2: Create the variables
----------------------------

.. container:: useful

    :api:`Variable class<Variable>`

    - create
    - setName

    :api:`Component class<Component>`

    - addVariable

.. container:: dothis

    **2.a** Create :code:`Variable` items for each of the missing variables discovered above.
    Remember that:

    - Each must have a name, either using the naming constructor or by manually calling the :code:`setName` function; and
    - Each name must match what's used inside your MathML string.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: //  2.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  2.a
        :end-before: #  2.b

.. container:: dothis

    **2.b** Add each of your new variables to the component using the :code:`addVariable` function.

.. container:: dothis

    **2.c** Call the validator again to check for issues.
    At this stage you can expect errors like those below relating to missing units for the variables.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  2.b
        :end-before: //  end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  2.b
        :end-before: #  end 2

.. code-block:: console

    Recorded 8 issues:

    Issue 0 is an ERROR:
        description: Variable 'a' in component 'predator_prey_component' does not have any units specified.
        see section 2.8.1.2 in the CellML specification.
        more information at: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
        stored item type: VARIABLE
    
    ... etc ... 

Step 3: Built-in and customised units
-------------------------------------
Linking variables to the *name* of their units is straightforward, but in order to be able to use them we need to also define what the name actually *means* by creating the units themselves.
Some common units have been defined and built into libCellML, others you can define by combining the built-in ones using scaling factors and exponents, or you can define your own from scratch if need be.

.. container:: toggle

    .. container:: header

        Read more about units

    .. container:: infospec

        .. include:: ../../asides/aside_units.rst

.. container:: useful

    :api:`Units class<Units>`

    - create
    - setName
    - addUnit (note singular)

    :api:`Model class<Model>`

    - addUnits (note plural)
    - linkUnits

    :api:`Variable class<Variable>`

    - setUnits (note plural)

To create a :code:`Units` item you need will follow the same basic steps as other entities: declare it, name it, define it, and then add it in.
For example:

.. tabs::

    .. code-tab:: cpp

        // Declare, name, and define a "millisecond" unit pointer.
        auto ms = libcellml::Units::create("millisecond");

        // The manner of specification here is agnostic: all three definitions are identical.
        ms->addUnit("second", "milli");  reference unit and built-in prefix
        // OR
        ms->addUnit("second", 1.0, -3);  reference unit, prefix, exponent
        // OR
        ms->addUnit("second", 1.0, 0, 0.001);  reference unit, prefix, exponent, multiplier

    .. code-tab:: python

        from libcellml import Units

        # Declare, name, and define a "millisecond" unit pointer.
        ms = Units("millisecond")

        # The manner of specification here is agnostic: all three definitions are identical.
        ms.addUnit("second", "milli")          # reference unit and built-in prefix
        # OR
        ms.addUnit("second", 1.0, -3)          # reference unit, prefix, exponent
        # OR
        ms.addUnit("second", 1, 1.0, 0.01)     # reference unit, prefix, exponent, multiplier

.. container:: dothis

    **3.a** Use the example above to create, name and define the units of "month" which will represent your time variable.
    This should be defined as a multiple of the built-in unit :code:`second`.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: //  3.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  3.a
        :end-before: #  3.b

Units can be defined based on one another as well.
For example, after defining our :code:`millisecond` units, we could then use this definition to define the :code:`per_millisecond` units by simply including it with an exponent of -1:

.. code-block:: cpp

    Define a per_millisecond unit based on millisecond^-1:
    per_ms->addUnit(ms, -1.0);

.. container:: dothis

    **3.b** Create a :code:`Units` item called "per_month" based on the one you just created, as shown above.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  3.b
        :end-before: //  3.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  3.b
        :end-before: #  3.c

.. container:: dothis

    **3.c** Create the irreducible units needed by the shark and fish populations.
    Call these "number_of_sharks" and "thousands_of_fish" respectively.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  3.c
        :end-before: //  3.d

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  3.c
        :end-before: #  3.d

Finally we need to create the units for the constants :code:`b` and :code:`d`.
These will be combinations of those which we've already created, as defined by the need for dimensional consistency in our governing equations.

.. container:: dothis

    **3.d** Create two units representing "per shark month" (for the :code:`b` variable) and "per fish month" (for the :code:`d` variable).

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  3.d
        :end-before: //  3.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  3.d
        :end-before: #  3.e

The final two steps are to associate each variable with its appropriate units, and to include the units in the model.

.. container:: gotcha

    - When you add different sub-unit parts into a :code:`Units` item, the function is :code:`addUnit` (singular), and it takes as argument the *name* of the sub-unit as a string (eg: :code:`"second"` used above).
    - When you add the final created combination into the :code:`Model` item, the function is :code:`addUnits` (plural), and it takes as argument the *reference* of the combined units (eg: :code:`ms`).

.. container:: dothis

    **3.e** Add the units to their variables using :code:`myVariable->setUnits(myUnits)`.
    Add the units to the model using :code:`myModel->addUnits(myUnits)`.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  3.e
        :end-before: //  3.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  3.e
        :end-before: #  3.f

.. container:: gotcha

    **Gotcha!** When you specify the :code:`Units` for a :code:`Variable` using its name then you may need to call the model's :code:`linkUnits` function before validating the model.
    If you see errors related to missing units which do in fact exist, this indicates that a call to the :code:`linkUnits` function is needed.

.. container:: dothis

    **3.f** Call the validator to check your model for errors.
    You should see an output similar to that shown below.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  3.f
        :end-before: //  3.g

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  3.f
        :end-before: #  3.g

.. code-block:: console

    Recorded 1 issues:

    Issue 0 is an ERROR:
        description: Math cn element with the value '2.0' does not have a valid cellml:units attribute. CellML identifiers must contain one or more basic Latin alphabetic characters.
        see section 2.13.4 in the CellML specification.
        more information at: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB13.html?issue=2.13.4
        stored item type: MATH

In the first MathML equation we used a real number :code:`<cn>2.0</cn>` without specifying any units for it.
Because the dimensionality of the equation needs to be valid, all real numbers must be associated with units, just the same way that variables are.
These are defined within the tags of the MathML, and must also refer to the :code:`cellml` namespace.  For example:

.. code-block:: xml

    <cn cellml:units="bunch_of_bananas">1</cn>

... which gives us one bunch of bananas, without needing to create a corresponding :code:`Variable` item.
Of course, you may need to create the corresponding :code:`Units` item and add it to the model, if it is not already present.

.. container:: dothis

    **3.g**  Create a copy of the MathML statement from step 1.c and add the namespace and units definition as in the example above into the string.
    Recall that using the :code:`setMath()` function will overwrite the existing maths, and repeat the process you did in step 1.e to include the new MathML instead.
    Remember that you will need to re-include the opening and closing :code:`<math>` tags as well as the other equations.

.. container:: dothis

    **3.h** Check that the model is now free of validation errors.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  3.g
        :end-before: //  end 3

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  3.g
        :end-before: #  end 3

Step 4: Analyse the mathematical model
--------------------------------------
The :code:`Analyser` class checks that the underlying mathematical model represented by the entire combination of variables, components, and mathematics, makes sense.
The :code:`Validator` checks your "spelling" and syntax, and the :code:`Analyser` checks for the ability to find a solution.

.. container:: useful

    :api:`Analyser class<Analyser>`

    - create
    - analyseModel
    - model

.. container:: dothis

    **4.a** Create an :code:`Analyser` instance and pass it the model using the :code:`analyseModel` function.  

.. container:: dothis

    **4.b** Check for issues found in the analyser.
    You should expect 6 errors, related to variables whose values are not computed or initialised.
    Note that you can use the same utility function :code:`printIssues` to output issues from the analyser as from the validator.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  4.a
        :end-before: //  4.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  4.a
        :end-before: #  4.c

The messages above refer to the fact that though our model has passed validation tests, it's not yet sufficiently constrained to allow it to be solved, which is what the :code:`Generator` checks for.
We need to set initial conditions for the variables we're solving for, the populations of sharks and fish, using the :code:`setInitialValue` function.
The values of the constants :code:`a, b, c, d` are just that - constant - and their values are set using the same :code:`setInitialValue` function.

.. container:: dothis

    **4.c** Set the values of the constants :math:`(a, b, d)=(-0.8, 0.3, -0.6)` and the initial conditions such that :math:`y_f(t=0)=2.0` and :math:`y_s(t=0)=1.0`.
    Note that:

    - The constant :math:`c` will be calculated by our equation 1, so does not need to be specified; and
    - The base variable (or "variable of integration", or "voi") :math:`t` must *not* have an initial condition set.

.. container:: dothis

    **4.d** Reprocess the model and check that the analyser is now free of issues.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at:  //  4.c
        :end-before:  //  end 4

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  4.c
        :end-before: #  end 4

Step 5: Generate code and output
--------------------------------
Some exciting new functionality in libCellML is the ability to generate a runnable file from a model description.
This means that if you already have a solver in either C or Python, you can simply translate your model from here into that language.

The :code:`Generator` has to re-interpret all of the maths, including the variables, their interaction with each other in different equations, values, initial conditions and units before it can output your model in your choice of language.
For the maths to make sense, the definitions in your model's variables, maths blocks and units need to be solvable too.
There are several requirements that need to be satisfied in order for the code generation functionality to be able to work, *beyond* the CellML syntax requirements.
These are:

- The mathematical model definition must be appropriately constrained (not over- or under-constrained);
- Initial conditions must be specified for variables which are integrated; 
- Initial conditions must not be specified for variables which are the base of integration;
- The values of constants must be specified or calculable; and
- **TODO get full list of stuff here ...**

.. container:: useful

    :api:`Generator class<Generator>`

    - create
    - profile
    - setModel
    - interfaceCode
    - implementationCode

    :api:`GeneratorProfile class<GeneratorProfile>`

    - create
    - setInterfaceFileNameString
    
    The GeneratorProfile class contains an enum indicating the language of profile to set.
    In C++ this is :code:`GeneratorProfile::Profile`.
    In Python this is :code:`GeneratorProfile.Profile`.

    At the time of writing two profiles are available:

    - :code:`C` (default)
    - :code:`PYTHON`

.. container:: dothis

    **5.a** Create a :code:`Generator` instance.
    Instead of giving it the model to process, the generator needs an analysed model as its argument.  
    Retrieve the analysed model using the analyser's :code:`model` function and pass it to the generator using the :code:`setModel` function.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  5.a
        :end-before: //  5.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  5.a
        :end-before: #  5.b

The generator takes the CellML model and turns it into procedural code in another language.
The default is C, but Python is available too.
This language choice is called the "profile", and is stored in a :code:`GeneratorProfile` item.

The default profile already exists inside the :code:`Generator` you've just created.
We need to edit that profile a little, but only to tell it the file name where they interface (header file) code will be written.
This is so that the implementation code (source file) knows where to look when it tries to include it.  

.. container:: dothis

    **5.b** Retrieve the C profile from the generator, and use its :code:`setInterfaceFileNameString` function to pass in the same filename that you'll use in 5.c below for the interface code.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  5.b
        :end-before: //  5.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  5.b
        :end-before: #  5.c

.. container:: dothis

    **5.c** Since we're using the default profile (C), we need to output both the interface code (the header file) and the implementation code (the source file) from the generator and write them to their respective files.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  5.c
        :end-before: //  5.d

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  5.c
        :end-before: #  5.d

The generator takes the CellML model and turns it into procedural code in another language.
The default is C, but Python is available too.
This language choice is called the "profile", and is stored in a :code:`GeneratorProfile` item.

.. container:: dothis
    
    **5.d** Create a :code:`GeneratorProfile` item using the Profile::PYTHON enum value in the constructor.
    Pass this profile to the :code:`setProfile` function in the generator.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  5.d
        :end-before: //  5.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  5.d
        :end-before: #  5.e

.. container:: dothis
    
    **5.e** Retrieve the Python implementation code (there is no header file) and write to a :code:`*.py` file.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: tutorial3_complete.cpp
        :language: c++
        :start-at: //  5.e
        :end-before: //  end 5

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: tutorial3_complete.py
        :language: python
        :start-at: #  5.d
        :end-before: #  end 5

.. container:: dothis
    
    Go and have a cuppa, you're done!
