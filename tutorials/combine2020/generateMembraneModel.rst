..  _combine_generateMembraneModel:

Tutorial 4: Generating code from a model
========================================

By the time you've worked through this tutorial you will be able to:

    - Use the Generator class to create C or Python code representing a CellML model.

This tutorial assumes you're already comfortable with:

    - Parsing an existing CellML file into a model instance (see TODO);
    - Using the diagnostic :code:`Validator` class to check for syntactic issues; 
    - Using the :code:`Importer` class to resolve and flatten imports; and
    - Using the :code:`Analyser` class to check for mathematical issues in the model. 
    - Writing to files. 

.. contents::
    :local:


All of the ingredients have been assembled for us to parse a membrane model so that it can be turned into runnable code using the code generation functionality.
We will parse the model, resolve its imports, flatten into an import-free model, validate, analyse, and generate.
By this stage you should be familiar with most of these processes: we'll go through the code generation in detail at the end.

Step 1: Parse the existing membrane model
-----------------------------------------

.. container:: dothis

    Parse the model in the "MembraneModel.cellml" file provided and print its contents to the terminal.

.. tabs::

    .. code-tab:: text

        Open the other tabs for the code.

    .. code-tab:: c++

        // Read the string from the file.
        std::ifstream inFile("MembraneModel.cellml");
        std::stringstream inFileContents;
        inFileContents << inFile.rdbuf();

        //  Create a Parser and use it to create a model.
        auto parser = libcellml::Parser::create();
        auto model = parser->parseModel(inFileContents.str());

        //  Print the model to the terminal.
        printModel(model, false);

    .. code-tab:: python

        # Python goes here.


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

.. tabs::

    .. code-tab:: text

        Open the other tabs for the code.

    .. code-tab:: c++

        // Resolve the imports in this model using a new Importer instance.
        auto importer = libcellml::Importer::create();
        importer->resolveImports(model, "");
        printIssues(importer);

        // Create a flattened version of the model.
        auto flatModel = importer->flattenModel(model);

    .. code-tab:: python

        # Resolve the imports in this model using a new Importer instance.
        importer = Importer()
        importer.resolveImports(model, '')
        print_issues(importer)

        # Create a flattened version of the model.
        flat_model = importer.flattenModel(model)


Step 3: Validate and analyse the flattened model
------------------------------------------------
You know what to do ... we do not expect any issues to be raised by either the validator or the analyser.

.. tabs::

    .. code-tab:: text

        Open the other tabs for the code.

    .. code-tab:: c++

        // Create a Validator and use it to check the flattened model.
        auto validator = libcellml::Validator::create();
        validator->validateModel(flatModel);
        printIssues(validator);

        // Create an Analyser and use it to check the flattened model.
        auto analyser = libcellml::Analyser::create();
        analyser->analyseModel(flatModel); 
        printIssues(analyser);
    
    .. code-tab:: python

        # Create a Validator and use it to check the flattened model.
        validator = Validator()
        validator.validateModel(flat_model)
        print_issues(validator)

        # Create an Analyser and use it to check the flattened model.
        analyser = Analyser()
        analyser.analyseModel(flat_model)
        print_issues(analyser)


Step 4: Generate code and output
--------------------------------
**TODO**

.. container:: dothis

    **4.a** Create a :code:`Generator` instance.  

.. container:: dothis

    **4.b** The generator uses a GeneratorProfile item to set up a translation between the model stored as CellML and the language of your choice (currently C or Python).
    Create a GeneratorProfile object, and use the constructor argument of the libcellml::GeneratorProfile::Profile enum for the language you want (C or PYTHON).

.. container:: dothis

    **4.c** Use the generator's setProfile function to pass in the profile item you just created.

.. tabs::

    .. code-tab:: text

        Open the other tabs for the code.

    .. code-tab:: c++

        auto generator = libcellml::Generator::create();
        auto profile = libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::C);
        generator->setProfile(profile);

    .. code-tab:: python

        generator = Generator()
        profile = GeneratorProfile(GeneratorProfile.Profile.PYTHON)
        generator.setProfile(profile)


Instead of submitting a :code:`Model` item (as we do for all other classes), the :code:`Generator` class will work from something which has already been processed by the :code:`Analyser` class: an :code:`AnalyserModel` object.
    
.. container:: dothis

    **4.d** Retrieve the analysed model using the Analyser::model() function, and submit to the generator using the Generator::setModel(analysedModel) function.

.. container:: infospec

    **Useful functions**

    Analyser::model 
    Generator::setModel

.. container:: dothis

    **4.e** (C only) If you're using the C profile then you have the option at this stage to specify the file name of the interface file you'll create in the next step.  
    This means that the two files will be prepared to link to one another without manual editing later.
    You can do this by specifying the header file name in the :code:`GeneratorProfile` item using the setInterfaceFileNameString("yourHeaderFileNameHere.h") function.
    This will need to be the same as the file which you write to in step 4.g below.

.. container:: dothis

    **4.f** Implementation code is the bulk of the model, and contains all the equations, variables, units etc.
    This is needed for both of the available profiles, and would normally be stored in a *.cpp or *.py file.  
    Use the Generator::implementationCode() function to return the implementation code as a string, and write it to a file with the appropriate extension.

.. container:: dothis

    **4.g** (C only) Interface code is the header needed by the C profile to define data types.
    Use the Generator::interfaceCode() function to return interface code as a string and write it to a *.h header file.This needs to be the same filename as you specified in step 4.e above.


.. tabs::

    .. code-tab:: text

        Open the other tabs for the code.

    .. code-tab:: c++

        generator->setModel(analyser->model());

        profile->setInterfaceFileNameString("HodgkinHuxleyModel.h");

        std::ofstream outFile("HodgkinHuxleyModel.c");
        outFile << generator->implementationCode();
        outFile.close();

        outFile.open("HodgkinHuxleyModel.h");
        outFile << generator->interfaceCode();
        outFile.close();
    
    .. code-tab:: python

        generator.setModel(analyser.model())

        write_file = open('MembraneModel.py', 'w')
        write_file.write(generator.implementationCode())
        write_file.close()

