..  _combine_debugSodiumChannelModel:

Tutorial 3: Debugging a model
=============================
By the time you have worked through this part of the tutorial you will be able to:

- Parse an existing CellML file and deserialise it into a model instance;
- Use the diagnostic :code:`Validator` class to identify issues in the model's definition; 
- Use the :code:`std::any_cast` to retrieve items which need repair from :code:`Issue` items;
- Use the :code:`Importer` class to resolve imports and identify issues; and
- Use the diagnostic :code:`Analyser` class to identify issues in the model's mathematical formulation.

**Requirements (C++)**

- :download:`CMakeLists.txt<code/CMakeLists_sodium.txt>` The CMake file for building this tutorial;
- :download:`debugSodiumChannelModel.cpp<code/debugSodiumChannelModel.cpp>` Either the skeleton code, or ..
- :download:`debugSodiumChannelModel_completed.cpp<code/debugSodiumChannelModel_completed.cpp>` the completed tutorial code;
- :download:`GateModel.cellml<code/GateModel.cellml>` the generic gate model created in Tutorial 1;
- :download:`SodiumChannelModel_broken.cellml<code/SodiumChannelModel_broken.cellml>` a sodium channel model that needs debugging;
- :download:`CircularControllerReference.cellml<code/CircularControllerReference.cellml>` import file;
- :download:`CircularControllerReference2.cellml<code/CircularControllerReference2.cellml>` import file; and
- :download:`SodiumChannelModelController.cellml<code/SodiumChannelModelController.cellml>` import file;

**Requirements (Python)**

- :download:`debugSodiumChannelModel.py<code/debugSodiumChannelModel.py>` Either the skeleton code, or ..
- :download:`debugSodiumChannelModel_completed.py<code/debugSodiumChannelModel_completed.py>` the completed tutorial code; and
- :download:`GateModel.cellml<code/GateModel.cellml>` the generic gate model created in Tutorial 1;
- :download:`SodiumChannelModel_broken.cellml<code/SodiumChannelModel_broken.cellml>` a sodium channel model that needs debugging;
- :download:`CircularControllerReference.cellml<code/CircularControllerReference.cellml>` import file;
- :download:`CircularControllerReference2.cellml<code/CircularControllerReference2.cellml>` import file; and
- :download:`SodiumChannelModelController.cellml<code/SodiumChannelModelController.cellml>` import file;

.. contents:: Contents
    :local:

Overview
--------
**TODO**

Step 1: Parse the existing sodium channel model
-----------------------------------------------
The :code:`Parser` class is used to deserialise a CellML string into a :code:`Model` instance.
This means that you're responsible for finding, opening and reading the *.cellml file into a single string.
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

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  1.a
        :end-before: //  end 1

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  1.a
        :end-before: #  end 1

Step 2: Validate the parsed model
---------------------------------
Create a :code:`Validator` item and use it to validate the model you've just read.

.. container:: dothis

    **2.a** Create a :code:`Validator` item and validate the model.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: //  end 2.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  2.a
        :end-before: #  end 2.a

Each :code:`Issue` contains:
- a description of the problem;
- the reference heading in the normative specification which affects this issue;
- a URL at which the informative specification notes can be found;
- an std::any item storing the CellML element most relevant to the issue; and
- a level indication. 

.. container:: dothis

    **2.b** Retrieve any issues from the validator and print their descriptions and help URL to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  2.b
        :end-before: //  end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  2.b
        :end-before: #  end 2

Step 3: Repair the parsed model
-------------------------------
The messages returned from the validator (and other classes) should (!) have enough information to enable you to know what the problem is.
In the case of the validator class, the URL listed contains additional resources and examples related to the issue, as well as its section in the CellML normative specification.

In some situations more than one :code:`Issue` will be generated from a single cause: this is the case with issues 0 and 1 here:

.. code-block:: terminal

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

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: //  end 3.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.a
        :end-before: #  end 3.a

.. code-block:: terminal

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

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.b
        :end-before: //  end 3.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.b
        :end-before: #  end 3.b

.. code-block:: terminal

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
     - retrieving an :code:`AnyItem` structure (whose "first" attribute is an enum of the :code:`ItemType`; 
       and "second" attribute is an std::any cast of the item itself); and casting it appropriately, or
     - since we know that the type of item in this error is a :code:`VARIABLE`, we can call the convenience method Issue::variable() to return the variable which needs attention.
 (Of course you could retrieve it using the name of its parent component and its name too - this is just another way!)

.. container:: dothis

    **3.c** Check that the item to be returned from the issue is in fact an :code:`ItemType::VARIABLE` by calling the :code:`type()` function.
    Retrieve the variable missing units from the issue.
    Set its units to be millivolts.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.c
        :end-before: //  end 3.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.c
        :end-before: #  end 3.c

The error below indicates that a child :code:`Unit` references something which can't be found.

.. code-block:: terminal

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
    
    **Useful functions**

    :api:`Units class<Units>`

    - unitAttributes
    - removeUnit
    - addUnit

.. container:: dothis

    **3.d** Choose your preferred method and use it to retrieve the problem unit attributes and print them all to the terminal.
    Then fix the issue.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.d
        :end-before: //  end 3.d

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.d
        :end-before: #  end 3.d

The final validator issue refers to the fact that we need to explicitly specify how other components can access each of the variables in this component.

.. code-block:: terminal

    Issue 9: Variable 't' in component 'sodiumChannelEquations' has no interface type set. The interface type required is 'public_and_private'.
        reference: 3.10.8
        see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specC10.html?issue=3.10.8

.. container:: dothis

    **3.e** Retrieve the variable either using the issue pointer method, or using the name method, and set its interface to be the required type.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.e
        :end-before: //  3.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.e
        :end-before: #  3.f

.. container:: dothis

    **3.f** Revalidate the model and confirm that the errors have gone.

.. container:: dothis

    **3.g** Even though the model is free from validation errors, we still need to make sure it represents what we want it to.
    Print the model to the terminal and check its structure.

.. container:: useful

    Useful functions: printModel(Model);  **TODO**

.. container:: dothis

    **3.h** Use the addComponent functions to rearrange the components as needed until you have the required structure.Validate the model again.

.. container:: useful

    **Useful functions**

    :api:`Model class<Model>`

    - component("componentName", true) will search for the component's name in the whole of the encapsulation hierarchy.

    :api:`Component class<Component>`

    - addComponent

    Tutorial functions

    - printEncapsulation **TODO**

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  3.g
        :end-before: //  end 3

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  3.g
        :end-before: #  end 3

Step 4: Resolve the model's imports
-----------------------------------
It's important to remember that the imports are merely instructions for how components or units items should be located: only their syntax is checked by the validator, not that the files exist or contain the required information.  To debug the imported aspects of the model, we need to use an :code:`Importer` class.

To resolve the imports, we need a path to a base location against which any relative file addresses can be resolved.  
For this tutorial, the files are in the same directory as the code, so simply using an empty string is sufficient.

.. container:: nb 

    If they're another directory, make sure to end your path with a slash, "/".  If they're in your working directory, enter an empty string.

.. container:: useful

    **Useful functions**
    
    :api:`Importer class<Importer>`
    
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

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  4.a
        :end-before: //  end 4.b

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  4.a
        :end-before: #  end 4.b

.. code-block:: terminal

    Importer error[0]:
        Description: Import of component 'importedGateH' from 'GateModel.cellml' requires 
        component named 'i_dont_exist' which cannot be found.

.. container:: dothis

    **4.c** Fix the issues reported by the importer.
    This needs to be an iterative process as more files become available to the importer.
    We need to change the import reference for the component to be "gateEquations" instead of "i_dont_exist".
    You can either retrieve the component using its name or directly from the issue.

.. container:: useful

    **Useful functions**

    :api:`Component class<Component>`

    - setImportReference

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  4.c
        :end-before: //  end 4.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  4.c
        :end-before: #  end 4.c

.. code-block:: terminal

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

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  4.d
        :end-before: //  end 4

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  4.d
        :end-before: #  end 4

Step 5: Validate the imported dependencies
------------------------------------------
At this stage we've validated the local model, and we've used the :code:`Importer` class to retrieve all of its import dependencies.
These dependencies are stored in the importer's library, and have not yet been validated or analysed.  

.. container:: useful

    **Useful functions**

    :api:`Importer class<Importer>`

         - libraryCount returns the number of stored models;
         - library returns the model at the given index or given key string;
         - key returns a key string at the given index;

.. container:: dothis

    **5.a** Use a simple loop to validate each of the models stored in the importer's library.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  5.a
        :end-before: //  end 5.a

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  5.a
        :end-before: #  end 5.a

Note that the two files creating the circular import in 4.a are still in the library. 
To limit ourselves to only those models which are still relevant as the import dependencies of our repaired model, we can iterate through our model's :code:`ImportSource` items instead.  
As soon as the model's imports have been resolved, all these will point to instantiated models within the importer.

.. container:: useful 

    **Useful functions**

    :api:`Model class<Model>`

    - importSourceCount
    - importSource

    :api:`ImportSource class<ImportSource>`

    - model
    - url

.. container:: dothis

    **5.b** Loop through the model's import source items and print their URLs to the terminal.
    You'll notice that these have been used as the keys in the importer library.
    Check that the importer library's models are the same as that attached to the import source item.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  5.b
        :end-before: //  end 5

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  5.b
        :end-before: #  end 5

Step 6: Analyse the model(s)
----------------------------
As with the validator, the :code:`Analyser` class is a diagnostic class which will check whether the mathematical representation is ready for simulation.
This involves making sure that variables are contained in equations, that integrated variables have initial conditions, and that there are no over- or under-constrained sets of equations.
Since this model uses imports, the real mathematical model is hidden from the :code:`Analyser` (just as it was from the :code:`Validator`).
The way around this is to use the :code:`Importer` class to create a flat (ie: import-free) version of the same model.
If the flat model meets the analyser's checks, then the importing version will too.


.. container:: useful

    **Useful functions**

    :api:`Analyser class<Analyser>`

    - analyseModel
    - flattenModel

.. container:: dothis

    **6.a** Create an Analyser instance and pass in the model for analysis.

.. container:: dothis

    **6.b** Retrieve and print the issues from the analysis to the screen.
    We expect to see messages related to un-computed variables, since anything which is imported is missing from this model.

.. code-block:: terminal

    **TODO**

.. container:: dothis

    **6.c** Create a flattened version of the model print it to the screen.
    Notice that any comments indicating that a component was an import have been removed as these components have been instantiated in the flattened model.

.. container:: dothis

    **6.d** Analyse the flattened model and print the issues to the screen.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  6.a
        :end-before: //  end 6.d

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  6.a
        :end-before: #  end 6.d

.. code-block:: terminal

    Issue [0] is an ERROR:
        description: Variable 't' in component 'importedGateM' and variable 't' in component
                 'importedGateH' cannot both be the variable of integration.
        stored item type: VARIABLE

The issue returned from the analyser says that we're trying to use two different variables as the base variable of integration, and the CellML code generation facility (which the analyser is tied to) does not support this yet.
It's still valid CellML though!
In this example, the real problem is that these two variables are talking about the same thing, but haven't been connected to one another yet.

.. container:: useful

    **Useful functions**

    :api:`Variable class<Variable>`

    - addEquivalence

.. container:: dothis

    **6.e** Create any necessary variable equivalences so that these two variables are connected.
    You can refer to your printout of the model's structure to help if need be, and remember that only variables in a sibling or parent/child relationship can be connected. 

.. container:: dothis

    **6.f** Re-flatten and re-analyse the model and print the issues to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  6.e
        :end-before: //  end 6.f

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  6.e
        :end-before: #  end 6.f

Now we see the importance of checking iteratively for issues in the analyser class!  
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

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  6.g
        :end-before: //  end 6.g

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
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
Hints: 

- There is at least one of each kind of problem;
- There's a convenience function provided which will print the equivalent variable set for a given variable.
  You can use the item stored by each issue and this function to check for missing connections: printEquivalentVariableSet(variable);
- the addEquivalence function returns a boolean indicating success or otherwise.
  If you check this as you go it will alert you quickly if you're trying to make an illegal connection.

.. container:: dothis

    **6.h** From the printout of your model and the issues listed, determine what needs to happen in order to make the model viable, and do it.
    Check that your final analysis contains no issues.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  6.h
        :end-before: //  end 6

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  6.h
        :end-before: #  end 6

Step 7: Serialise and print the repaired model
----------------------------------------------

.. container:: dothis

    **7.a** Create a :code:`Printer` instance and use it to print the CellML-formatted version of the repaired model to a string.
    Remember we'll still be printing the original version of the model, not the flattened one!

auto printer = libcellml::Printer::create();
auto modelString = printer->printModel(model);

.. container:: dothis

    **7.b** Write the string to a file named "SodiumChannelModel.cellml".

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.cpp
        :language: c++
        :start-at: //  7.a
        :end-before: //  end

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/debugSodiumChannelModel_completed.py
        :language: python
        :start-at: #  7.a
        :end-before: #  end
