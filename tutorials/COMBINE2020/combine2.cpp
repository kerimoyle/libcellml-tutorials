/**
 * COMBINE TUTORIAL 2: Debugging the sodium channel model
 * 
 * By the time you have worked through this part of the tutorial you will be able to:
 *  - Parse an existing CellML file and deserialise it into a model instance;
 *  - Use the diagnostic Validator class to identify issues in the model's definition; 
 *  - Use the std::any_cast to retrieve items which need repair from validator Issue items; and
 *  - Use the diagnostic Analyser class to identify issues in the model's mathematical formulation.
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "tutorial_utilities.h"

int main()
{
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 1: Parse the existing sodium channel model " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 1: Parse an existing model from a CellML file.
    //         The Parser class is used to deserialise a CellML string into a Model instance.
    //         This means that you're responsible for finding, opening and reading the *.cellml 
    //         file into a single string.  The Parser will then read that string and return a model.

    //  1.a Read a CellML file into a std::string.
    std::ifstream inFile("sodiumChannelModel_broken.cellml");
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();

    //  1.b Create a Parser item. 
    auto parser = libcellml::Parser::create();

    //  1.c Use the parser to deserialise the contents of the string you've read and return the model.
    auto model = parser->parseModel(inFileContents.str());

    //  1.d The Parser class - like the other utility classes - also keeps track when things go wrong.
    //      You can check for any issues and retrieve them just as you did earlier for the Validator class.
    std::cout << "The parser found " << parser->issueCount() << " issues." << std::endl;

    //  1.e Print the parsed model to the terminal for viewing.
    printModelToTerminal(model, false);

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Validate the parsed model " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 2: Check the parsed model for errors.
    //         Just as you did in the first part of the tutorial, create a Validator item and use it
    //         to check the model you've just read.

    //  2.a Create a Validator item and validate the model.
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);

    //  2.b Retrieve any issues from the validator and print them to the terminal.
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i){
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Repair the parsed model " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 3: Fix the errors reported by the validator.

    //  3.a A group of issues reported is similar to: "Variable 't' in component 'hGate' has a 
    //      units reference 'ms' which is neither standard nor defined in the parent model."
    //      Looking at the model printout, we see that this is completely missing.  
    //      Create a Units item named "ms" and representing milliseconds, and add it to the model.
    //      Link the units and update the validation.
    auto ms = libcellml::Units::create("ms");
    ms->addUnit("second", "milli");
    model->addUnits(ms);
    model->linkUnits();
    std::cout << "Created 'ms' units" << std::endl;

    validator->validateModel(model);
        std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i){
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl; 

    //  3.b Two of the errors read:
    //      - "Variable 'V' in component 'hGate' has a units reference 'mV' which 
    //      is neither standard nor defined in the parent model."
    //      - "Math has a cn element with a cellml:units attribute 'mV' that is not a valid
    //      reference to units in the model 'SodiumChannelModel' or a standard unit."
    //      Looking at the printout of the model in step 1.e we can see that there is a
    //      units item there, but it's named "millivolt" instead of "mV".
    //      Since there seem to be several errors regarding missing "mV" units, we'll change
    //      the name of the existing Units item instead of adding a new one.
    //
    //      Retrieve the units item from the model using its current name, and set its new name.
    //      Because we're changing something to do with units, we also need to call the model->linkUnits()
    //      function to refresh their relationship.
    //      Revalidate the model to reduce the number of errors reported.
    model->units("millivolt")->setName("mV");
    model->linkUnits();
    std::cout << "Renamed 'millivolt' units to 'mV'" << std::endl;

    validator->validateModel(model);
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i){
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  3.c The first issue returned says:
    //      "Variable 'V' in component 'sodiumChannel' has a units reference 'i_dont_exist' 
    //      which is neither standard nor defined in the parent model."
    //      We have a choice.  We have enough information to locate this variable and change
    //      units, but since the Issue class also stores a pointer to that same variable, we
    //      can just retrieve it directly from there instead.
    //      Fetch the 0th issue from the validator into an Issue item.
    auto issue1 = validator->issue(0);

    //  3.d The Issue class uses a std::any type to store the item which is relevant to the
    //      issue.  Before we can fix it, we need to first cast it back into its libCellML
    //      item type.  First let's check that this issue is connected to a variable type 
    //      by checking against the ItemType enumeration returned from the issue->itemType()
    //      function.
    if(issue1->itemType() == libcellml::ItemType::VARIABLE){
        std::cout << "The 0th issue stores a Variable item." << std::endl;
    }

    //  3.e Again we have a choice.  We can use the Issue::variable() function to return
    //      a pointer to the variable with the problem, OR we can retrieve the AnyItem
    //      item and do the casting ourselves.  Here we'll go the easy way and use the 
    //      convenience function.
    //      Retrieve the variable with the missing units from the issue, and change its 
    //      units to be 'mV'.
    auto V = issue1->variable();
    V->setUnits("mV");
    model->linkUnits();
    std::cout << "Changed references to units 'i_dont_exist' into 'mV'." << std::endl;

    validator->validateModel(model);
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i){
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    //  3.f The final errors are similar to "The equivalence between 'V' in component 'sodiumChannel'
    //      and 'V' in component 'hGate' is invalid. Component 'sodiumChannel' and 'hGate' are neither
    //      siblings nor in a parent/child relationship."
    //      Looking again at the model's printout we can see that the hGate component is a child of the
    //      mGate component.  They should be siblings, both children of the sodiumChannel component.
    //      The error occurs because variables can only be connected when they are in components which are
    //      adjacent to one another: either siblings or in a parent-child relationship.  Fixing the 
    //      encapsulation structure by moving the hGate to be a child of the sodiumChannel will remove these
    //      errors.
    //      It's sufficient to simply add the hGate component to the sodiumChannel component.  libCellML
    //      will handle removing it from its previous place.  Do this, print the model to check.
    auto hGate = model->component("sodiumChannel")->component("mGate")->component("hGate");
    hGate = model->component("hGate", true);

    model->component("sodiumChannel")->addComponent(hGate);
    std::cout << "Moved the hGate component to be a child of the sodiumChannel component." << std::endl << std::endl;
    
    printModelToTerminal(model, false);

    validator->validateModel(model);
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i){
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    // We also need to fix up variable connections between the gates and the sodium channel.
    auto sodiumChannel = model->component("sodiumChannel");
    auto mGate = model->component("mGate", true);

    // Add the missing transfer variables.
    {
        auto alpha_h = libcellml::Variable::create("alpha_h");
        alpha_h->setUnits("per_ms");
        alpha_h->setInterfaceType("public_and_private");
        sodiumChannel->addVariable(alpha_h);

        auto beta_h = libcellml::Variable::create("beta_h");
        beta_h->setUnits("per_ms");
        beta_h->setInterfaceType("public_and_private");
        sodiumChannel->addVariable(beta_h);

        auto alpha_m = libcellml::Variable::create("alpha_m");
        alpha_m->setUnits("per_ms");
        alpha_m->setInterfaceType("public_and_private");
        sodiumChannel->addVariable(alpha_m);

        auto beta_m = libcellml::Variable::create("beta_m");
        beta_m->setUnits("per_ms");
        beta_m->setInterfaceType("public_and_private");
        sodiumChannel->addVariable(beta_m);
    }

    libcellml::Variable::addEquivalence(hGate->variable("h"), sodiumChannel->variable("h"));
    libcellml::Variable::addEquivalence(hGate->variable("t"), sodiumChannel->variable("t"));
    libcellml::Variable::addEquivalence(hGate->variable("alpha_h"), sodiumChannel->variable("alpha_h"));
    libcellml::Variable::addEquivalence(hGate->variable("beta_h"), sodiumChannel->variable("beta_h"));

    libcellml::Variable::addEquivalence(mGate->variable("m"), sodiumChannel->variable("m"));
    libcellml::Variable::addEquivalence(mGate->variable("t"), sodiumChannel->variable("t"));
    libcellml::Variable::addEquivalence(mGate->variable("alpha_m"), sodiumChannel->variable("alpha_m"));
    libcellml::Variable::addEquivalence(mGate->variable("beta_m"), sodiumChannel->variable("beta_m"));

    // Add in missing equivalences for the alpha, beta, g, E variables.


    printModelToTerminal(model);

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 4: Serialise and print the repaired model " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 4: Print the repaired model to a new file.
    
    //  4.a Create a Printer instance and use it to print the CellML-formatted version of
    //      the repaired model to a string.
    auto printer = libcellml::Printer::create();
    auto modelString = printer->printModel(model);

    //  4.b Write the string to a file named "SodiumChannelModel.cellml".
    std::ofstream outFile("SodiumChannelModel.cellml");
    outFile << modelString;
    outFile.close();

    std::cout << "The repaired sodium channel model has been written to SodiumChannelModel.cellml." << std::endl;
    
}
