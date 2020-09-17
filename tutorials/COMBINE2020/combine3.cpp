
#include <fstream>
#include <iostream>

#include <libcellml>

#include "tutorial_utilities.h"

int main() {

    // STEP 1: Parse self-contained membrane component model.
    auto parser = libcellml::Parser::create();
    std::ifstream inFile("MembraneModel_selfContained.cellml");
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();
   
    auto model = parser->parseModel(inFileContents.str());

    // Validate.
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i){
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    printModelToTerminal(model, false);

    // STEP 2: Create the import definitions.

    auto membrane = model->component("membrane");

    // Sodium channel import.
    auto sodiumChannelImport = libcellml::ImportSource::create();
    sodiumChannelImport->setUrl("SodiumChannelModel.cellml");

    auto sodiumChannel = libcellml::Component::create("sodiumChannel");
    sodiumChannel->setImportSource(sodiumChannelImport);
    sodiumChannel->setImportReference("sodiumChannel");

    membrane->addComponent(sodiumChannel);

    // Potassium channel import.
    auto potassiumChannelImport = libcellml::ImportSource::create();
    potassiumChannelImport->setUrl("PotassiumChannelModel.cellml");

    auto potassiumChannel = libcellml::Component::create("potassiumChannel");
    potassiumChannel->setImportSource(potassiumChannelImport);
    potassiumChannel->setImportReference("potassiumChannel");

    membrane->addComponent(potassiumChannel);

    // Leakage current import.
    auto leakageImport = libcellml::ImportSource::create();
    leakageImport->setUrl("LeakageModel.cellml");

    auto leakage = libcellml::Component::create("leakage");
    leakage->setImportSource(leakageImport);
    leakage->setImportReference("leakage");

    membrane->addComponent(leakage);

    // STEP 3: Connect imports into the membrane component.

    // Dummy variables to connect the components together.
    leakage->addVariable(libcellml::Variable::create("i_L"));
    leakage->addVariable(libcellml::Variable::create("g_L"));
    leakage->addVariable(libcellml::Variable::create("E_L"));
    sodiumChannel->addVariable(libcellml::Variable::create("i_Na"));
    sodiumChannel->addVariable(libcellml::Variable::create("g_Na"));
    sodiumChannel->addVariable(libcellml::Variable::create("E_Na"));
    sodiumChannel->addVariable(libcellml::Variable::create("t"));
    sodiumChannel->addVariable(libcellml::Variable::create("h"));
    sodiumChannel->addVariable(libcellml::Variable::create("alpha_h"));
    sodiumChannel->addVariable(libcellml::Variable::create("beta_h"));
    sodiumChannel->addVariable(libcellml::Variable::create("m"));
    sodiumChannel->addVariable(libcellml::Variable::create("alpha_m"));
    sodiumChannel->addVariable(libcellml::Variable::create("beta_m"));

    potassiumChannel->addVariable(libcellml::Variable::create("i_K"));
    potassiumChannel->addVariable(libcellml::Variable::create("g_K"));
    potassiumChannel->addVariable(libcellml::Variable::create("E_K"));
    potassiumChannel->addVariable(libcellml::Variable::create("t"));
    potassiumChannel->addVariable(libcellml::Variable::create("n"));
    potassiumChannel->addVariable(libcellml::Variable::create("alpha_n"));
    potassiumChannel->addVariable(libcellml::Variable::create("beta_n"));

    // Connect the components together.
    libcellml::Variable::addEquivalence(leakage->variable("i_L"), membrane->variable("i_L"));
    libcellml::Variable::addEquivalence(leakage->variable("E_L"), membrane->variable("E_L"));
    libcellml::Variable::addEquivalence(leakage->variable("g_L"), membrane->variable("g_L"));

    libcellml::Variable::addEquivalence(sodiumChannel->variable("i_Na"), membrane->variable("i_Na"));
    libcellml::Variable::addEquivalence(sodiumChannel->variable("E_Na"), membrane->variable("E_Na"));
    libcellml::Variable::addEquivalence(sodiumChannel->variable("g_Na"), membrane->variable("g_Na"));
    libcellml::Variable::addEquivalence(sodiumChannel->variable("t"), membrane->variable("t"));
    libcellml::Variable::addEquivalence(sodiumChannel->variable("h"), membrane->variable("h"));
    libcellml::Variable::addEquivalence(sodiumChannel->variable("m"), membrane->variable("m"));
    libcellml::Variable::addEquivalence(sodiumChannel->variable("alpha_m"), membrane->variable("alpha_m"));
    libcellml::Variable::addEquivalence(sodiumChannel->variable("beta_m"), membrane->variable("beta_m"));
    libcellml::Variable::addEquivalence(sodiumChannel->variable("alpha_h"), membrane->variable("alpha_h"));
    libcellml::Variable::addEquivalence(sodiumChannel->variable("beta_h"), membrane->variable("beta_h"));

    libcellml::Variable::addEquivalence(potassiumChannel->variable("i_K"), membrane->variable("i_K"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("E_K"), membrane->variable("E_K"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("g_K"), membrane->variable("g_K"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("t"), membrane->variable("t"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("n"), membrane->variable("n"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("alpha_n"), membrane->variable("alpha_n"));
    libcellml::Variable::addEquivalence(potassiumChannel->variable("beta_n"), membrane->variable("beta_n"));
    

    // Validate
    validator->validateModel(model);
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i){
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    printModelToTerminal(model, false);

    // STEP 4: Serialise and write to a CellML file.
    auto printer = libcellml::Printer::create();
    std::ofstream outFile("MembraneModel.cellml");
    outFile << printer->printModel(model);
    outFile.close();

}