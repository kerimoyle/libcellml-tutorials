..  _debugSodiumChannelModel:

Hodgkin-Huxley 3: Debugging a model
====================================

.. container:: shortlist

    By the time you have worked through this part of the tutorial you will be able to:

    - Parse an existing CellML file and deserialise it into a model instance;
    - Use the diagnostic :code:`Validator` class to identify issues in the model's definition; 
    - Use the :code:`std::any_cast` to retrieve items which need repair from :code:`Issue` items;
    - Use the :code:`Importer` class to resolve imports and identify issues; and
    - Use the diagnostic :code:`Analyser` class to identify issues in the model's mathematical formulation.


.. container:: shortlist

    **Code (C++)**

    - :download:`CMakeLists.txt<CMakeLists.txt>` The CMake file for building this tutorial;
    - :download:`debugSodiumChannelModel.cpp<debugSodiumChannelModel.cpp>` Either the skeleton code, or ..
    - :download:`debugSodiumChannelModel_completed.cpp<debugSodiumChannelModel_completed.cpp>` the completed tutorial code; and
    - :download:`utilities.cpp<../code/utilities.cpp>` and :download:`utilities.h<../code/utilities.h>` Helper functions.

.. container:: shortlist

    **Code (Python)**

    - :download:`debugSodiumChannelModel.py<debugSodiumChannelModel.py>` Either the skeleton code, or ..
    - :download:`debugSodiumChannelModel_completed.py<debugSodiumChannelModel_completed.py>` the completed tutorial code; and

.. container:: shortlist

    **Resources**

    - :download:`GateModel.cellml<GateModel.cellml>` the generic gate model created in Tutorial 1;
    - :download:`SodiumChannelModel_broken.cellml<SodiumChannelModel_broken.cellml>` a sodium channel model that needs debugging;
    - :download:`CircularControllerReference.cellml<CircularControllerReference.cellml>` import file;
    - :download:`CircularControllerReference2.cellml<CircularControllerReference2.cellml>` import file; and
    - :download:`SodiumChannelController.cellml<SodiumChannelController.cellml>` import file.

.. contents:: Contents
    :local:

Overview
--------
This tutorial looks at how a "broken" model can be investigated and debugged using the diagnostic tools in the three special classes: the :code:`Importer`, the :code:`Analyser`, and the :code:`Validator`.

Step 1: Parse the existing sodium channel model
-----------------------------------------------
The :code:`Parser` class is used to deserialise a CellML string into a :code:`Model` instance.
This means that you're responsible for finding, opening and reading the :code:`*.cellml` file into a single string.
The parser will then read that string and return a model.

.. container:: dothis

    **1.a** Read a CellML file into a string.

.. container:: dothis

    **1.b** Create a :code:`Parser` item. 

.. container:: dothis

    **1.c** Use the parser to deserialise the contents of the string you've read into a model.

.. container:: dothis

    **1.d** Print the parsed model to the terminal for viewing.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  1.a
        :end-before: //  end 1

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  1.a
        :end-before: #  end 1

.. code-block:: text

    MODEL: 'SodiumChannelModel'
        UNITS: 5 custom units
            [0]: mV
            [1]: ms
            [2]: per_ms
            [3]: per_mV_ms
            [4]: microA_per_cm2
        COMPONENTS: 4 components
            [0]: controller <--- imported from: 'controller' in 'CircularControllerReference.cellml'
                VARIABLES: 2 variables
                    [0]: t
                        └──> sodiumChannel:t [ms]
                    [1]: V
                        └──> sodiumChannel:V
            [1]: mGateEquations!
                VARIABLES: 5 variables
                    [0]: alpha_m [per_ms]
                    [1]: V [mV]
                    [2]: beta_m [per_ms]
                    [3]: m [dimensionless]
                    [4]: t [ms]
            [2]: importedGateM <--- imported from: 'gateEquations' in 'GateModel.cellml'
                VARIABLES: 4 variables
                    [0]: alpha_X
                    [1]: beta_X
                    [2]: X
                    [3]: t
            [3]: sodiumChannel
                VARIABLES: 3 variables
                    [0]: t [ms]
                        └──> controller:t, sodiumChannelEquations:t [ms]
                    [1]: V
                        └──> controller:V, sodiumChannelEquations:V [mV]
                    [2]: i_Na [microA_per_cm2]
                        └──> sodiumChannelEquations:i_Na [microA_per_cm2]
                COMPONENT sodiumChannel has 2 child components:
                    [0]: sodiumChannelEquations
                        VARIABLES: 8 variables
                            [0]: Na_conductance [mS_per_cm2]
                            [1]: g_Na [mS_per_cm2]
                                └──> sodiumChannelParameters:g_Na [mS_per_cm2]
                            [2]: h [dimensionless]
                                └──> hGate:h [dimensionless]
                            [3]: m [dimensionless]
                                └──> mGate:m [dimensionless]
                            [4]: i_Na [microA_per_cm2]
                                └──> sodiumChannel:i_Na [microA_per_cm2]
                            [5]: V [mV]
                                └──> sodiumChannel:V, hGate:V [mV], mGate:V [mV]
                            [6]: E_Na [mV]
                                └──> sodiumChannelParameters:E_Na [mV]
                            [7]: t [ms]
                                └──> sodiumChannel:t [ms], hGate:t [ms], mGate:t [ms]
                        COMPONENT sodiumChannelEquations has 2 child components:
                            [0]: mGate
                                VARIABLES: 3 variables
                                    [0]: t [ms]
                                        └──> sodiumChannelEquations:t [ms]
                                    [1]: m [dimensionless]
                                        └──> sodiumChannelEquations:m [dimensionless]
                                    [2]: V [mV]
                                        └──> sodiumChannelEquations:V [mV]
                                COMPONENT mGate has 1 child components:
                                    [0]: mGateParameters
                                        VARIABLES: 2 variables
                                            [0]: m [dimensionless], initial = 0.05
                                            [1]: i_am_redundant [steradian]
                            [1]: hGate
                                VARIABLES: 3 variables
                                    [0]: t [ms]
                                        └──> sodiumChannelEquations:t [ms], hGateEquations:t [ms]
                                    [1]: h [dimensionless]
                                        └──> sodiumChannelEquations:h [dimensionless], hGateEquations:h [dimensionless]
                                    [2]: V [mV]
                                        └──> sodiumChannelEquations:V [mV], hGateEquations:V [mV]
                                COMPONENT hGate has 2 child components:
                                    [0]: hGateParameters
                                        VARIABLES: 1 variables
                                            [0]: h [dimensionless], initial = 0.6
                                    [1]: hGateEquations
                                        VARIABLES: 5 variables
                                            [0]: alpha_h [per_ms]
                                                └──> importedGateH:alpha_X
                                            [1]: beta_h [per_ms]
                                                └──> importedGateH:beta_X
                                            [2]: V [mV]
                                                └──> hGate:V [mV]
                                            [3]: h [dimensionless]
                                                └──> hGate:h [dimensionless], importedGateH:X
                                            [4]: t [ms]
                                                └──> hGate:t [ms], importedGateH:t
                                        COMPONENT hGateEquations has 1 child components:
                                            [0]: importedGateH <--- imported from: 'i_dont_exist' in 'GateModel.cellml'
                                                VARIABLES: 4 variables
                                                    [0]: alpha_X
                                                        └──> hGateEquations:alpha_h [per_ms]
                                                    [1]: beta_X
                                                        └──> hGateEquations:beta_h [per_ms]
                                                    [2]: X
                                                        └──> hGateEquations:h [dimensionless]
                                                    [3]: t
                                                        └──> hGateEquations:t [ms]
                    [1]: sodiumChannelParameters
                        VARIABLES: 2 variables
                            [0]: g_Na [mS_per_cm2], initial = 120
                                └──> sodiumChannelEquations:g_Na [mS_per_cm2]
                            [1]: E_Na [mV]
                                └──> sodiumChannelEquations:E_Na [mV]

Step 2: Validate the parsed model
---------------------------------
Create a :code:`Validator` item and use it to validate the model you've just read.

.. container:: dothis

    **2.a** Create a :code:`Validator` item and validate the model.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: //  end 2.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  2.a
        :end-before: #  end 2.a

.. container:: dothis

    **2.b** Retrieve any issues from the validator and print them to the terminal.

.. code-block:: container

    The validator found 10 issues.
    Issue 0: CellML identifiers must not contain any characters other than [a-zA-Z0-9_].
    reference: 1.3.1.1
    see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specA03.html?issue=1.3.1.1
    stored item type: UNDEFINED

    Issue 1: Component 'mGateEquations!' does not have a valid name attribute.
    reference: 2.7.1
    see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB07.html?issue=2.7.1
    stored item type: COMPONENT

    Issue 2: Variable 'Na_conductance' in component 'sodiumChannelEquations' has a units reference 'mS_per_cm2' which is neither standard nor defined in the parent model.
    reference: 2.8.1.2
    see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    stored item type: VARIABLE

    ... etc ...

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.b
        :end-before: //  end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  2.b
        :end-before: #  end 2

Step 3: Repair the parsed model
-------------------------------
The messages returned from the validator (and other classes) should (!) have enough information to enable you to know what the problem is.
In the case of the validator class, the URL listed contains additional resources and examples related to the issue, as well as its section in the CellML normative specification.

In some situations more than one :code:`Issue` will be generated from a single cause: this is the case with issues 0 and 1 here:

.. code-block:: text

    Issue 0: CellML identifiers must not contain any characters other than [a-zA-Z0-9_].
        reference: 1.3.1.1
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specA03.html?issue=1.3.1.1
    Issue 1: Component 'mGateEquations!' does not have a valid name attribute.
        reference: 2.7.1
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB07.html?issue=2.7.1

.. container:: dothis

    **3.a** Fetch the component using its name, and set the name to something valid.
    Note that when finding a :code:`Component` item, setting an optional second argument to :code:`true` will search the entire encapsulation hierarchy for a component with that name, and not only the direct children of the model.
    You can follow the URL for information about what makes a valid name, and use the :code:`setName` function to fix it.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: //  end 3.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.a
        :end-before: #  end 3.a

.. code-block:: text

    Issue 2: Variable 'Na_conductance' in component 'sodiumChannelEquations' has a units reference 'mS_per_cm2' which is neither standard nor defined in the parent model.
        reference: 2.8.1.2
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    Issue 3: Variable 'g_Na' in component 'sodiumChannelEquations' has a units reference 'mS_per_cm2' which is neither standard nor defined in the parent model.
        reference: 2.8.1.2
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    Issue 4: Variable 'g_Na' in component 'sodiumChannelParameters' has a units reference 'mS_per_cm2' which is neither standard nor defined in the parent model.
        reference: 2.8.1.2
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2

.. container:: dothis

    **3.b** The messages above indicate that we're missing a Units item named "mS_per_cm2". 
    Create an appropriate Units item (note that S stands for "siemens"), and add it to your model.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.b
        :end-before: //  end 3.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.b
        :end-before: #  end 3.b

.. code-block:: text

    Issue 5: CellML identifiers must contain one or more basic Latin alphabetic characters.
        reference: 1.3.1.1
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specA03.html?issue=1.3.1.1
    Issue 6: Variable 'V' in component 'sodiumChannel' does not have a valid units attribute. The attribute given is ''.
        reference: 2.8.1.2
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    Issue 8: Variable 'V' in component 'sodiumChannel' has units of '' and an equivalent variable 'V' in component 'sodiumChannelEquations' with non-matching units of 'mV'. The mismatch is:
        reference: 
        see: 

As with 3.a, here we have more than one issue generated from the same cause: in this case, we haven't specified units for a variable.
    
Each issue generated contains a pointer to the item to which it refers. 
We can retrieve the affected item directly from the issue in one of two ways:

- retrieving an :code:`AnyItem` structure (whose "first" attribute is an enum of the :code:`CellmlElementType`; 
  and "second" attribute is an std::any cast of the item itself); and casting it appropriately, or
- since we know that the type of item in this error is a :code:`VARIABLE`, we can call the convenience method :code:`Issue::variable()` to return the variable which needs attention.
  (Of course you could retrieve it using the name of its parent component and its name too - this is just another way!)

.. container:: dothis

    **3.c** Check that the item to be returned from the issue is in fact an :code:`CellmlElementType::VARIABLE` by calling its :code:`cellmlElementType()` function.
    Retrieve the variable missing units from the issue.
    Set its units to be millivolts.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.c
        :end-before: //  end 3.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.c
        :end-before: #  end 3.c

The error below indicates that a child :code:`Unit` references something which can't be found.

.. code-block:: text

    Issue 7: Units reference 'i_dont_exist' in units 'mV' is not a valid reference to a local units or a standard unit type.
        reference: 2.6.1
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB06.html?issue=2.6.1

You have a few different options for how to fix this one. 

1) The manual way.  
   The issue description tells us the name of the :code:`Units` where the issue is, and the name of the units required by one of its children.
   We can locate both items using these names, remove the problem child, and replace it with a correct one.  
2) The pointer way. 
   As with the example in 3.c, we can retrieve a structure representing the :code:`Unit` child directly from the issue.
   Since (as above) we know it will be a :code:`Unit` item, we can call the issue->unit() function to retrieve it.
   The :code:`Unit` item consists of a pointer to its parent :code:`Units` item, and the index of the relevant child.  
3) The roundabout option.
   Since the error is saying that units named "i_dont_exist" are missing, we could simply provide them by creating a :code:`Units` item and adding it to the model.
 
.. container:: useful

    :api:`Units class <Units>`

    - unitAttributes
    - removeUnit
    - addUnit

.. container:: dothis

    **3.d** Choose your preferred method and use it to retrieve the problem unit attributes and print them all to the terminal.
    Then fix the issue.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.d
        :end-before: //  end 3.d

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.d
        :end-before: #  end 3.d

The final validator issue refers to the fact that we need to explicitly specify how other components can access each of the variables in this component.

.. code-block:: text

    Issue 9: Variable 't' in component 'sodiumChannelEquations' has no interface type set. The interface type required is 'public_and_private'.
        reference: 3.10.8
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specC10.html?issue=3.10.8

.. container:: dothis

    **3.e** Retrieve the variable either using the issue pointer method, or using the name method, and set its interface to be the required type.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.e
        :end-before: //  3.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.e
        :end-before: #  3.f

.. container:: dothis

    **3.f** Revalidate the model and confirm that the errors have gone.

.. container:: useful

    :api:`Model class <Model>`

    - component("componentName", true) will search for the component's name in the whole of the encapsulation hierarchy.

    :api:`Component class <Component>`

    - addComponent

    Tutorial functions

    - printEncapsulation will output just the names of the components, nested in their encapsulation hierarchy.

.. container:: dothis

    **3.g** Even though the model is free from validation errors, we still need to make sure it represents what we want it to.
    Print the model to the terminal and check its structure.

.. container:: dothis

    **3.h** Use the addComponent functions to rearrange the components as needed until you have the required structure.
    Validate the model again.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.g
        :end-before: //  end 3

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.g
        :end-before: #  end 3

.. code-block:: text

    Model 'SodiumChannelModel' has 2 components
    - Component 'controller' has 0 child components
    - Component 'sodiumChannel' has 2 child components
        - Component 'sodiumChannelEquations' has 2 child components
            - Component 'mGate' has 2 child components
                - Component 'mGateParameters' has 0 child components
                - Component 'mGateEquations' has 1 child components
                    - Component 'importedGateM' has 0 child components
            - Component 'hGate' has 2 child components
                - Component 'hGateParameters' has 0 child components
                - Component 'hGateEquations' has 1 child components
                    - Component 'importedGateH' has 0 child components
        - Component 'sodiumChannelParameters' has 0 child components


Step 4: Resolve the model's imports
-----------------------------------
It's important to remember that the imports are merely instructions for how components or units items should be located: only their syntax is checked by the validator, not that the files exist or contain the required information.  To debug the imported aspects of the model, we need to use an :code:`Importer` class.

To resolve the imports, we need a path to a base location against which any relative file addresses can be resolved.  
For this tutorial, the files are in the same directory as the code, so simply using an empty string is sufficient.

.. container:: nb 

    If they're another directory, make sure to end your path with a slash, "/".
    If they're in your working directory, enter an empty string.

.. container:: useful
    
    :api:`Importer class <Importer>`
    
    - create
    - resolveImports

.. container:: dothis

    **4.a** Create an :code:`Importer` instance and use it to resolve the model.

.. container:: dothis

    **4.b** Similarly to the validator, the importer will log any issues it encounters.
    Retrieve these and print to the terminal (you can do this manually or using the convenience function as before).

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  4.a
        :end-before: //  end 4.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  4.a
        :end-before: #  end 4.b

.. code-block:: text

    Recorded 2 issues:

    Issue [0] is an ERROR:
        description: Import of component 'importedGateH' from 'GateModel.cellml' requires component named 'i_dont_exist' which cannot be found.
        stored item type: COMPONENT

Fix the issues reported by the importer.
This needs to be an iterative process because as more files become available to the importer, the content of those files needs to be checked too.

.. container:: useful

    :api:`Component class <Component>`

    - setImportReference

.. container:: dothis

    **4.c** 
    We need to change the import reference for the component to be "gateEquations" instead of "i_dont_exist".
    You can either retrieve the component using its name or directly from the item stored with the issue.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  4.c
        :end-before: //  end 4.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  4.c
        :end-before: #  end 4.c

.. code-block:: text

    Issue [1] is a WARNING:
        description: Cyclic dependencies were found when attempting to resolve components in model 'CircularReferences'. The dependency loop is:
            - component 'importedGateH' is imported from 'i_dont_exist' in 'GateModel.cellml';
            - component 'importedGateM' is imported from 'gateEquations' in 'GateModel.cellml';
            - component 'controller' is imported from 'controller' in 'CircularControllerReference.cellml';
            - component 'controller' is imported from 'controller2' in 'CircularControllerReference2.cellml';
            - component 'controller2' is imported from 'controller' in 'CircularControllerReference.cellml'; and
            - component 'controller' is imported from 'controller2' in 'CircularControllerReference2.cellml'.
        stored item type: UNDEFINED
  
To fix this, we have two options: 

- to open and repair the file which is actually broken, or
- to switch the import source in this current model to one which doesn't have circular imports.

It's included here to highlight the fact that the :code:`Importer` class opens and instantiates all required dependencies, and that some of those dependencies may have problems of their own ... even issues in files that haven't (yet) been seen at all by you, the user.

.. container:: dothis

    **4.d** In this example we can change the import of the controller component to have url of 'SodiumChannelController.cellml'.

.. container:: dothis

    **4.e** Resolve the imports again and check that there are no further issues.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  4.d
        :end-before: //  end 4

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  4.d
        :end-before: #  end 4

Step 5: Validate the imported dependencies
------------------------------------------
At this stage we've validated the local model, and we've used the :code:`Importer` class to retrieve all of its import dependencies.
These dependencies are stored in the importer's library, and have not yet been validated or analysed.  

.. container:: useful

    :api:`Importer class <Importer>`

    - libraryCount returns the number of stored models;
    - library returns the model at the given index or given key string;
    - key returns a key string at the given index;

.. container:: dothis

    **5.a** Use a simple loop to validate each of the models stored in the importer's library.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  5.a
        :end-before: //  end 5.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  5.a
        :end-before: #  end 5.a

.. code-block:: text

    Imported model at key: CircularControllerReference.cellml
    Recorded 0 issues!

    Imported model at key: CircularControllerReference2.cellml
    Recorded 0 issues!

    Imported model at key: GateModel.cellml
    Recorded 0 issues!

    Imported model at key: SodiumChannelController.cellml
    Recorded 0 issues!

Note that the two files creating the circular import in 4.a are still in the library. 
To limit ourselves to only those models which are still relevant as the import dependencies of our repaired model, we can iterate through our model's :code:`ImportSource` items instead.  
As soon as the model's imports have been resolved, all these will point to instantiated models within the importer.

.. container:: useful 

    :api:`Model class <Model>`

    - importSourceCount
    - importSource

    :api:`ImportSource class <ImportSource>`

    - model
    - url

.. container:: dothis

    **5.b** Loop through the model's import source items and print their URLs to the terminal.
    You'll notice that these have been used as the keys in the importer library.
    Check that the importer library's models are the same as that attached to the import source item.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  5.b
        :end-before: //  end 5

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  5.b
        :end-before: #  end 5


.. code-block:: text

    Import source [0]:
        url = GateModel.cellml
        model = 0x7ff61265b3f0
        library[url] = 0x7ff61265b3f0
    Import source [1]:
        url = SodiumChannelController.cellml
        model = 0x7ff6141003c0
        library[url] = 0x7ff6141003c0


Step 6: Analyse the model(s)
----------------------------
As with the validator, the :code:`Analyser` class is a diagnostic class which will check whether the mathematical representation is ready for simulation.
This involves making sure that variables are contained in equations, that integrated variables have initial conditions, and that there are no over- or under-constrained sets of equations.
Since this model uses imports, the real mathematical model is hidden from the :code:`Analyser` (just as it was from the :code:`Validator`).
The way around this is to use the :code:`Importer` class to create a flat (ie: import-free) version of the same model.
If the flat model meets the analyser's checks, then the importing version will too.

.. container:: useful

    :api:`Analyser class <Analyser>`

    - analyseModel
    - flattenModel

.. container:: dothis

    **6.a** Create an :code:`Analyser` instance and pass in the model for analysis.

.. container:: dothis

    **6.b** Retrieve and print the issues from the analysis to the screen.
    We expect to see messages related to un-computed variables, since anything which is imported is missing from this model.

.. code-block:: text

    Recorded 19 issues:
    Issue [0] is an ERROR:
        description: Variable 'V' in component 'controller' is not computed.
        stored item type: VARIABLE
    Issue [1] is an ERROR:
        description: Variable 't' in component 'controller' is not computed.
        stored item type: VARIABLE
    Issue [2] is an ERROR:
        description: Variable 'alpha_h' in component 'hGateEquations' is not computed.
        stored item type: VARIABLE

    ... etc ...

.. container:: dothis

    **6.c** Create a flattened version of the model print it to the screen.
    Notice that any comments indicating that a component was an import have been removed as these components have been instantiated in the flattened model.

.. container:: dothis

    **6.d** Analyse the flattened model and print the issues to the screen.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  6.a
        :end-before: //  end 6.d

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  6.a
        :end-before: #  end 6.d

.. code-block:: text

    Issue [0] is an ERROR:
        description: Variable 't' in component 'importedGateM' and variable 't' in component
                 'importedGateH' cannot both be the variable of integration.
        stored item type: VARIABLE

The issue returned from the analyser says that we're trying to use two different variables as the base variable of integration, and the CellML code generation facility (which the analyser is tied to) does not support this yet.
It's still valid CellML though!
In this example, the real problem is that these two variables are talking about the same thing, but haven't been connected to one another yet.

.. container:: useful

    :api:`Variable class <Variable>`

    - addEquivalence

.. container:: dothis

    **6.e** Create any necessary variable equivalences so that these two variables are connected.
    You can refer to your printout of the model's structure to help if need be, and remember that only variables in a sibling or parent/child relationship can be connected. 

.. container:: dothis

    **6.f** Re-flatten and re-analyse the model and print the issues to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  6.e
        :end-before: //  end 6.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  6.e
        :end-before: #  end 6.f

.. code-block:: text

    Recorded 13 issues:
    Issue [0] is an ERROR:
        description: Variable 'X' in component 'importedGateM' is used in an ODE, but it is not initialised.
        stored item type: VARIABLE
    Issue [1] is an ERROR:
        description: Variable 'alpha_X' in component 'importedGateM' is not computed.
        stored item type: VARIABLE

Now we see the importance of checking iteratively for issues in the analyser class.  
The nature of this class means that frequently it is unable to continue processing when an issue is encountered.
It's not unusual to fix one issue only to find twenty more!
Two of the errors reported deal with non-initialised variables.
Looking at the model printout we can see that this is because the integrated variable "X" (in both the imported gates) hasn't been connected to its local variable "h" or "m" in the appropriate parameters component. 

.. container:: dothis

    **6.g** Create all required connections needed to connect these variables.
    Re-flatten, re-analyse and print the issues to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  6.g
        :end-before: //  end 6.g

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  6.g
        :end-before: #  end 6.g

The nice thing about issues in this class is that frequently a few issues refer to the same single problem.
The remainder of the issues reported deal with variables that are not computed.
This could mean any one of:

1) the variable is not included in any equations (it's completely unused);
2) the variable is included in an equation, but the equation can't be evaluated (contains some other un-computed variable(s));
3) the variable is a constant that should have a value assigned; or
4) the variable hasn't been connected to the rest of its definition (usually it's this one!).

Because the "is not computed" errors are cascading by nature, frequently fixing just one will resolve many others. 

.. container:: useful

    - C++: printEquivalentVariableSet with the variable argument
    - Python: print_equivalent_variable_set with the variable argument

Hints for this tutorial: 

- There is at least one of each kind of problem;
- There's a convenience function provided (see below) which will print the equivalent variable set for a given variable.
  You can use the item stored by each issue and this function to check for missing connections.
- the :code:`addEquivalence` function returns a boolean indicating success or otherwise.
  If you check this as you go it will alert you quickly if you're trying to connect to a variable that's not found.

.. container:: dothis

    **6.h** From the printout of your model and the issues listed, determine what needs to happen in order to make the model viable, and do it.
    Check that your final analysis contains no issues.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  6.h
        :end-before: //  end 6

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  6.h
        :end-before: #  end 6

Step 7: Serialise and print the repaired model
----------------------------------------------

.. container:: dothis

    **7.a** Create a :code:`Printer` instance and use it to print the CellML-formatted version of the repaired model to a string.
    Remember we'll still be printing the original version of the model, not the flattened one!

.. container:: dothis

    **7.b** Write the string to a file named "SodiumChannelModel.cellml"; you will use this in :ref:`Tutorial 4<generateMembraneModel>`.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  7.a
        :end-before: //  end

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  7.a
        :end-before: #  end
