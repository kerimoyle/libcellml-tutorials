/**
 * COMBINE2020 libCellML Tutorial 1: Creating a generic gate model
 *
 *  By the time you have worked through this tutorial you will be able to:
 *      - Assemble a model using the API; 
 *      - Use the diagnostic Validator class to identify errors in the 
 *        model's syntax; 
 *      - Use the diagnostic Analyser class to identify errors in the 
 *        model's mathematical construction; and
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

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 1: Setup the model  " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 1

    //  1.a 
    //      The first step is to create a Model item which will later contain the component and 
    //      the units it needs.  
    auto model = libcellml::Model::create();

    //  1.b 
    //      Each CellML element must have a name, which is set using the setName() function.
    model->setName("GateModel");

    //  1.c 
    //       We'll create a wrapper component whose only job is to encapsulate the other components.
    //      This makes is a lot easier for this model to be reused, as the connections between
    //      components internal to this one won't need to be re-established.
    //      Note that the constructor for all named CellML entities is overloaded, so 
    //      you can pass it the name string at the time of creation.
    //      Create a component named "gate".
    auto gate = libcellml::Component::create("gate");

    //  1.d Finally we need to add the component to the model.  This sets it at the top-level of 
    //      the components' encapsulation heirarchy.  All other components need to be added 
    //      to this component, rather than the model.
    //      Add the component to the model using the Model::addComponent() function.
    model->addComponent(gate);

    // end 1

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Create the gateEquations component             " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 2

    //  2.a 
    //  Create a gateEquations component, name it "gateEquations" and add it to the model.
    auto gateEquations = libcellml::Component::create("gateEquations");

    //  2.b 
    //  Add the new gateEquations component to the gate component.
    gate->addComponent(gateEquations);

    //  2.c 
    //  Add the mathematics to the gateEquations component.
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

    gateEquations->setMath(mathHeader);
    gateEquations->appendMath(equation);
    gateEquations->appendMath(mathFooter);

    // end 2

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Validate the model                            " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  Once the mathematics has been added to the component, and the component to the 
    //  model, we can make use of the diagnostic messages within the Validator class
    //  to tell us what else needs to be done.  

    //  3.a 
    //      Create a Validator instance, and pass it your model for processing using the 
    //      validateModel function.  
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);

    // end 3.a

    //  Calling the validator does not return anything: we have to go looking for issues 
    //  that it found during processing.  When a problem is found, an Issue item is created
    //  containing:
    //      - a description string explaining the problem;
    //      - a URL at which more information is available;
    //      - an std::any item relevant to the problem, if available;
    //      - a level indicator; and
    //      - a cause indicator relevant to the stored item.
    //  We can use these issues as we need to.  The simplest way is to print the descriptions
    //  to the terminal.

    //  Two helper functions have been provided for this tutorial that will help printing 
    //  enumerated values to the terminal.  These are:
    //      - getIssueLevelFromEnum; and
    //      - getItemTypeAsString. 

    //  3.b 
    //      Retrieve the number of issues encountered using the validator->issueCount() function,
    //      then retrieve the issue items from the validator using their index and the validator->issue(index)
    //      function.  Print the information from each issue to the terminal.
    std::cout << "The validator has found " << validator->issueCount() << " issues." << std::endl;
    for (size_t i = 0; i < validator->issueCount(); ++i) {
        auto issue = validator->issue(i);
        auto ref = issue->referenceHeading();
        std::cout << "Issue [" << i << "] is " << getIssueLevelFromEnum(issue->level()) << ":" << std::endl;
        std::cout << "    description: " << issue->description() << std::endl;
        if (ref != "") {
            std::cout << "    see section " << ref
                        << " in the CellML specification." << std::endl;
        }
        std::cout << "    stored item type: " << getItemTypeAsString(issue->itemType()) << std::endl;
    }

    //  3.c 
    //      Add the missing variables to the gateEquations component, and validate again.
    //      Expect errors relating to missing units.
    //      Note that you can use the helper function printIssues(validator) to print your
    //      issues to the screen instead of repeating the code from 3.b.
    gateEquations->addVariable(libcellml::Variable::create("t"));
    gateEquations->addVariable(libcellml::Variable::create("alpha_X"));
    gateEquations->addVariable(libcellml::Variable::create("beta_X"));
    gateEquations->addVariable(libcellml::Variable::create("X"));

    validator->validateModel(model);
    printIssues(validator);

    //  3.d 
    //      Create the units which will be needed by your variables and add them to the model.
    //      Use the setUnits function to associate them with the appropriate variables.  
    //      Validate again, and expect no errors.
    auto ms = libcellml::Units::create("ms");
    ms->addUnit("second", "milli");
    model->addUnits(ms);

    auto per_ms = libcellml::Units::create("per_ms");
    per_ms->addUnit("second", "milli", -1);
    model->addUnits(per_ms);

    gateEquations->variable("t")->setUnits(ms);
    gateEquations->variable("alpha_X")->setUnits(per_ms);
    gateEquations->variable("beta_X")->setUnits(per_ms);
    gateEquations->variable("X")->setUnits("dimensionless");

    validator->validateModel(model);
    printIssues(validator);

    //  end 3

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 4: Analyse the model  " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  4.a 
    //      Create an Analyser item and submit the model for processing. 
    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(model);

    //  4.b 
    //      Just like the Validator class, the Analyser class keeps track of issues. 
    //      Retrieve these and print to the terminal. Expect errors related to 
    //      un-computed variables and missing initial values.
    printIssues(analyser);

    //  end 4.b
    //  In order to avoid hard-coding values here, we will need to connect to external 
    //  values to initialise the X variable and provide the value for alpha_X and beta_X.
    //  This means that:
    //      - we need to create an external component to hold variable values;
    //      - we need to create external variables in that component; 
    //      - we need to specify the connections between variables; and
    //      - we need to permit external connections on the variables.
    
    //  4.c 
    //      Create a component which will store the hard-coded values for initialisation.
    //      Name it "gateParameters", and add it to the top-level gate component as a sibling
    //      of the gateEquations component.
    auto gateParameters = libcellml::Component::create("gateParameters");
    gate->addComponent(gateParameters);

    //  4.d 
    //      Create appropriate variables in this component, and set their units.
    //      Use the setInitialValue function to initialise them.
    auto X = libcellml::Variable::create("X");
    X->setUnits("dimensionless");
    X->setInitialValue(0);
    gateParameters->addVariable(X);

    auto alpha = libcellml::Variable::create("alpha");
    alpha->setUnits(per_ms);
    alpha->setInitialValue(0.1);
    gateParameters->addVariable(alpha);

    auto beta = libcellml::Variable::create("beta");
    beta->setUnits(per_ms);
    beta->setInitialValue(0.5);
    gateParameters->addVariable(beta);
    
    //  4.e 
    //      Specify a variable equivalence between the gateEquations variables and the parameter variables.
    //      Validate the model again, expecting errors related to the variable interface types.
    libcellml::Variable::addEquivalence(gateEquations->variable("X"), gateParameters->variable("X"));
    libcellml::Variable::addEquivalence(gateEquations->variable("alpha_X"), gateParameters->variable("alpha"));
    libcellml::Variable::addEquivalence(gateEquations->variable("beta_X"), gateParameters->variable("beta"));

    validator->validateModel(model);
    printIssues(validator);

    //  4.f 
    //      Set the variable interface type according to the recommendation from the validator.
    //      This can either be done individually using the Variable::setInterfaceType() function, or 
    //      en masse for all the model's interfaces using the Model::fixVariableInterfaces() function.
    //      Validate and analyse again, expecting no errors. 
    model->fixVariableInterfaces();

    validator->validateModel(model);
    printIssues(validator);

    analyser->analyseModel(model);
    printIssues(analyser);

    //  end 4.f

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 5: Sanity check" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  5.a 
    //      Print the model to the terminal using the helper function printModel.
    printModel(model);

    // end 5.a
    //      Looking at the printout we see that the top-level component has no variables.  
    //      Even though this is clearly a valid situation (as proved by 4.f), it's not
    //      going to make this model easy to reuse.  We need to make sure that any input and
    //      output variables are also connected into the top level gate component.  
    //
    //      Create intermediate variables for time t and gate status X in the gate component,
    //      and ensure they have a public and private interface to enable two-way connection.
    //      You may also need to set a public and private connection onto t and X in the
    //      equations component too.
    //  5.b
    gate->addVariable(gateEquations->variable("t")->clone());
    gate->addVariable(gateEquations->variable("X")->clone());

    gate->variable("t")->setInterfaceType("public_and_private");
    gate->variable("X")->setInterfaceType("public_and_private");
    gateEquations->variable("t")->setInterfaceType("public_and_private");
    gateEquations->variable("X")->setInterfaceType("public_and_private");
    
    //  5.c 
    //      Connect the intermediate variables to their respective partners in the equations
    //      component, and recheck the model.
    libcellml::Variable::addEquivalence(gate->variable("t"), gateEquations->variable("t"));
    libcellml::Variable::addEquivalence(gate->variable("X"), gateEquations->variable("X"));

    validator->validateModel(model);
    printIssues(validator);
    analyser->analyseModel(model);
    printIssues(analyser);

    //  end 5
    
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 6: Serialise and output the model" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  6.a 
    //      Create a Printer instance and use it to serialise the model.  This creates a string
    //      containing the CellML-formatted version of the model.  Write this to a file called
    //     "GateModel.cellml".
    auto printer = libcellml::Printer::create();
    std::ofstream outFile("GateModel.cellml");
    outFile << printer->printModel(model);
    outFile.close();

    std::cout << "The created model has been written to GateModel.cellml" << std::endl;

    //  end 6
}
