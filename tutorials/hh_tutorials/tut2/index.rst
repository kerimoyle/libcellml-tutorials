..  _createPotassiumChannelModel:

Hodgkin-Huxley 2: Creating a model which uses imports
=====================================================

.. container:: shortlist

    By the time you have worked through this tutorial you will be able to:

    - Assemble a multi-component model using the API; 
    - Create a model which imports items from others;
    - Inter-connect the components using the equivalent variables functionality;
    - Use the diagnostic :code:`Validator` class to identify errors in the model's syntax; 
    - Use the diagnostic :code:`Analyser` class to identify errors in the model's mathematical formulation; and
    - Serialise the model to CellML format for output.

.. container:: shortlist

    **Code (C++)**

    - :download:`CMakeLists.txt<CMakeLists.txt>` The CMake file for building this tutorial;
    - :download:`createPotassiumChannelModel.cpp<createPotassiumChannelModel.cpp>` Either the skeleton code, or ..
    - :download:`createPotassiumChannelModel_completed.cpp<createPotassiumChannelModel_completed.cpp>` the completed tutorial code; and
    - :download:`utilities.cpp<../code/utilities.cpp>` and :download:`utilities.h<../code/utilities.h>` Helper functions.

.. container:: shortlist

    **Code (Python)**

    - :download:`createPotassiumChannelModel.py<createPotassiumChannelModel.py>` Either the skeleton code, or ..
    - :download:`createPotassiumChannelModel_completed.py<createPotassiumChannelModel_completed.py>` the completed tutorial code.

.. container:: shortlist

    **Resources**

    - :download:`GateModel.cellml<GateModel.cellml>` The generic gate model created in :ref:`Tutorial 1<createGateModel>`.

.. contents:: Contents
    :local:

Step 1: Create the model
------------------------
The final structure of this model is shown in the diagram below.

.. code-block:: text

    model: PotassiumChannelModel
        component: controller <-- imported from PotassiumChannelController.cellml, component 'controller'
        component: potassiumChannel
            component: potassiumChannelEquations
                    component: nGateEquations
                        component: gateEquations <-- imported from GateModel.cellml, component 'gateEquations'
                    component: nGateParameters <-- created here so that the parameters are specific to the nGateEquations.
            component: kChannelParameters


.. container:: useful

    :api:`Model class <Model>`

    - :api:`create <Model?fName=create>`
    - :api:`setName <Model?fName=setName>`
    - :api:`addComponent <Model?fName=addComponent>`

    :api:`Component class <Component>`

    - :api:`create <Component?fName=create>`
    - :api:`setName <Component?fName=setName>`
    - :api:`addComponent <Component?fName=addComponent>`

.. container:: dothis

    **1.a** Create a :code:`Model` item and name it appropriately.

.. container:: dothis

    **1.b** Create a wrapping component and name it "potassiumChannel".

.. container:: dothis

    **1.c** Add the component to the model.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  1.a
        :end-before: //  end 1

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  1.a
        :end-before: #  end 1

Step 2: Create a potassium channel equations component
------------------------------------------------------

.. container:: useful

    :api:`Component class <Component>`

    - :api:`create <Component?fName=create>`
    - :api:`setName <Component?fName=setName>`
    - :api:`setMath <Component?fName=setMath>`
    - :api:`appendMath <Component?fName=appendMath>`

.. container:: dothis

    **2.a** Create a :code:`Component` instance for the equations and name it "potassiumChannelEquations".  
    Add it to the wrapper component you created above.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: //  end 2.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  2.a
        :end-before: #  end 2.a

.. container:: dothis

    **2.b** Define the maths inside the potassiumChannelEquations component.
    This is a MathML 2 string (NB: higher versions of MathML are not supported), and is added to the component using :code:`setMath` and :code:`appendMath` functions.
    Your string needs to contain the namespaces for MathML and for CellML: these have been provided for you in the mathHeader string above.
        
.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.b
        :end-before: //  2.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  2.b
        :end-before: #  2.c
        
Once the mathematics has been added to the component, and the component to the model, we can make use of the diagnostic messages within the :code:`Validator` class to tell us what else needs to be done.

.. container:: useful

    :api:`Validator class <Validator>`

    - :api:`create <Validator?fName=create>`
    - :api:`validateModel <Validator?fName=validateModel>`
    - :api:`issueCount <Validator?fName=issueCount>`
    - :api:`issue <Validator?fName=issue>`

.. container:: dothis

    **2.c** Create a :code:`Validator` instance, and pass it your model for processing using the :code:`validateModel` function.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.c
        :end-before: //  end 2.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  2.c
        :end-before: #  end 2.c

Calling the validator does not return anything: we have to go looking for issues that it found during processing.
When a problem is found, an :code:`Issue` item is created containing:

.. container:: shortlist

    - a description string explaining the problem;
    - a URL at which more information is available;
    - an std::any item relevant to the problem, if available;
    - a level indicator; and
    - a cause indicator relevant to the stored item.

We can use these issues as we need to.
The simplest way is to print the descriptions to the terminal.

.. container:: dothis

    **2.d** Retrieve the number of issues encountered using the :code:`issueCount` function in the validator, then retrieve the issue items from the validator using their index and the :code:`issue(index)` function.
    Print their information to the terminal.  
    (Alternatively use the helper :code:`printIssues` or :code:`print_issues` function to do it for you).

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.d
        :end-before: //  2.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  2.d
        :end-before: #  2.e

.. code-block:: text

    The validator has found 5 issues.
    MathML ci element has the child text 'i_K' which does not correspond with any variable names present in component 'potassiumChannelEquations'.
    MathML ci element has the child text 'n' which does not correspond with any variable names present in component 'potassiumChannelEquations'.
    MathML ci element has the child text 'g_K' which does not correspond with any variable names present in component 'potassiumChannelEquations'.
    MathML ci element has the child text 'V' which does not correspond with any variable names present in component 'potassiumChannelEquations'.
    MathML ci element has the child text 'E_K' which does not correspond with any variable names present in component 'potassiumChannelEquations'.

.. container:: useful

    :api:`Variable class <Variable>`

    - :api:`create <Variable?fName=create>`
    - :api:`setUnits <Variable?fName=setUnits>`

    :api:`Component class <Component>`

    - :api:`addVariable <Component?fName=addVariable>`

    :api:`Model class <Model>`

    - :api:`addUnits <Model?fName=addUnits>`

    :api:`Units class <Units>`

    - :api:`create <Units?fName=create>`
    - :api:`setName <Units?fName=setName>`
    - :api:`addUnit <Units?fName=addUnit>`

.. container:: dothis

    **2.e** Create the variables needed and add them to the potassium channel component.
    Revalidate and expect errors related to variables without units.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.e
        :end-before: //  2.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  2.e
        :end-before: #  2.f

.. code-block:: text

    Recorded 12 issues:
    Issue [0] is an ERROR:
        description: CellML identifiers must contain one or more basic Latin alphabetic characters.
        see section 1.3.1.1 in the CellML specification.
        stored item type: UNDEFINED
    Issue [1] is an ERROR:
        description: Variable 'E_K' in component 'potassiumChannelEquations' does not have a valid units attribute. The attribute given is ''.
        see section 2.8.1.2 in the CellML specification.
        stored item type: VARIABLE
    
    ... etc ...

.. container:: dothis

    **2.f** Create the missing :code:`Units` items and add them to the model. These are:

    - milli-volts;
    - milli-seconds;
    - milli-moles;
    - micro-Amperes per square centimetre; and
    - milli-Siemens per square centimetre.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.f
        :end-before: //  2.g

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  2.f
        :end-before: #  2.g

.. container:: dothis

    **2.g** Set the units on each of the variables.  
    Call the validator again, and expect there to be no errors.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.g
        :end-before: //  end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  2.g
        :end-before: #  end 2

Step 3: Create the nGate and its child components
-------------------------------------------------
The nGateEquations has some of the working of a generic gate (which we'll import from GateModel.cellml), but instead of constant values for alpha and beta, we'll introduce a voltage dependence.
The nGateParameters component allows us to specify those parameters specific to the movement of potassium.

.. container:: dothis

    **3.a** Create a component, name it "nGate", and add it to the equations component.

.. container:: dothis

    **3.b** Create a component, name it "nGateEquations" and add it to the nGate component.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: //  3.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  3.a
        :end-before: #  3.c

.. container:: dothis

    **3.c** Add the mathematics to the nGateEquations component and validate the model.
    Expect errors relating to missing variables.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.c
        :end-before: //  3.d

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  3.c
        :end-before: #  3.d

.. code-block:: text

    Recorded 7 issues:
    Issue [0] is an ERROR:
        description: MathML ci element has the child text 'alpha_n' which does not correspond with any variable names present in component 'nGateEquations'.
        see section 2.12.3 in the CellML specification.
        stored item type: MATH
    Issue [1] is an ERROR:
        description: Math has a cn element with a cellml:units attribute 'per_mV_ms' that is not a valid reference to units in the model 'PotassiumChannelModel' or a standard unit.
        see section 2.13.4 in the CellML specification.
        stored item type: MATH
    Issue [2] is an ERROR:
        description: MathML ci element has the child text 'V' which does not correspond with any variable names present in component 'nGateEquations'.
        see section 2.12.3 in the CellML specification.
        stored item type: MATH

    ... etc ... 

.. container:: dothis

    **3.d** Add the missing variables to the nGateEquations component, and validate again.
    Expect errors relating to units missing from the variables.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.d
        :end-before: //  end 3.d

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  3.d
        :end-before: #  end 3.d

.. code-block:: text

    Recorded 12 issues:
    Issue [0] is an ERROR:
        description: CellML identifiers must contain one or more basic Latin alphabetic characters.
        see section 1.3.1.1 in the CellML specification.
        stored item type: UNDEFINED
    Issue [1] is an ERROR:
        description: Variable 't' in component 'nGateEquations' does not have a valid units attribute. The attribute given is ''.
        see section 2.8.1.2 in the CellML specification.
        stored item type: VARIABLE
    Issue [2] is an ERROR:
        description: CellML identifiers must contain one or more basic Latin alphabetic characters.
        see section 1.3.1.1 in the CellML specification.
        stored item type: UNDEFINED
    Issue [3] is an ERROR:
        description: Variable 'V' in component 'nGateEquations' does not have a valid units attribute. The attribute given is ''.
        see section 2.8.1.2 in the CellML specification.
        stored item type: VARIABLE
    
    ... etc ...

.. container:: dothis

    **3.e** Create the missing units and add them to the model.
    The only two which aren't available are:

    - per millisecond; and 
    - per millivolt millisecond.

    Remember that you'll need to give these names that are the same as those needed by the  variables.
    In this case they are "per_ms" and "per_mV_ms".

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.e
        :end-before: //  3.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  3.e
        :end-before: #  3.f

.. container:: dothis

    **3.f** Associate the correct units items with the variables which need them.
    Revalidate the model, expecting there to be no errors reported.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.f
        :end-before: //  end 3

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  3.f
        :end-before: #  end 3

Step 4: Specify imports for the generic gate components
-------------------------------------------------------

The generic gate model (in GateModel.cellml) has two components: 

- "gateEquations" which solves an ODE for the gate status parameter, X; and
- "gateParameters" which sets the values of alpha, beta, and initialises X.

We will import only the "gateEquations" component and set it to be a child of the nGateEquations component.
This means we can introduce the voltage dependence for the alpha and beta, and using a specified initial value for the gate's status.
Note that the variable "n" in the nGateEquations is equivalent to the generic gate's variable "X".

.. container::shortlist

Imports require three things:

    - A destination for the imported item. 
      This could be a :code:`Component` or :code:`Units` item.
    - A model to import for the imported item from.
      This is stored in an :code:`ImportSource` item containing the URL of the model to read.
    - The name of the item to import.
      This is called the "import reference" and is stored by the destination :code:`Component` or :code:`Units` item.

.. container:: useful

    :api:`ImportSource class <ImportSource>`

    - :api:`create <ImportSource?fName=create>`
    - :api:`setUrl <ImportSource?fName=setUrl>`

    :api:`Model class <Model>`

    - :api:`addImportSource <Model?fName=addImportSource>`

    :api:`ImportedEntity class <ImportedEntity>` (applies to components and units)

    - :api:`setImportSource <ImportedEntity?fName=setImportSource>`
    - :api:`setImportReference <ImportedEntity?fName=setImportReference>`

.. container:: dothis

    **4.a** Create an :code:`ImportSource` item and set its URL to be "GateModel.cellml".

.. container:: dothis

    **4.b** Create a destination component for the imported gate component, and add this to the nGateEquations component. 

.. container:: dothis

    **4.c** Set the import reference on the component you just created to be the name of the component in the GateModel.cellml file that you want to use.
    In this example, it is "gateEquations".

.. container:: dothis

    **4.d** Associate the import source with the component using the setImportSource function.
    Note that this step also makes the import source available to other items through the :code:`importSource(index)` function on the model.
    This way the same imported model file can be used as a source for more than one item.

Note that we are deliberately not importing the parameters component in the GateModel.cellml file, since we will be setting our own values of its variables.

.. container:: dothis

    **4.e** Validate the model and confirm that there are no issues.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  4.a
        :end-before: //  end 4

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  4.a
        :end-before: #  end 4

Step 5: Specify imports for the controller component
----------------------------------------------------
Repeat Step 4 to import a controller component.
This should be at the top of the encapsulation hierarchy, and should import the component named "controller" from the file "PotassiumChannelController.cellml".

.. container:: dothis

    **5.a** Repeat steps 4.a-d for the controller component.
    Put it at the top level of the encapsulation hierarchy.

.. container:: dothis

    **5.b** Validate the model and confirm that there are no issues.

At this point we've defined the equations that govern the potassium channel's operation.
We've also confirmed that the CellML representation of these equations is valid (using the :code:`Validator`); now we need to check that it's also solvable (using the :code:`Analyser`).

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  5.a
        :end-before: //  end 5

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  5.a
        :end-before: #  end 5
        
Step 6: Analyse the model
-------------------------
We will introduce the :code:`Analyser` class here so that its use as a debugging  tool can be demonstrated.
Of course, we know ahead of time that there is still a lot of connections to be created between the components, but the analyser can help us to find them.
The analyser is similar to the :code:`Validator` class and keeps a record of issues it encounters.

.. container:: nb 

    **A reminder:** We're aiming for a potassium channel component which can accept two external parameters - time, t (ms) and voltage, V (mV) - and use them to calculate a potassium current, i_K (microA_per_cm2). 
    A utility function :code:`printModel(Model, bool)` has been provided to help you to see what's going  on inside your model.
    Setting the second optional parameter to :code:`true` will also print the MathML content.

.. container:: dothis

    **6.a** Create an :code:`Analyser` item and pass it the model for checking using its :code:`analyseModel` function.

.. container:: dothis

    **6.b** Retrieve the analyser's issues and print them to the terminal, just as you've done for the validator.
    Expect messages related to un-computed variables.

.. code-block:: text

    Recorded 11 issues:
    Issue [0] is an ERROR:
        description: Variable 'V' in component 'nGateEquations' is not computed.
        stored item type: VARIABLE
    Issue [1] is an ERROR:
        description: Variable 'alpha_n' in component 'nGateEquations' is not computed.
        stored item type: VARIABLE
    Issue [2] is an ERROR:
        description: Variable 'beta_n' in component 'nGateEquations' is not computed.
        stored item type: VARIABLE
    
    ... etc ...

Even though all of the messages we see are "variable not calculated" errors, we can divide them into different categories:

- those variables which are constants whose value has not been set yet;
- those variables whose calculation depends on as-yet un-calculated variables;
- those variables which need to be connected to where their calculation happens; and
- those variables which aren't present in any equation.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  6.a
        :end-before: //  end 6

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  6.a
        :end-before: #  end 6

Step 7: Define the constants
----------------------------
As we work through the next few steps we'll be defining and connecting all of the components and variables together.
First we'll define the variables which will have a constant value in the simulation.

.. container:: dothis

    **7.a** Use the print model helper function to show your current model contents.
    This should show that we have currently got variables only in the nGateEquations and potassiumChannelEquations components.
    These need to have sibling parameters components created to hold any hard-coded values or initial conditions that are required.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  7.a
        :end-before: //  end 7.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  7.a
        :end-before: #  end 7.a

Create parameters siblings components for the equations components, and add the variables that they will require.
These are:

- potassium channel parameters

    - E_K (-87) 
    - g_K (36) 

- nGate parameters

    - initial value for n (dimensionless)

You can either do this by creating the variables from scratch (as in Step 3.d) but because these are intended to be duplicates of existing variables, but in another component, we can simply add a cloned variable to the parameters component.

.. container:: dothis

    **7.b** Create parameters components for the equations components, and add cloned versions of any variables which need to be given a value into the new parameters components.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  7.b
        :end-before: //  7.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  7.b
        :end-before: #  7.c

.. container:: dothis

    **7.c** In order for other encapsulating components to access these variables, they also need to have intermediate variables in the nGate or potassium channel components too.
    This is only true of variables that you want to be available to the outside.
    In this example, we need to add the variable "n" to the nGate in order that its parent (the potassium channel equations) can access it.

.. container:: dothis

    **7.d** Create variable connections between these variables and their counterparts in the equations components.
    Validate, expecting errors related to missing or incorrect interface types.

.. code-block:: text

    Recorded 6 issues:
    Issue [0] is an ERROR:
        description: Variable 'E_K' in component 'potassiumChannelEquations' has no interface type set. The interface type required is 'public'.
        see section 3.10.8 in the CellML specification.
        stored item type: VARIABLE

    ... etc ... 

.. container:: dothis

    **7.e** Set the required interface types as listed by the validator.
    This can be done individually using the :code:`setInterfaceType` function on each variable, or automatically using the :code:`fixVariableInterfaces` function on the entire model.
    Validate again, expecting no validation errors.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  7.c
        :end-before: //  end 7.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  7.c
        :end-before: #  end 7.e

If we were to analyse the model again now we would we still have the same set of errors as earlier as we haven't given a value to any of our parameters.

.. container:: shortlist

    These values should be:

    - Potassium channel parameters:

        - E_K = -85 [mV]
        - g_K = 36 [milliS_per_cm2]

    - nGate parameters:

        - n = 0.325 [dimensionless]

.. container:: dothis

    **7.f** Use the :code:`setInitialValue` function to set these parameter values.
    Analyse the model again, expecting that the calculation errors related to these constants have been solved.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  7.f
        :end-before: //  end 7

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  7.f
        :end-before: #  end 7

Step 8: Connect the input variables
-----------------------------------
Looking at the variables listed in the issues above we can see that some of our "external" or "input" variables are listed more than once.
These are the voltage, V, and time, t.
Time is needed in every equations component, including the imported gate component.
Voltage is needed by the potassium channel and nGate equations components.

.. container:: dothis

    **8.a** Print the model to the terminal and notice the components which contain V and t variables.  

.. code-block:: text

    MODEL: 'PotassiumChannelModel'
        UNITS: 7 custom units
            [0]: ms
            [1]: mV
            [2]: mM
            [3]: microA_per_cm2
            [4]: milliS_per_cm2
            [5]: per_ms
            [6]: per_mV_ms
        COMPONENTS: 2 components
            [0]: potassiumChannel
                VARIABLES: 0 variables
                COMPONENT potassiumChannel has 2 child components:
                    [0]: potassiumChannelEquations
                        VARIABLES: 6 variables
                            [0]: E_K [mV]
                                └──> potassiumChannelParameters:E_K [mV]
                            [1]: i_K [microA_per_cm2]
                            [2]: g_K [milliS_per_cm2]
                                └──> potassiumChannelParameters:g_K [milliS_per_cm2]
                            [3]: V [mV]
                            [4]: t [ms]
                            [5]: n [dimensionless]
                        COMPONENT potassiumChannelEquations has 1 child components:
                            [0]: nGate
                                VARIABLES: 1 variables
                                    [0]: n [dimensionless]
                                        └──> nGateEquations:n [dimensionless]
                                COMPONENT nGate has 2 child components:
                                    [0]: nGateEquations
                                        VARIABLES: 5 variables
                                            [0]: t [ms]
                                            [1]: V [mV]
                                            [2]: alpha_n [per_ms]
                                            [3]: beta_n [per_ms]
                                            [4]: n [dimensionless]
                                                └──> nGate:n [dimensionless]
                                        COMPONENT nGateEquations has 1 child components:
                                            [0]: importedGate <--- imported from: 'gateEquations' in 'GateModel.cellml'
                                                VARIABLES: 0 variables
                                    [1]: nGateParameters
                                        VARIABLES: 1 variables
                                            [0]: n [dimensionless], initial = 0.325
                    [1]: potassiumChannelParameters
                        VARIABLES: 2 variables
                            [0]: E_K [mV], initial = -85
                                └──> potassiumChannelEquations:E_K [mV]
                            [1]: g_K [milliS_per_cm2], initial = 36
                                └──> potassiumChannelEquations:g_K [milliS_per_cm2]
            [1]: controller <--- imported from: 'controller' in 'PotassiumChannelController.cellml'
                VARIABLES: 0 variables

Connections between the variables in any two components are only possible when those components are in a sibling-sibling or parent-child relationship.
We can see from the printed structure that the top-level potassium channel component doesn't have any variables, and neither does the nGate component.
We'll need to create intermediate variables in those components to allow connections to be made through them.  

.. container:: dothis

    **8.b** Create dummy variables for time and voltage using the cloning technique described in Step 7.b, and add a clone to each appropriate component.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  8.a
        :end-before: //  8.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  8.a
        :end-before: #  8.c

.. container:: dothis

    **8.c** Connect these variables to their counterparts as needed.

.. container:: dothis

    **8.d** Fix the variable interfaces and validate the model, expecting no errors.

.. container:: dothis

    **8.e** Analyse the model and expect that errors related to voltage and time now occur only in the top-level potassium channel component.
    Because this needs to be connected to the imported controller component, they'll be addressed later in Step 10.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  8.c
        :end-before: //  end 8

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  8.c
        :end-before: #  end 8

Step 9: Connect the calculated variables
----------------------------------------
Now we need to make sure that all of the calculated variables can move through the model properly.
In this example, the only calculated variable is n, the gate status.
This is calculated by solving the ODE in the nGate equations component, but needs to be initialised by the nGate parameters component, and its value passed back to the potassium channel equations component. 

.. container:: dothis

    **9.a** Make the required variable connections as described above.

.. container:: dothis

    **9.b** Fix the variable interfaces for the model, and validate, expecting no errors.

.. container:: dothis

    **9.c** Analyse the model, expecting that the errors related to the n variable have been resolved.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  9.a
        :end-before: //  end 9

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  9.a
        :end-before: #  end 9

Step 10: Connect to imported components
---------------------------------------
At this point, we have made all the connections we can between existing variables and components.
(You can verify this for yourself by printing your model to the terminal again if you like.) 
Now the problem we have is that we need to connect to variables inside imported components, but these don't exist in our model yet: the import sources that we created in Steps 4 and 5 are simply a recipe; they don't actually create anything here.

In order to connect to variables in imported components, we can create dummy variables inside them.
These will be overwritten when the imports are resolved and the model flattened, at which time the imported variables will replace the dummy ones.
As with other steps, we have a choice here.
We can manually create variables or clone existing ones into the destination components we have already created; or we can make use of the :code:`Importer` class to help us manage these.
We're going to do the latter now.

.. container:: dothis

    **10.a** Create an :code:`Importer` item.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  10.a
        :end-before: //  end 10.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  10.a
        :end-before: #  end 10.a

Resolving the imports in a model triggers the importer to go searching for all of the information required by this model's imports, even through multiple generations of import layers.
It also instantiates each of those requirements into its own library.
You could use the model's :code:`hasUnresolvedImports` function to test whether the operation was successful or not; expecting it to be true before resolution, and false afterwards.

.. container:: dothis

    **10.b** Pass the model and the path to the GateModel.cellml file into the importer's :code:`resolveImports` function. 

The :code:`Importer` class has a logger (just like the :code:`Validator`), so needs to be checked for issues.

.. container:: dothis

    **10.c** Check for issues and print any found to the terminal - we do not expect any at this stage.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  10.b
        :end-before: //  end 10.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  10.b
        :end-before: #  end 10.c

The models which have the source components that we wanted to reuse from the GateModel.cellml and PotassiumChannelController.cellml files are now available to us in two ways:

- through the :code:`model` function of the destination component's :code:`ImportSource` item; or
- as an item in the importer's library.  
  The library items can be retrieved either by index or by key, where the key is the name of the file that was resolved.

.. container:: dothis

    **10.d** Iterate through the items in the library (:code:`libraryCount` will give you the total), and print its keys to the terminal.
    The keys can be retrieved as a string from the :code:`key(index)` function.  
    This should contain two models.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  10.d
        :end-before: //  10.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  10.d
        :end-before: #  10.e

.. code-block:: text

    The importer has 2 models in the library.
        library(0) = GateModel.cellml
        library(1) = PotassiumChannelController.cellml

.. container:: dothis

    **10.e** We can simply use a clone of the imported components to define dummy variables in the destination component.
    Create dummy components from the resolved imported components.
    You can get these from the library or from the import source's model (or one of each, to prove to yourself that it works either way!).

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  10.e
        :end-before: //  10.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  10.e
        :end-before: #  10.f 

.. container:: nb

    **GOTCHA:** Note that when an item is added to a new parent, it is automatically removed from its original parent.  
    Iterating through a set of children is best done in descending index order or using a while loop so that child items are not skipped as the indices change.

.. container:: dothis

    **10.f** Iterate through the variables in each dummy component, and add a clone of each variable to the destination component.     
               
.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  10.f
        :end-before: //  10.g

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  10.f
        :end-before: #  10.g

.. container:: dothis

    **10.g** Connect all the variables in the nGate equations component to the dummy variables in the imported gate component.
    These connections should be:

    - *nGate equations component : imported gate component*
    - n : X
    - alpha_n : alpha_X
    - beta_n : beta_X
    - t : t
    
    Repeat for the controller component and the potassium channel component.
    Fix the variable interfaces and validate the model, expecting there to be no errors.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  10.g
        :end-before: //  10.h

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  10.g
        :end-before: #  10.h

.. container:: dothis

    **10.h** Make sure that the output variable from this component - the potassium current - is available at the top level, and with a public and private interface.
    You'll need to create a dummy variable in the potassium channel component and link it appropriately.
    Validate your model and expect no errors.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  10.h
        :end-before: //  end 10.h

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  10.h
        :end-before: #  end 10.h

The :code:`Analyser` class can only operate on a flat (ie: import-free) model.
In order to do the final check before serialising our model for output, we will use the importer to create a flattened version of the model to submit for analysis.

.. container:: dothis

    **10.i** Create a flat version of the model and submit it for analysis.  Print the errors

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  10.i
        :end-before: //  end 10.i

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  10.i
        :end-before: #  end 10.i

.. container:: nb

    Note that at this point an analysis of the unflattened model will still show errors, but that's totally fine.

Step 11: Output the model
-------------------------

.. container:: dothis

    **11.a** Create a :code:`Printer` instance and use it to serialise the model.
    This creates a string containing the CellML-formatted version of the model.
    Write this to a file called "PotassiumChannelModel.cellml"; you will need this in :ref:`Tutorial 4<generateMembraneModel>`.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: createPotassiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  11.a
        :end-before: //  end

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: createPotassiumChannelModel_completed.py
        :language: python
        :start-at: #  11.a
        :end-before: #  end
