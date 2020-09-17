/**
 * Final step of the COMBINE 2020 workshop.
 * 
 *  Goal: To create and generate code for a model representing the HH model,
 *        by using imports to combine the various components it needs from 
 *        models created during this workshop.
 */

#include <fstream>
#include <iostream>

#include <libcellml>

#include "tutorial_utilities.h"

int main() {

    // STEP 1: Create a model and import components.
    auto model = libcellml::Model::create("HodgkinHuxleyModel");

    // Membrane component import.
    auto membraneImport = libcellml::ImportSource::create();
    membraneImport->setUrl("MembraneModel.cellml");

    auto membrane = libcellml::Component::create("membrane");
    membrane->setImportSource(membraneImport);
    membrane->setImportReference("membrane");

    model->addComponent(membrane);

    // Controller component import.
    auto controllerImport = libcellml::ImportSource::create();
    controllerImport->setUrl("Controller.cellml");

    auto controller = libcellml::Component::create("controller");
    controller->setImportSource(controllerImport);
    controller->setImportReference("controller");

    model->addComponent(controller);

    // Parameters component import.
    auto parametersImport = libcellml::ImportSource::create();
    parametersImport->setUrl("Parameters.cellml");

    auto parameters = libcellml::Component::create("parameters");
    parameters->setImportSource(parametersImport);
    parameters->setImportReference("parameters");

    model->addComponent(parameters);

    // STEP 2: Connect variables between the components.

    // Have to create dummy variables in the components before they can be connected to the parameters.

    parameters->addVariable(libcellml::Variable::create("V"));
    parameters->addVariable(libcellml::Variable::create("Cm"));
    parameters->addVariable(libcellml::Variable::create("h"));
    parameters->addVariable(libcellml::Variable::create("m"));
    parameters->addVariable(libcellml::Variable::create("E_Na"));
    parameters->addVariable(libcellml::Variable::create("g_Na"));
    parameters->addVariable(libcellml::Variable::create("n"));
    parameters->addVariable(libcellml::Variable::create("E_K"));
    parameters->addVariable(libcellml::Variable::create("g_K"));
    parameters->addVariable(libcellml::Variable::create("E_L"));
    parameters->addVariable(libcellml::Variable::create("g_L"));

    membrane->addVariable(libcellml::Variable::create("V"));
    membrane->addVariable(libcellml::Variable::create("Cm"));
    membrane->addVariable(libcellml::Variable::create("h"));
    membrane->addVariable(libcellml::Variable::create("m"));
    membrane->addVariable(libcellml::Variable::create("E_Na"));
    membrane->addVariable(libcellml::Variable::create("g_Na"));
    membrane->addVariable(libcellml::Variable::create("n"));
    membrane->addVariable(libcellml::Variable::create("E_K"));
    membrane->addVariable(libcellml::Variable::create("g_K"));
    membrane->addVariable(libcellml::Variable::create("E_L"));
    membrane->addVariable(libcellml::Variable::create("g_L"));
    membrane->addVariable(libcellml::Variable::create("i_stim"));

    controller->addVariable(libcellml::Variable::create("i_stim"));
    controller->addVariable(libcellml::Variable::create("t"));


    auto validator = libcellml::Validator::create();
    validator->validateModel(model);
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i){
        std::cout << validator->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    printModelToTerminal(model);

    //  2.a Connect the controller's output (i_stim) to the membrane.
    libcellml::Variable::addEquivalence(controller->variable("i_stim"), membrane->variable("i_stim"));

    //  2.b Connect the parameter component's values to the membrane.
    libcellml::Variable::addEquivalence(controller->variable("t"), membrane->variable("t"));
    libcellml::Variable::addEquivalence(parameters->variable("V"), membrane->variable("V"));
    libcellml::Variable::addEquivalence(parameters->variable("Cm"), membrane->variable("Cm"));
    libcellml::Variable::addEquivalence(parameters->variable("h"), membrane->variable("h"));
    libcellml::Variable::addEquivalence(parameters->variable("m"), membrane->variable("m"));
    libcellml::Variable::addEquivalence(parameters->variable("n"), membrane->variable("n"));
    libcellml::Variable::addEquivalence(parameters->variable("g_K"), membrane->variable("g_K"));
    libcellml::Variable::addEquivalence(parameters->variable("g_Na"), membrane->variable("g_Na"));
    libcellml::Variable::addEquivalence(parameters->variable("g_L"), membrane->variable("g_L"));
    libcellml::Variable::addEquivalence(parameters->variable("E_K"), membrane->variable("E_K"));
    libcellml::Variable::addEquivalence(parameters->variable("E_Na"), membrane->variable("E_Na"));
    libcellml::Variable::addEquivalence(parameters->variable("E_L"), membrane->variable("E_L"));

    // STEP 3: Output model with imports.
    auto printer = libcellml::Printer::create();
    std::ofstream outFile("HodgkinHuxleyModel.cellml");
    outFile << printer->printModel(model);
    outFile.close();

}