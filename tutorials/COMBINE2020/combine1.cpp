/**
 * COMBINE2020 TUTORIAL 1: Creating a potassium channel model
 *
 *  By the time you have worked through this tutorial you will be able to:
 *      - Assemble a multi-component model using the API; 
 *      - Import items from existing models for reuse here;
 *      - Inter-connect the components using the equivalent variables
 *        functionality;
 *      - Use the diagnostic Validator class to identify errors in the
 *        model's syntax; 
 *      - Use the diagnostic Analyser class to identify errors in the 
 *        model's mathematical formulation; and
 *      - Serialise the model to CellML format for output.
 */

#include <iostream>
#include <fstream>

#include <libcellml>

#include "tutorial_utilities.h"

int main()
{
    //  Setup useful things.
    std::string mathHeader = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\">\n";
    std::string mathFooter = "</math>";


    // Overall the model structure will be:
    //          model
    //              component: potassiumChannel
    //                  component: nGate 
    //                      component: genericGate <-- imported from GateModel.cellml
    //                  component: 

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 1: Define the potassiumChannel component " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 1: The first step is to create a model instance that will contain the potassium
    //         channel component.  Later, this will be combined with other channels and
    //         components to form the HH model.

    //  1.a Create a Model and name it appropriately.
    auto model = libcellml::Model::create("PotassiumChannelModel");

    //  1.b Create a Component instance and name it appropriately.  
    //      Add the component to the model you created above.
    auto potassiumChannel = libcellml::Component::create("potassiumChannel");
    model->addComponent(potassiumChannel);

    //  1.c Define the maths inside the potassiumChannel component.  This is a MathML 2 string
    //      (NB: higher versions of MathML are not supported), and is added to the component using
    //      setMath() and appendMath() functions.  
    //      Your string needs to contain the namespaces for MathML and for CellML: these have been
    //      provided for you in the mathHeader string above.
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

    potassiumChannel->setMath(mathHeader);
    potassiumChannel->appendMath(equationIK);
    potassiumChannel->appendMath(mathFooter);
     

    //  1.d Once the mathematics has been added to the component, and the component to the 
    //      model, we can make use of the diagnostic messages within the Validator class
    //      to tell us what else needs to be done.  
    //      Create a Validator instance, and pass it your model for processing using the 
    //      validateModel function.  
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);

    //  1.e Calling the validator does not return anything: we have to go looking for issues 
    //      that it found during processing.  When a problem is found, an Issue item is created
    //      containing:
    //          - a description string explaining the problem;
    //          - a URL at which more information is available;
    //          - an std::any item relevant to the problem, if available;
    //          - a level indicator; and
    //          - a cause indicator relevant to the stored item.
    //  We can use these issues as we need to.  The simplest way is to print the descriptions
    //  to the terminal.
    //  Retrieve the number of issues encountered using the validator->issueCount() function,
    //  then retrieve the issue items from the validator using their index and the validator->issue(index)
    //  function.
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  1.f Create the variables needed and add them to the potassium channel component.
    //      Revalidate and expect errors related to variables without units.
    potassiumChannel->addVariable(libcellml::Variable::create("E_K"));
    potassiumChannel->addVariable(libcellml::Variable::create("i_K"));
    potassiumChannel->addVariable(libcellml::Variable::create("g_K"));
    potassiumChannel->addVariable(libcellml::Variable::create("V"));
    potassiumChannel->addVariable(libcellml::Variable::create("t"));
    potassiumChannel->addVariable(libcellml::Variable::create("n"));

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  1.f Create the missing Units items and add them to the model. These are:
    //      - milli-volts
    //      - milli-seconds
    //      - milli-moles
    //      - micro-Amperes per square centimetre
    //      - milli-Siemens per square centimetre

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

    //  1.g Set the units on each of the variables.  
    //      Call the validator again, and expect there to be no errors.
    potassiumChannel->variable("E_K")->setUnits(mV);
    potassiumChannel->variable("i_K")->setUnits(microA_per_cm2);
    potassiumChannel->variable("g_K")->setUnits(mS_per_cm2);
    potassiumChannel->variable("V")->setUnits(mV);
    potassiumChannel->variable("t")->setUnits(ms);
    potassiumChannel->variable("n")->setUnits("dimensionless");

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Create the nGate component  " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  STEP 2: Create the nGate component.  The nGate is a generic gate (which we'll import from
    //          GateModel.cellml), but with parameters specific to a potassium nGate.  In addition, 
    //          instead of constant values for alpha and beta, we'll introduce a voltage dependence.

    //  2.a Create an nGate component, and add it to the potassiumChannel component (NB: not the model!).
    //      Adding it to the component creates an encapsulation hierarchy, which affects which components
    //      have access to each other.  It also means that if the potassiumChannel component is
    //      moved or imported somewhere else, then the nGate child component will be included too.
    auto nGate = libcellml::Component::create("nGate");
    potassiumChannel->addComponent(nGate);

    //  2.b Add the mathematics to the nGate component and validate the model.
    //      Expect errors relating to missing variables.
    
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

    nGate->setMath(mathHeader);
    nGate->appendMath(equationAlphaN);
    nGate->appendMath(equationBetaN);
    nGate->appendMath(mathFooter);
    
    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  2.c Add the missing variables to the nGate component, and validate again.
    //      Expect errors relating to units missing from the variables.
    nGate->addVariable(libcellml::Variable::create("t"));
    nGate->addVariable(libcellml::Variable::create("V"));
    nGate->addVariable(libcellml::Variable::create("alpha_n"));
    nGate->addVariable(libcellml::Variable::create("beta_n"));
    nGate->addVariable(libcellml::Variable::create("n"));
    
    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  2.d Create the missing units and add them to the model.  The only two which aren't available
    //      are:
    //      - per millisecond
    //      - per millivolt millisecond
    auto per_ms = libcellml::Units::create("per_ms");
    per_ms->addUnit("second", "milli", -1);
    model->addUnits(per_ms);

    auto per_mV_ms = libcellml::Units::create("per_mV_ms");
    per_mV_ms->addUnit("second", "milli", -1);
    per_mV_ms->addUnit("volt", "milli", -1);
    model->addUnits(per_mV_ms);

    //  2.e Associate the correct units items with the variables which need them.
    //      Revalidate the model, expecting there to be no errors reported.
    nGate->variable("t")->setUnits(ms);
    nGate->variable("V")->setUnits(mV);
    nGate->variable("alpha_n")->setUnits(per_ms);
    nGate->variable("beta_n")->setUnits(per_ms);
    nGate->variable("n")->setUnits("dimensionless");

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Import the generic gate component " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 3: Import the generic gate component.
    //      The generic gate model (in GateModel.cellml) has two components: 
    //          - "gate" which solves an ODE for the gate status parameter, X
    //          - "parameters" which sets the values of alpha, beta, and initialises X
    //      We will import the gate component from GateModel.cellml and connect it 
    //      to the nGate component.  This connection means we can introduce the voltage
    //      dependence for the alpha and beta, and using a specified initial value for 
    //      the gate's status.  Note that the variable 'n' in the nGate is equivalent to the
    //      generic gate's variable 'X'.

    //  Imports require three things:
    //      - a destination for the imported item. This could be a Component or Units item.
    //      - a model to import for the imported item from.  This is an ImportSource item 
    //        containing the URL of the model to read.
    //      - an import reference.  This is the name of the item to be imported from the 
    //        import source, and is associated with the destination item.

    //  3.a Create an ImportSource item and set its URL to be "GateModel.cellml".
    auto gateModelImportSource = libcellml::ImportSource::create();
    gateModelImportSource->setUrl("GateModel.cellml");

    //  3.b Create a destination component for the imported gate component, and add this to 
    //      the nGate component. 
    auto gate = libcellml::Component::create("gate");
    nGate->addComponent(gate);

    //  3.c Set the import reference on the component you just created to be the name
    //      of the component in the GateModel.cellml file that you want to use.  In this
    //      example, it is "gate".
    gate->setImportReference("gate");

    //  3.d Associate the import source with the component using the setImportSource function.
    gate->setImportSource(gateModelImportSource);

    // Note that we are deliberately not importing the second component in the GateModel.cellml
    // file, since we will be setting our own values of its variables.
    // Our next step is to connect the imported component to the nGate component, but there's a 
    // catch.  Before we connect variables to one another, they need to exist first ... but 
    // the imported component does not contain any variables (yet).  We thus need to create
    // a set of dummy variables inside the imported gate component so that we can connect them to
    // those in the nGate component.

    //  3.e
    auto importer = libcellml::Importer::create();
    importer->resolveImports(model, "");

    std::cout << "The importer has found " << importer->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < importer->issueCount(); ++i) {
        std::cout << importer->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "The importer has " << importer->libraryCount() << " models in the library." << std::endl;
    for(size_t i = 0; i < importer->libraryCount(); ++i){
        std::cout << " library("<<i<<") = " << importer->key(i) << std::endl;
    }
        std::cout << std::endl;

    // We can simply use a clone of the imported component to define dummy variables in the 
    // destination component.
    // GOTCHA: Note that when one item is added to another, it is removed from its original parent.
    //         Iterating through a set is best done in descending index order so that variables are 
    //         not missed, or using a while loop.

    // Take items from the import library to make dummy variables in the gate component.
    auto dummy = importer->library("GateModel.cellml")->component("gate")->clone();
    while(dummy->variableCount()) {
        gate->addVariable(dummy->variable(0));
    }

    // Connect all the variables in the nGate component to the dummy variables in the gate component.
    // These connections should be:
    // (nGate component : gate component)
    //  - n : X
    //  - alpha_n : alpha_X
    //  - beta_n : beta_X
    //  - t : t
    // Revalidate the model, expecting errors related to the interface types required on nGate variables.
    libcellml::Variable::addEquivalence(nGate->variable("n"), gate->variable("X"));
    libcellml::Variable::addEquivalence(nGate->variable("alpha_n"), gate->variable("alpha_X"));
    libcellml::Variable::addEquivalence(nGate->variable("beta_n"), gate->variable("beta_X"));
    libcellml::Variable::addEquivalence(nGate->variable("t"), gate->variable("t"));

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    // Fix the variable interface types according to the validator's suggestions.  Revalidate and 
    // expect there to be no errors.
    nGate->variable("n")->setInterfaceType("private");
    nGate->variable("t")->setInterfaceType("private");
    nGate->variable("alpha_n")->setInterfaceType("private");
    nGate->variable("beta_n")->setInterfaceType("private");

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 4: Connect the components together " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  STEP 4: Connect the components together.
    //      In order for the voltage dependence of the alpha and beta rates within
    //      the nGate component to affect the current in the potassiumChannel component
    //      some of the variables need to share their values between the components.
    //      This is done using variable equivalence and interfaces.

    //  4.a Set the equivalent variable pairs between the nGate and potassiumChannel components.
    //      These are:
    //          - voltage, V
    //          - time, t
    //          - gate status, n
    //      Use the Variable::addEquivalence(VariablePtr, VariablePtr) function.

    libcellml::Variable::addEquivalence(potassiumChannel->variable("t"), nGate->variable("t"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("V"), nGate->variable("V"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("n"), nGate->variable("n"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("alpha_n"), nGate->variable("alpha_n"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("beta_n"), nGate->variable("beta_n"));

    //  4.b Validate the model.  Expect errors related to unspecified interface types and invalid connections.
    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  4.c Set the recommended interface types for all of the variables with connections using the 
    //      setInterfaceType function.
    potassiumChannel->variable("t")->setInterfaceType("public_and_private");
    potassiumChannel->variable("V")->setInterfaceType("public_and_private");
    potassiumChannel->variable("E_K")->setInterfaceType("public_and_private");
    potassiumChannel->variable("g_K")->setInterfaceType("public_and_private");
    potassiumChannel->variable("n")->setInterfaceType("public_and_private");

    nGate->variable("n")->setInterfaceType("public_and_private");
    nGate->variable("t")->setInterfaceType("public_and_private");
    nGate->variable("V")->setInterfaceType("public");
    nGate->variable("alpha_n")->setInterfaceType("public_and_private");
    nGate->variable("beta_n")->setInterfaceType("public_and_private");

    //  4.d We also need to set the interface on the i_K variable, which is the variable which 
    //      this model is used to calculate.  It needs to have a "public" interface set.
    potassiumChannel->variable("i_K")->setInterfaceType("public");

    //  4.e Revalidate the model, and check that it is now free of errors.
    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 5: Analyse the model" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 5: Analyse the model 
    //      Now that the model is valid we can use the diagnostic Analyser class to check 
    //      the mathematical formulation and identify variables which need values or initialisation.

    //  5.a Create an Analyser item and pass it the model for checking with the analyseModel function.
    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(model);

    //  5.b The analyser is similar to the validator and keeps a record of issues it encounters.
    //      Retrieve these and print to the terminal, just as you've done for the validator.
    //      Expect messages related to un-calculated variables.
    std::cout << "The analyser has found " << analyser->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        std::cout << analyser->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  The "variable not calculated" message could come from four things:
    //      - the variable has not been connected to a calculated variable in another component; or
    //      - the variable needs to be given a numerical value; or
    //      - the variable is in an equation which relies on other uncomputed variables; or
    //      - the variable needs to be included in an equation somewhere.
    //  For this example, the potassium channel needs a voltage stimulus from outside to work, 
    //  so we'll need to connect variables V and t to an external component.  In turn, the calculation of
    //  variable V enables calculation of alpha_n, beta_n, which are passed to the gate component.
    //  The gate component can then calculate X, which is returned as n.  
    //  The remaining values are constants specific to the potassium channel, and are:
    //      - E_K =
    //      - g_K = 
    //  
    //  Finally, the potassium current i_K can be calculated and returned.

    //  5.c Create a potassium channel parameters component as a sibling of the potassium channel.
    //      We will use this to store parameters which are specific to a potassium channel:
    //      constants E_K, g_K, initial values of alpha_n, beta_n.
    //      Since these variables already exist in our potassium channel component, we can simply
    //      clone those ones to add to this new component.  This saves having to redefine the 
    //      names, units, and interfaces for each of them.

    auto potassiumChannelParameters = libcellml::Component::create("potassiumChannelParameters");
    model->addComponent(potassiumChannelParameters);

    auto E_K = potassiumChannel->variable("E_K")->clone();
    E_K->setInitialValue(999);
    potassiumChannelParameters->addVariable(E_K);

    auto g_K = potassiumChannel->variable("g_K")->clone();
    g_K->setInitialValue(999);
    potassiumChannelParameters->addVariable(g_K);

    auto n = potassiumChannel->variable("n")->clone();
    n->setInitialValue(999);
    potassiumChannelParameters->addVariable(n);

    //  5.d Add variable equivalence connections between the new variables and their appropriate
    //      partners in the potassiumChannel component.
    //      Validate and analyse the model.
    libcellml::Variable::addEquivalence(potassiumChannelParameters->variable("E_K"), potassiumChannel->variable("E_K"));
    libcellml::Variable::addEquivalence(potassiumChannelParameters->variable("g_K"), potassiumChannel->variable("g_K"));
    libcellml::Variable::addEquivalence(potassiumChannelParameters->variable("n"), potassiumChannel->variable("n"));

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "The analyser has found " << analyser->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        std::cout << analyser->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    // TODO KRM

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 6: Serialise and output the model" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  6.a Create a Printer instance and use it to serialise the model.  This creates a string
    //      containing the CellML-formatted version of the model.  Write this to a *.cellml file.
    auto printer = libcellml::Printer::create();
    std::ofstream outFile("PotassiumChannelModel.cellml");
    outFile << printer->printModel(model);
    outFile.close();

    std::cout << "The created '" << model->name()
              << "' model has been output to PotassiumChannelModel.cellml" << std::endl;
}
