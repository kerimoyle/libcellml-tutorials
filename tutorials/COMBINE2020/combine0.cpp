/**
 * COMBINE2020 TUTORIAL 0: Creating a generic gate model
 *
 *  By the time you have worked through this tutorial you will be able to:
 *      - Assemble a model using the API; 
 *      - Use the diagnostic Validator class to identify errors in
 *        the model's syntax; 
 *      - Use the diagnostic Analyser class to identify errors in the model's 
 *        mathematical construction; and
 *      - Serialise the model to CellML format for output.
 */

#include <iostream>
#include <fstream>

#include <libcellml>

int main()
{
    //  Setup useful things.
    std::string mathHeader = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\">\n";
    std::string mathFooter = "</math>";

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 1: Create the model  " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 1: Create a Model item

    //  1.a The first step is to create a Model item which will later contain the component and 
    //      the units it needs.  
    auto model = libcellml::Model::create();

    //  1.b Each CellML element must have a name, which is set using the setName() function.
    model->setName("GateModel");

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Create the gate component  " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  STEP 2: Create the gate component

    //  2.a Create a gate component, name it "gate" and add it to the model.
    //      Note that the constructor for all named CellML entities is overloaded, so 
    //      you can pass it the name string at the time of creation.

    auto gate = libcellml::Component::create("gate");

    //  2.b Add the new gate component to the model.
    model->addComponent(gate);

    //  2.c Add the mathematics to the gate component.
    std::string equation =
        "  <apply><eq/>\n"
        "    <apply><diff/>\n"
        "      <bvar><ci>t</ci></bvar>\n"
        "      <ci>X</ci>\n"
        "    </apply>\n" 
        "    <apply><minus/>\n"
        "      <apply><times/>\n"
        "        <ci>alpha_X</ci>\n"
        "        <apply><minus/>\n"
        "          <cn cellml:units=\"dimensionless\">1</cn>\n"
        "          <ci>X</ci>\n"
        "        </apply>\n" 
        "      </apply>\n" 
        "      <apply><times/>\n"
        "        <ci>beta_X</ci>\n"
        "        <ci>X</ci>\n"
        "      </apply>\n" 
        "    </apply>\n" 
        "  </apply>\n"; 

    gate->setMath(mathHeader);
    gate->appendMath(equation);
    gate->appendMath(mathFooter);

    auto validator = libcellml::Validator::create();
    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }

    //  2.c Add the missing variables to the gate component, and validate again.
    //      Expect errors relating to missing units.
    gate->addVariable(libcellml::Variable::create("t"));
    gate->addVariable(libcellml::Variable::create("alpha_X"));
    gate->addVariable(libcellml::Variable::create("beta_X"));
    gate->addVariable(libcellml::Variable::create("X"));

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }

    //  2.d Create the units which will be needed by your variables and add them to the model.
    //      Use the setUnits function to associate them with the appropriate variables.  
    //      Validate again, and expect no errors.
    auto ms = libcellml::Units::create("ms");
    ms->addUnit("second", "milli");
    model->addUnits(ms);

    auto per_ms = libcellml::Units::create("per_ms");
    per_ms->addUnit("second", "milli", -1);
    model->addUnits(per_ms);

    gate->variable("t")->setUnits(ms);
    gate->variable("alpha_X")->setUnits(per_ms);
    gate->variable("beta_X")->setUnits(per_ms);
    gate->variable("X")->setUnits("dimensionless");

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Analyse the model  " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 3: Analyse the mathematical construction of the model.

    //  3.a Create an Analyser item and submit the model for processing. 
    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(model);

    //  3.b Just like the Validator class, the Analyser class keeps track of issues. 
    //      Retrieve these and print to the terminal. Expect errors related to 
    //      un-computed variables and missing inital values.
    std::cout << "The analyser has found " << analyser->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        std::cout << analyser->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  In order to avoid hard-coding values here, we will need to connect to external 
    //  values to initialise the X variable and provide the value for alpha_X and beta_X.
    //  This means three things need to happen:
    //      - we need to create an external component to hold variable values;
    //      - we need to create external variables in that component; 
    //      - we need to specify the connections between variables; and
    //      - we need to permit external connections on the variables.
    
    //  3.c Create a component which will store the hard-coded values for initialisation.
    //      Name it "parameters", and add it to the model as a sibling of the gate component.
    auto parameters = libcellml::Component::create("parameters");
    model->addComponent(parameters);

    //  3.d Create appropriate variables in this component, and set their units.
    //      Use the setInitialValue function to initialise them.
    {
        auto X = libcellml::Variable::create("X");
        X->setUnits("dimensionless");
        X->setInitialValue(0);
        parameters->addVariable(X);

        auto alpha = libcellml::Variable::create("alpha");
        alpha->setUnits(per_ms);
        alpha->setInitialValue(0.1);
        parameters->addVariable(alpha);

        auto beta = libcellml::Variable::create("beta");
        beta->setUnits(per_ms);
        beta->setInitialValue(0.5);
        parameters->addVariable(beta);
    }
   
    //  3.e Specify a variable equivalence between the gate variables and the parameter variables.
    //      Validate the model again, expecting errors related to the variable interface types.
    libcellml::Variable::addEquivalence(gate->variable("X"), parameters->variable("X"));
    libcellml::Variable::addEquivalence(gate->variable("alpha_X"), parameters->variable("alpha"));
    libcellml::Variable::addEquivalence(gate->variable("beta_X"), parameters->variable("beta"));

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  3.f Set the variable interface type according to the recommendation from the validator.
    //      Validate and analyse again, expecting no errors. 
    gate->variable("alpha_X")->setInterfaceType("public");
    gate->variable("beta_X")->setInterfaceType("public");
    gate->variable("X")->setInterfaceType("public");
    parameters->variable("alpha")->setInterfaceType("public");
    parameters->variable("beta")->setInterfaceType("public");
    parameters->variable("X")->setInterfaceType("public");

    validator->validateModel(model);
    std::cout << "The validator has found " << validator->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    analyser->analyseModel(model);
    std::cout << "The analyser has found " << analyser->issueCount() << " errors." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        std::cout << analyser->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 4: Serialise and output the model" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  4.a Create a Printer instance and use it to serialise the model.  This creates a string
    //      containing the CellML-formatted version of the model.  Write this to a file called
    //     "GateModel.cellml".
    auto printer = libcellml::Printer::create();
    std::ofstream outFile("GateModel.cellml");
    outFile << printer->printModel(model);
    outFile.close();

    std::cout << "The created model has been written to GateModel.cellml" << std::endl;
}
