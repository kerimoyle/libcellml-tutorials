..  _combine_generateMembraneModel:

Tutorial 4: Generating code from a model
========================================

.. container:: shortlist

    By the time you've worked through this tutorial you will be able to:

    - Use the Generator class to create C or Python code representing a CellML model.

    This tutorial assumes you're already comfortable with:

    - Parsing an existing CellML file into a model instance;
    - Using the diagnostic :code:`Validator` class to check for syntactic issues; 
    - Using the :code:`Importer` class to resolve and flatten imports; and
    - Using the :code:`Analyser` class to check for mathematical issues in the model. 
    - Writing to files. 

.. container:: shortlist

    **Code (C++)**

    - :download:`CMakeLists.txt<code/tut4/CMakeLists.txt>` The CMake file for building this tutorial;
    - :download:`generateMembraneModel.cpp<code/tut4/generateMembraneModel.cpp>` Either the skeleton code, or ..
    - :download:`generateMembraneModel_completed.cpp<code/tut4/generateMembraneModel_completed.cpp>` the completed tutorial code; and
    - :download:`tutorial_utilities.cpp<code/tutorial_utilities.cpp>` and :download:`tutorial_utilities.h<code/tutorial_utilities.h>` Helper functions.

.. container:: shortlist

    **Code (Python)**

    - :download:`generateMembraneModel.py<code/tut4/generateMembraneModel.py>` Either the skeleton code, or ..
    - :download:`generateMembraneModel_completed.py<code/tut4/generateMembraneModel_completed.py>` the completed tutorial code;

.. container:: shortlist

    **Resources**

    - :download:`GateModel.cellml<code/resources/GateModel.cellml>` the generic gate model (from Tutorial 1);
    - :download:`PotassiumChannelModel.cellml<code/resources/PotassiumChannelModel.cellml>` the potassium channel model (from Tutorial 2);
    - :download:`SodiumChannelModel.cellml<code/resources/SodiumChannelModel.cellml>` the sodium channel model (from Tutorial 3);
    - :download:`LeakageModel.cellml<code/resources/LeakageModel.cellml>` an import dependency representing current leakage; 
    - :download:`MembraneController.cellml<code/resources/MembraneController.cellml>` an import dependency for the membrane model controller; and
    - :download:`MembraneModel.cellml<code/resources/MembraneModel.cellml>` the file to parse.

**Contents**

.. contents::
    :local:

All of the ingredients have been assembled for us to parse a membrane model so that it can be turned into runnable code using the code generation functionality.
We will parse the model, resolve its imports, flatten into an import-free model, validate, analyse, and generate.
By this stage you should be familiar with most of these processes: we'll go through the code generation in detail at the end.

Step 1: Parse the existing membrane model
-----------------------------------------

.. container:: dothis

    Parse the model in the "MembraneModel.cellml" file provided and print its contents to the terminal.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.cpp
        :language: c++
        :start-at: //  1.a
        :end-before: //  end 1

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.py
        :language: python
        :start-at: #  1.a
        :end-before: #  end 1

.. code-block:: terminal

    MODEL: 'MembraneModel'
    UNITS: 5 custom units
        [0]: mV
        [1]: ms
        [2]: mS_per_cm2
        [3]: microA_per_cm2
        [4]: microF_per_cm2
    COMPONENTS: 2 components
        [0]: controller <--- imported from: 'controller' in 'Controller.cellml'
            VARIABLES: 2 variables
                [0]: i_stim
                      └──> membrane:i_stim [microA_per_cm2]
                [1]: t
                      └──> membrane:t [ms]
        [1]: membrane
            VARIABLES: 3 variables
                [0]: t [ms]
                      └──> membraneEquations:t [ms], controller:t
                [1]: i_tot [microA_per_cm2]
                      └──> membraneEquations:i_tot [microA_per_cm2]
                [2]: i_stim [microA_per_cm2]
                      └──> membraneEquations:i_stim [microA_per_cm2], controller:i_stim
            COMPONENT membrane has 2 child components:
                [0]: membraneEquations
                    VARIABLES: 8 variables
                        [0]: V [mV]
                              └──> membraneParameters:V [mV], sodiumChannel:V, potassiumChannel:V, leakage:V
                        [1]: t [ms]
                              └──> membrane:t [ms], sodiumChannel:t, potassiumChannel:t
                        [2]: i_K [microA_per_cm2]
                              └──> potassiumChannel:i_K
                        [3]: i_Na [microA_per_cm2]
                              └──> sodiumChannel:i_Na
                        [4]: i_L [microA_per_cm2]
                              └──> leakage:i_L
                        [5]: i_stim [microA_per_cm2]
                              └──> membrane:i_stim [microA_per_cm2]
                        [6]: i_tot [microA_per_cm2]
                              └──> membrane:i_tot [microA_per_cm2]
                        [7]: Cm [microF_per_cm2]
                              └──> membraneParameters:Cm [microF_per_cm2]
                    COMPONENT membraneEquations has 3 child components:
                        [0]: sodiumChannel <--- imported from: 'sodiumChannel' in 'SodiumChannelModel.cellml'
                            VARIABLES: 3 variables
                                [0]: t
                                      └──> membraneEquations:t [ms]
                                [1]: i_Na
                                      └──> membraneEquations:i_Na [microA_per_cm2]
                                [2]: V
                                      └──> membraneEquations:V [mV]
                        [1]: potassiumChannel <--- imported from: 'potassiumChannel' in 'PotassiumChannelModel.cellml'
                            VARIABLES: 3 variables
                                [0]: t
                                      └──> membraneEquations:t [ms]
                                [1]: i_K
                                      └──> membraneEquations:i_K [microA_per_cm2]
                                [2]: V
                                      └──> membraneEquations:V [mV]
                        [2]: leakage <--- imported from: 'leakage' in 'LeakageModel.cellml'
                            VARIABLES: 4 variables
                                [0]: i_L
                                      └──> membraneEquations:i_L [microA_per_cm2]
                                [1]: g_L
                                [2]: E_L
                                [3]: V
                                      └──> membraneEquations:V [mV]
                [1]: membraneParameters
                    VARIABLES: 2 variables
                        [0]: Cm [microF_per_cm2], initial = 1
                              └──> membraneEquations:Cm [microF_per_cm2]
                        [1]: V [mV], initial = 1
                              └──> membraneEquations:V [mV]

Step 2: Resolve the imports and flatten
---------------------------------------
In all of the other tutorials we've used a flattened model only to analyse its mathematics.
This time, we need to keep the flattened version and will use this as input to the code generator.
Resolve the imports, and create a flattened version of the model.
We do not expect any issues to be reported by the importer.

.. container:: dothis

    **2.a** Create an Importer instance and use it to resolve the imports in your model.

.. container:: dothis

    **2.b** Check that the importer has not raised any issues.

.. container:: dothis

    **2.c** Use the importer to create a flattened version of the model.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.cpp
        :language: c++
        :start-at: //  2.a
        :end-before: //  end 2

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.py
        :language: python
        :start-at: #  2.a
        :end-before: #  end 2

Step 3: Validate and analyse the flattened model
------------------------------------------------
You know what to do ... we do not expect any issues to be raised by either the validator or the analyser.

.. container:: dothis

    **3.a** Create a :code:`Validator` instance, pass in the flattened model, and check that there are no issues raised.

.. container:: dothis

    **3.b** Create an :code:`Analyser` instance, pass in the flattened model, and check that there are no issues raised.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.cpp
        :language: c++
        :start-at: //  3.a
        :end-before: //  end 3

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.py
        :language: python
        :start-at: #  3.a
        :end-before: #  end 3

Step 4: Generate code and output
--------------------------------
The :code:`Generator` is a translator class that will change the CellML model and its MathML equations into a representation in another language.
This is done using a :code:`GeneratorProfile` to specify a dictionary of mathematical operations.
Two profiles are already defined; for C++ and for Python.

.. container:: useful

	**Useful functions**

	:api:`GeneratorProfile class<GeneratorProfile>`

	- create
	
	:api:`Generator class<Generator>`

	- create
	- setProfile

.. container:: dothis

    **4.a** Create a :code:`Generator` instance.  

.. container:: dothis

    **4.b** Create a GeneratorProfile object, and use the constructor argument of the :code:`libcellml::GeneratorProfile::Profile` enum for the language you want (C or PYTHON).

.. container:: dothis

    **4.c** Use the generator's :code:`setProfile` function to pass in the profile item you just created.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.cpp
        :language: c++
        :start-at: //  4.a
        :end-before: //  end 4.a

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.cpp
        :language: c++
        :start-at: //  4.b
        :end-before: //  end 4.c

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.py
        :language: python
        :start-at: #  4.a
        :end-before: #  end 4.a
    

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.py
        :language: python
        :start-at: #  4.b
        :end-before: #  end 4.c

Instead of submitting a :code:`Model` item (as we do for all other classes), the :code:`Generator` class will work from something which has already been processed by the :code:`Analyser` class: an :code:`AnalyserModel` object.
    
.. container:: useful

    **Useful functions**

    :api:`Analyser class<GeneratorProfile>`

    - model

    :api:`Generator class<Generator>`

    - setModel
    - implementationCode
    - interfaceCode

    :api:`GeneratorProfile class<GeneratorProfile>`

    - setInterfaceFileNameString

.. container:: dothis

    **4.d** Retrieve the analysed model using the Analyser::model() function, and submit to the generator using the Generator::setModel(analysedModel) function.

.. container:: dothis

    **4.e** (C only) If you're using the C profile then you have the option at this stage to specify the file name of the interface file you'll create in the next step.  
    This means that the two files will be prepared to link to one another without manual editing later.
    You can do this by specifying the header file name in the :code:`GeneratorProfile` item using its :code:`setInterfaceFileNameString` function.
    This will need to be the same as the file which you write to in step 4.g below.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.cpp
        :language: c++
        :start-at: //  4.d
        :end-before: //  end 4.d

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.cpp
        :language: c++
        :start-at: //  4.e
        :end-before: //  end 4.e

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.py
        :language: python
        :start-at: #  4.d
        :end-before: #  end 4.d

.. container:: dothis

    **4.f** Implementation code is the bulk of the model, and contains all the equations, variables, units etc.
    This is needed for both of the available profiles, and would normally be stored in a *.cpp or *.py file.  
    Use the :code:`implementationCode` function to return the implementation code as a string, and write it to a file with the appropriate extension.

.. container:: dothis

    **4.g** (C only) Interface code is the header needed by the C profile to define data types.
    Use the :code:`interfaceCode` function to return interface code as a string and write it to a *.h header file.
    This needs to be the same filename as you specified in step 4.e above.

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.cpp
        :language: c++
        :start-at: //  4.f
        :end-before: //  end 4

.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/tut4/generateMembraneModel_completed.py
        :language: python
        :start-at: #  4.f
        :end-before: #  end 4
    

