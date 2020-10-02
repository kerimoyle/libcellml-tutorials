..  _combine_createPotassiumChannelModel:

Tutorial 2: Creating a model using imports
==========================================

By the time you have worked through this tutorial you will be able to:
- Assemble a multi-component model using the API; 
- Import items from existing models and reuse them here;
- Inter-connect the components using the equivalent variables functionality;
- Use the diagnostic :code:`Validator` class to identify errors in the model's syntax; 
- Use the diagnostic :code:`Analyser` class to identify errors in the model's mathematical formulation; and
- Serialise the model to CellML format for output.

**Requirements (C++)**

- :download:`CMakeLists.txt` The CMake file for building this tutorial;
- :download:`createPotassiumChannelModel.cpp` Either the skeleton code, or ..
- :download:`createPotassiumChannelModel_completed.cpp` the completed tutorial code; and
- :download:`GateModel.cellml` The generic gate model created in Tutorial 1.

**Requirements (Python)**

- :download:`createPotassiumChannelModel.py` Either the skeleton code, or ..
- :download:`createPotassiumChannelModel_completed.py` the completed tutorial code; and
- :download:`GateModel.cellml` The generic gate model created in Tutorial 1.

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


.. container:: dothis

    **1.a** Create a Model and name it appropriately.
        auto model = libcellml::Model::create("PotassiumChannelModel");

.. container:: dothis

    **1.b** Create a wrapping component and name it "potassiumChannel".
        auto kChannel = libcellml::Component::create("potassiumChannel");

.. container:: dothis

    **1.c** Add the component to the model.

        model->addComponent(kChannel);

.. container:: dothis

    **2.a** Create a Component instance for the equations and name it "potassiumChannelEquations".  
    Add it to the wrapper component you created above.

        auto kChannelEquations = libcellml::Component::create("potassiumChannelEquations");
        kChannel->addComponent(kChannelEquations);

.. container:: dothis

    **2.b** Define the maths inside the potassiumChannelEquations component.
    This is a MathML 2 string (NB: higher versions of MathML are not supported), and is added to the component using :code:`setMath()` and :code:`appendMath()` functions.
    Your string needs to contain the namespaces for MathML and for CellML: these have been provided for you in the mathHeader string above.

        std::string equationIK =
            "  <apply><eq/>\n"
            "    <ci>i_K</ci>\n"
            "    <apply><times/>\n"
            "       <apply><power/>\n"
            "           <ci>n</ci>\n"
            "           <cn cellml:units=\"dimensionless\">4</cn>\n"
            "       </apply>\n"
            "       <ci>g_K</ci>\n"
            "       <apply><minus/>\n"
            "           <ci>V</ci>\n"
            "           <ci>E_K</ci>\n"
            "       </apply>\n"
            "    </apply>\n"
            "  </apply>\n";

        kChannelEquations->setMath(mathHeader);
        kChannelEquations->appendMath(equationIK);
        kChannelEquations->appendMath(mathFooter);
        
Once the mathematics has been added to the component, and the component to the model, we can make use of the diagnostic messages within the :code:`Validator` class to tell us what else needs to be done.

.. container:: dothis

    **2.c** Create a Validator instance, and pass it your model for processing using the :code:`validateModel`function.

        auto validator = libcellml::Validator::create();
        validator->validateModel(model);

Calling the validator does not return anything: we have to go looking for issues that it found during processing.  
When a problem is found, an :code:`Issue` item is created containing:
- a description string explaining the problem;
- a URL at which more information is available;
- an std::any item relevant to the problem, if available;
- a level indicator; and
- a cause indicator relevant to the stored item.

We can use these issues as we need to.
The simplest way is to print the descriptions to the terminal.

.. container:: dothis

    **2.d** Retrieve the number of issues encountered using the :code:`issueCount()` function in the validator, then retrieve the issue items from the validator using their index and the :code:`Validator::issue(index)` function.

        std::cout << "The validator has found " << validator->issueCount() << " issues." << std::endl;
        for(size_t i = 0; i < validator->issueCount(); ++i) {
            std::cout << validator->issue(i)->description() << std::endl;
        }
        std::cout << std::endl;

.. container:: dothis

    **2.e** Create the variables needed and add them to the potassium channel component.
    Revalidate and expect errors related to variables without units.

        kChannelEquations->addVariable(libcellml::Variable::create("E_K"));
        kChannelEquations->addVariable(libcellml::Variable::create("i_K"));
        kChannelEquations->addVariable(libcellml::Variable::create("g_K"));
        kChannelEquations->addVariable(libcellml::Variable::create("V"));
        kChannelEquations->addVariable(libcellml::Variable::create("t"));
        kChannelEquations->addVariable(libcellml::Variable::create("n"));

.. container:: dothis

    **2.f** Create the missing Units items and add them to the model. These are:
    - milli-volts;
    - milli-seconds;
    - milli-moles;
    - micro-Amperes per square centimetre; and
    - milli-Siemens per square centimetre;

        auto mV = libcellml::Units::create("mV");
        mV->addUnit("volt", "milli");

        auto microA_per_cm2 = libcellml::Units::create("microA_per_cm2");
        microA_per_cm2->addUnit("ampere", "micro");
        microA_per_cm2->addUnit("metre", "centi", -2.0);

        auto mS_per_cm2 = libcellml::Units::create("milliS_per_cm2");
        mS_per_cm2->addUnit("siemens", "milli");
        mS_per_cm2->addUnit("metre", "centi", -2.0);

        auto ms = libcellml::Units::create("ms");
        ms->addUnit("second", "milli");

        auto mM = libcellml::Units::create("mM");
        mM->addUnit("mole", "milli");

        model->addUnits(ms);
        model->addUnits(mV);
        model->addUnits(mM);
        model->addUnits(microA_per_cm2);
        model->addUnits(mS_per_cm2);

.. container:: dothis

    **2.g** Set the units on each of the variables.  
    Call the validator again, and expect there to be no errors.

        kChannelEquations->variable("E_K")->setUnits(mV);
        kChannelEquations->variable("i_K")->setUnits(microA_per_cm2);
        kChannelEquations->variable("g_K")->setUnits(mS_per_cm2);
        kChannelEquations->variable("V")->setUnits(mV);
        kChannelEquations->variable("t")->setUnits(ms);
        kChannelEquations->variable("n")->setUnits("dimensionless");

        validator->validateModel(model);
        printIssues(validator);

Step 3: Create the nGate and its child components
-------------------------------------------------

The nGateEquations has some of the working of a generic gate (which we'll import from GateModel.cellml), but instead of constant values for alpha and beta, we'll introduce a voltage dependence.
The nGateParameters component allows us to specify those parameters specific to the movement of potassium.

.. container:: dothis

    **3.a** Create a component, name it "nGate", and add it to the equations component.

        auto nGate = libcellml::Component::create("nGate");
        kChannelEquations->addComponent(nGate);

.. container:: dothis

    **3.b** Create a component, name it "nGateEquations" and add it to the nGate component.

        auto nGateEquations = libcellml::Component::create("nGateEquations");
        nGate->addComponent(nGateEquations);

.. container:: dothis

    **3.c** Add the mathematics to the nGateEquations component and validate the model.
    Expect errors relating to missing variables.

        std::string equationAlphaN =
            "  <apply><eq/>\n"
            "    <ci>alpha_n</ci>\n"
            "    <apply><divide/>\n"
            "      <apply><times/>\n"
            "        <cn cellml:units=\"per_mV_ms\">0.01</cn>\n"
            "        <apply><plus/>\n"
            "          <ci>V</ci>\n"
            "          <cn cellml:units=\"mV\">10</cn>\n"
            "        </apply>\n" 
            "      </apply>\n" 
            "      <apply><minus/>\n"
            "        <apply><exp/>\n"
            "          <apply><divide/>\n"
            "            <apply><plus/>\n"
            "              <ci>V</ci>\n"
            "              <cn cellml:units=\"mV\">10</cn>\n"
            "            </apply>\n" 
            "            <cn cellml:units=\"mV\">10</cn>\n"
            "          </apply>\n" 
            "        </apply>\n" 
            "        <cn cellml:units=\"dimensionless\">1</cn>\n"
            "      </apply>\n" 
            "    </apply>\n" 
            "  </apply>\n"; 

        std::string equationBetaN =
            "  <apply><eq/>\n"
            "    <ci>beta_n</ci>\n"
            "    <apply><times/>\n"
            "      <cn cellml:units=\"per_ms\">0.125</cn>\n"
            "      <apply><exp/>\n"
            "        <apply><divide/>\n"
            "          <ci>V</ci>\n"
            "          <cn cellml:units=\"mV\">80</cn>\n"
            "        </apply>\n" 
            "      </apply>\n" 
            "    </apply>\n" 
            "  </apply>\n"; 

        nGateEquations->setMath(mathHeader);
        nGateEquations->appendMath(equationAlphaN);
        nGateEquations->appendMath(equationBetaN);
        nGateEquations->appendMath(mathFooter);
        
        validator->validateModel(model);
        printIssues(validator);

.. container:: dothis

    **3.d** Add the missing variables to the nGateEquations component, and validate again.
    Expect errors relating to units missing from the variables.

        nGateEquations->addVariable(libcellml::Variable::create("t"));
        nGateEquations->addVariable(libcellml::Variable::create("V"));
        nGateEquations->addVariable(libcellml::Variable::create("alpha_n"));
        nGateEquations->addVariable(libcellml::Variable::create("beta_n"));
        nGateEquations->addVariable(libcellml::Variable::create("n"));
        
        validator->validateModel(model);
        printIssues(validator);

.. container:: dothis

    **3.e** Create the missing units and add them to the model.
    The only two which aren't available are:
    - per millisecond; and 
    - per millivolt millisecond.

    Remember that you'll need to give these names that are the same as those needed by the  variables.
    In this case they are "per_ms" and "per_mV_ms".

        auto per_ms = libcellml::Units::create("per_ms");
        per_ms->addUnit("second", "milli", -1);
        model->addUnits(per_ms);

        auto per_mV_ms = libcellml::Units::create("per_mV_ms");
        per_mV_ms->addUnit("second", "milli", -1);
        per_mV_ms->addUnit("volt", "milli", -1);
        model->addUnits(per_mV_ms);

.. container:: dothis

    **3.f** Associate the correct units items with the variables which need them.
    Revalidate the model, expecting there to be no errors reported.

        nGateEquations->variable("t")->setUnits(ms);
        nGateEquations->variable("V")->setUnits(mV);
        nGateEquations->variable("alpha_n")->setUnits(per_ms);
        nGateEquations->variable("beta_n")->setUnits(per_ms);
        nGateEquations->variable("n")->setUnits("dimensionless");

        validator->validateModel(model);
        printIssues(validator);

Step 4: Specify imports for the generic gate components
-------------------------------------------------------
The generic gate model (in GateModel.cellml) has two components: 
    - "gateEquations" which solves an ODE for the gate status parameter, X; and
    - "gateParameters" which sets the values of alpha, beta, and initialises X.

We will import only the "gateEquations" component and set it to be a child of the nGateEquations component.
This means we can introduce the voltage dependence for the alpha and beta, and using a specified initial value for the gate's status.
Note that the variable "n" in the nGateEquations is equivalent to the generic gate's variable "X".

Imports require three things:
- A destination for the imported item. 
  This could be a :code:`Component` or :code:`Units` item.
- A model to import for the imported item from.
  This is stored in an :code:`ImportSource` item containing the URL of the model to read.
- The name of the item to import.
  This is called the "import reference" and is stored by the destination :code:`Component` or :code:`Units` item.

.. container:: dothis

    **4.a** Create an :code:`ImportSource` item and set its URL to be "GateModel.cellml".

        auto gateModelImportSource = libcellml::ImportSource::create();
        gateModelImportSource->setUrl("GateModel.cellml");

.. container:: dothis

    **4.b** Create a destination component for the imported gate component, and add this to the nGateEquations component. 

        auto importedGate = libcellml::Component::create("importedGate");
        nGateEquations->addComponent(importedGate);

.. container:: dothis

    **4.c** Set the import reference on the component you just created to be the nameof the component in the GateModel.cellml file that you want to use.
    In this example, it is "gateEquations".

        importedGate->setImportReference("gateEquations");

.. container:: dothis

    **4.d** Associate the import source with the component using the setImportSource function.
    Note that this step also makes the import source available to other items through the :code:`Model::importSource(index)` function.
    This way the same model file can be used as a source for more than one item.

        importedGate->setImportSource(gateModelImportSource);

Note that we are deliberately not importing the parameters component in the GateModel.cellml file, since we will be setting our own values of its variables.

.. container:: dothis

    **4.e** Validate the model and confirm that there are no issues.

        validator->validateModel(model);
        printIssues(validator);

Step 5: Specify imports for the controller component
----------------------------------------------------
Repeat Step 4 to import a controller component.
This should be at the top of the encapsulation hierarchy, and should import the component named "controller" from the file "PotassiumChannelController.cellml".

.. container:: dothis

    **5.a** Repeat steps 4.a-d for the controller component.
    Put it at the top level of the encapsulation hierarchy.

        auto controllerImportSource = libcellml::ImportSource::create();
        controllerImportSource->setUrl("PotassiumChannelController.cellml");

        auto controller = libcellml::Component::create("controller");
        controller->setImportReference("controller");
        controller->setImportSource(controllerImportSource);
        model->addComponent(controller);

.. container:: dothis

    **5.b** Validate the model and confirm that there are no issues.

        validator->validateModel(model);
        printIssues(validator)

At this point we've defined the equations that govern the potassium channel's operation.
We've also confirmed that the CellML representation of these equations is valid (using the :code:`Validator`); now we need to check that it's also solvable (using the :code:`Analyser`).
        
Step 6: Analyse the model
-------------------------
We will introduce the :code:`Analyser` class here so that its use as a debugging  tool can be demonstrated.
Of course, we know ahead of time that there is still a lot of connections to be created between the components, but the analyser can help us to find them.

.. container:: nb 

    **A reminder:** We're aiming for a potassium channel component which can accept two external parameters - time, t (ms) and voltage, V (mV) - and use them to calculate a potassium current, i_K (microA_per_cm2). 
    A utility function :code:`printModel(Model, bool)` has been provided to help you to see what's going  on inside your model.
    Setting the second optional parameter to :code:`true` will also print the MathML content.

.. container:: dothis

    **6.a** Create an :code:`Analyser` item and pass it the model for checking using its :code:`analyseModel` function.

        auto analyser = libcellml::Analyser::create();
        analyser->analyseModel(model);

The analyser is similar to the :code:`Validator` class and keeps a record of issues it encounters.

.. container:: dothis

    **6.b** Retrieve the analyser's issues and print them to the terminal, just as you've done for the validator.
    Expect messages related to un-computed variables.

        printIssues(analyser);

Even though all of the messages we see are "variable not calculated" errors, we can divide them into different categories:
- those variables which are constants whose value has not been set yet;
- those variables whose calculation depends on as-yet un-calculated variables;
- those variables which need to be connected to where their calculation happens; and
- those variables which aren't present in any equation.


Step 7: Define the constants
----------------------------
As we work through the next few steps we'll be defining and connecting all of the components and variables together.First we'll define the variables which will have a constant value in the simulation.

.. container:: dothis

    **7.a** Use the :code:`printModel()` function to show your current model contents.
    This should show that we hve currently got variables only in the nGateEquations and potassiumChannelEquations components.
    These need to have sibling parameters components created to hold any hard-coded values or initial conditions that are required.

         printModel(model, true);

Create parameters siblings components for the equations components, and add the variables that they will require.
These are:
- potassium channel parameters
    - ??, E_K (-85)
    - conductance, g_K (??)
- nGate parameters
    - initial value for n (dimensionless)
You can either do this by creating the variables from scratch (as in Step 3.d) but because these are intended to be duplicates of existing variables, but in another component, we can simply add a cloned variable to the parameters component.

.. container:: dothis

    **7.b** Create parameters components for the equations components, and add cloned versions of ny variables which need to be given a value into the new parameters components.

        auto kChannelParameters = libcellml::Component::create("potassiumChannelParameters");
        kChannel->addComponent(kChannelParameters);
        kChannelParameters->addVariable(kChannelEquations->variable("E_K")->clone());
        kChannelParameters->addVariable(kChannelEquations->variable("g_K")->clone());
         kChannelParameters->addVariable(kChannelEquations->variable("n")->clone());

        auto nGateParameters = libcellml::Component::create("nGateParameters");
        nGate->addComponent(nGateParameters);
        nGateParameters->addVariable(nGateEquations->variable("n")->clone());

.. container:: dothis

    **7.c** In order for other encapsulating components to access these variables, they also need to have intermediate variables in the nGate or potassium channel components too.
    This is only true of variables that you want to be available to the outside.
    In this example, we need to add the variable "n" to the nGate in order that its parent (the potassium channel equations) can access it.

        nGate->addVariable(nGateEquations->variable("n")->clone());

.. container:: dothis

    **7.d** Create variable connections between these variables and their counterparts in the equations components.  Validate, expecting errors related to missing or incorrect interface types.

        libcellml::Variable::addEquivalence(kChannelParameters->variable("E_K"), kChannelEquations->variable("E_K"));
        libcellml::Variable::addEquivalence(kChannelParameters->variable("g_K"), kChannelEquations->variable("g_K"));
        libcellml::Variable::addEquivalence(nGate->variable("n"), nGateEquations->variable("n"));

        validator->validateModel(model);
        printIssues(validator);

.. container:: dothis

    **7.e** Set the required interface types as listed by the validator.
    This can be done individually using the :code:`Variable::setInterfaceType()` function, or automatically using the :code:`Model::fixVariableInterfaces()` function.
    Validate again, expecting no validation errors.

        model->fixVariableInterfaces();

        validator->validateModel(model);
        printIssues(validator);
        
If we were to analyse the model again now we would we still have the same set of errors as earlier as we haven't given a value to any of our parameters.
These values should be:

- Potassium channel parameters:
    - E_K = -85 [mV]
    - g_K = 36 [milliS_per_cm2]
- nGate parameters:
    - n = 0.325 []

.. container:: dothis

    **7.f** Use the :code:`Variable::setInitialValue()` function to set these parameter values.
    Analyse the model again, expecting that the calculation errors related to these constants have been solved.

        kChannelParameters->variable("E_K")->setInitialValue(-85);
        kChannelParameters->variable("g_K")->setInitialValue(36);
        nGateParameters->variable("n")->setInitialValue(0.325);

        analyser->analyseModel(model);
        printIssues(analyser);

Step 8: Connect the input variables
-----------------------------------
Looking at the variables listed in the issues above we can see that some of our "external" or "input" variables are listed more than once.
These are the voltage, V, and time, t.
Time is needed in every equations component, including the imported gate component.
Voltage is needed by the potassium channel and nGate equations components.

.. container:: dothis

    **8.a** Print the model to the terminal and notice the components which contain V and t variables.  
        printModel(model);

Connections between the variables in any two components are only possible when those components are in a sibling-sibling or parent-child relationship.
We can see from the printed structure that the top-level potassium channel component doesn't have any variables, and neither does the nGate component.
We'll need to create intermediate variables in those components to allow connections to be made through them.  

.. container:: dothis

    **8.b** Create dummy variables for time and voltage using the cloning technique described in Step 7.b, and add a clone to each appropriate component.

        kChannel->addVariable(kChannelEquations->variable("t")->clone());
        kChannel->addVariable(kChannelEquations->variable("V")->clone());
        nGate->addVariable(kChannelEquations->variable("t")->clone());
        nGate->addVariable(kChannelEquations->variable("V")->clone());
        kChannelParameters->addVariable(kChannelEquations->variable("V")->clone());

.. container:: dothis

    **8.c** Connect these variables to their counterparts as needed.

        libcellml::Variable::addEquivalence(nGate->variable("t"), nGateEquations->variable("t"));
        libcellml::Variable::addEquivalence(nGate->variable("V"), nGateEquations->variable("V"));
        libcellml::Variable::addEquivalence(nGate->variable("t"), kChannelEquations->variable("t"));
        libcellml::Variable::addEquivalence(nGate->variable("V"), kChannelEquations->variable("V"));
        libcellml::Variable::addEquivalence(kChannel->variable("t"), kChannelEquations->variable("t"));
        libcellml::Variable::addEquivalence(kChannel->variable("V"), kChannelEquations->variable("V"));
        libcellml::Variable::addEquivalence(kChannelParameters->variable("V"), kChannelEquations->variable("V"));

.. container:: dothis

    **8.d** Fix the variable interfaces and validate the model, expecting no errors.

        model->fixVariableInterfaces();

        validator->validateModel(model);
        printIssues(validator);

.. container:: dothis

    **8.e** Analyse the model and expect that errors related to voltage and time now occur only in the top-level potassium channel component.
    Because this needs to be connected to the imported controller component, they'll be addressed later in Step 10.

        analyser->analyseModel(model);
        printIssues(analyser);

Step 9: Connect the calculated variables
----------------------------------------
Now we need to make sure that all of the calculated variables can move through the model properly.
In this example, the only calculated variable is n, the gate status.
This is calculated by solving the ODE in the nGate equations component, but needs to be initialised by the nGate parameters component, and its value passed back to the potassium channel equations component. 

.. container:: dothis

    **9.a** Make the required variable connections as described above.

        libcellml::Variable::addEquivalence(nGateParameters->variable("n"), nGateEquations->variable("n"));
        libcellml::Variable::addEquivalence(kChannelEquations->variable("n"), nGate->variable("n"));
        libcellml::Variable::addEquivalence(nGate->variable("n"), nGateEquations->variable("n"));

.. container:: dothis

    **9.b** Fix the variable interfaces for the model, and validate, expecting no errors.

        model->fixVariableInterfaces();

        validator->validateModel(model);
        printIssues(validator);

.. container:: dothis

    **9.c** Analyse the model, expecting that the errors related to the n variable have been resolved.

        analyser->analyseModel(model);
        printIssues(analyser);


Step 10: Connect to imported components
---------------------------------------
At this point, we have made all the connections we can between existing variables and components.
(You can verify this for yourself by printing your model to the terminal again if you like.) 
Now the problem we have is that we need to connect to variables inside imported components, but these don't exist in our model yet: the import sources that we created in Steps 4 and 5 are simply a recipe; they don't actually create anything here.

        printModel(model);

In order to connect to variables in imported components, we can create dummy variables inside them.
These will be overwritten when the imports are resolved and the model flattened, at which time the imported variables will replace the dummy ones.
As with other steps, we have a choice here.
We can manually create variables or clone existing ones into the destination components we have already created; or we can make use of the :code:`Importer` class to help us manage these.
We're going to do the latter now.

.. container:: dothis

    **10.a** Create an :code:`Importer` item.

        auto importer = libcellml::Importer::create();

Resolving the imports in a model triggers the importer to go searching for all of the information required by this model's imports, even through multiple generations of import layers.
It also instantiates each of those requirements into its own library.
You could use the :code:`Model::hasUnresolvedImports()` function to test whether the operation was successful or not; expecting it to be true before resolution, and false afterwards.

.. container:: dothis

    **10.b** Pass the model and the path to the GateModel.cellml file into the :code:`Importer::resolveImports` function. 

        importer->resolveImports(model, "");

The Importer class contains a logger (just like the Validator), so needs to be checked for issues.

.. container:: dothis

    **10.c** Check for issues and print any found to the terminal - we do not expect any at this stage.

        printIssues(importer);

The models which have the source components that we wanted to reuse from the GateModel.cellml and PotassiumChannelController.cellml files are now available to us in two ways:
- through the :code:`model()` function of the destination component's :code:`ImportSource` item; or
- as an item in the importer's library.  
  The library items can be retrieved either by index or by key, where the key is the name of the file that was resolved.

.. container:: dothis

    **10.d** Iterate through the items in the library (:code:`Importer::libraryCount()` will give you the total), and print its keys to the terminal.
    The keys can be retrieved as a string from the :code:`Importer::key(index)` function.  
    This should contain one model.

        std::cout << "The importer has " << importer->libraryCount() << " models in the library." << std::endl;
        for(size_t i = 0; i < importer->libraryCount(); ++i) {
            std::cout << " library("<<i<<") = " << importer->key(i) << std::endl;
        }
        std::cout << std::endl;

.. container:: dothis

    **10.e** We can simply use a clone of the imported components to define dummy variables in the destination component.
    Create dummy components from the resolved imported components.
    You can get these from the library or from the import source's model (or one of each, to prove to yourself that it works either way!).

        auto dummyGate = importedGate->importSource()->model()->component(importedGate->importReference())->clone();
        auto dummyController = importer->library("PotassiumChannelController.cellml")->component(controller->importReference())->clone();

.. container:: gotcha

    **GOTCHA:** Note that when an item is added to a new parent, it is automatically removed from its original parent.  
    Iterating through a set of children is best done in descending index order or using a while loop so that child items are not skipped as the indices change.

.. container:: dothis

    **10.f** Iterate through the variables in each dummy component, and add a clone of each variable to the destination component.     
               
        while(dummyGate->variableCount()) {
            importedGate->addVariable(dummyGate->variable(0));
        }
        while(dummyController->variableCount()) {
            controller->addVariable(dummyController->variable(0));
        }

.. container:: dothis

    **10.g** Connect all the variables in the nGate equations component to the dummy variables in the imported gate component.
    These connections should be:
    - (nGate equations component : imported gate component)
        - n : X
        - alpha_n : alpha_X
        - beta_n : beta_X
        - t : t
    Repeat for the controller component and the potassium channel component.
    Fix the variable interfaces and validate the model, expecting there to be no errors.

        libcellml::Variable::addEquivalence(nGateEquations->variable("n"), importedGate->variable("X"));
        libcellml::Variable::addEquivalence(nGateEquations->variable("alpha_n"), importedGate->variable("alpha_X"));
        libcellml::Variable::addEquivalence(nGateEquations->variable("beta_n"), importedGate->variable("beta_X"));
        libcellml::Variable::addEquivalence(nGateEquations->variable("t"), importedGate->variable("t"));
        libcellml::Variable::addEquivalence(controller->variable("t"), kChannel->variable("t"));
        libcellml::Variable::addEquivalence(controller->variable("V"), kChannel->variable("V"));

.. container:: dothis

    **10.h** Make sure that the output variable from this component - the potassium current - is available at the top level, and with a public and private interface.
    You'll need to create a dummy variable in the potassium channel component and link it appropriately.

        kChannel->addVariable(kChannelEquations->variable("i_K")->clone());
        kChannel->variable("i_K")->setInterfaceType("public_and_private");
        libcellml::Variable::addEquivalence(kChannelEquations->variable("i_K"), kChannel->variable("i_K"));

        model->fixVariableInterfaces();

        validator->validateModel(model);
        printIssues(validator);

The :code:`Analyser` class can only operate on a flat (ie: import-free) model.
In order to do the final check before serialising our model for output, we will use the importer to create a flattened version of the model to submit for analysis.

.. container:: dothis

    **10.i** Create a flat version of the model and submit it for analysis.  Print the errors

        auto flatModel = importer->flattenModel(model);
        analyser->analyseModel(flatModel);
        printIssues(analyser);

.. container:: nb

    Note that at this point an analysis of the unflattened model will still show errors, but that's totally fine.

Step 11: Output the model
-------------------------

.. container:: dothis

    **11.a** Create a :code:`Printer` instance and use it to serialise the model.
    This creates a string containing the CellML-formatted version of the model.
    Write this to a file called "PotassiumChannelModel.cellml".

        auto printer = libcellml::Printer::create();
        std::ofstream outFile("PotassiumChannelModel.cellml");
        outFile << printer->printModel(model);
        outFile.close();
