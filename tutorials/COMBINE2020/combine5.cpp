/**
 * 
 * 
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "tutorial_utilities.h"

int main() {

    // STEP 1: Parse the HodgkinHuxleyModel.cellml.

    //  1.a  Read the Hodgkin-Huxley model from the CellML file.
    std::ifstream inFile("HodgkinHuxleyModel.cellml");
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();

    //  1.b Deserialise into a model.
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(inFileContents.str());

    printModelToTerminal(model);

    //  1.c Validate the imported model.
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << validator->issue(i)->description() << std::endl;
    }

    // STEP 2: Use an Importer to manage the dependencies of the model.

    //  2.a Create the Importer instance.
    auto importer = libcellml::Importer::create();

    //  2.b Use the importer to resolve the model's import dependencies.
    importer->resolveImports(model, "");

    //  2.c Iterate through the importer's library and validate each of 
    //      the individual imported models.
    for(size_t i = 0; i < importer->libraryCount(); ++i) {
        validator->validateModel(importer->library(i));
        std::cout << "The validator found " << validator->issueCount() << " issues in " << importer->key(i) << std::endl;
        for(size_t i = 0; i < validator->issueCount(); ++i) {
            std::cout << validator->issue(i)->description() << std::endl;
        }
    }

    //  2.d Check for any issues.
    std::cout << "The importer found " << importer->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < importer->issueCount(); ++i) {
        std::cout << importer->issue(i)->description() << std::endl;
    }

    printModelToTerminal(model);

    //  2.e Create a flattened version of the model.
    auto flatModel = importer->flattenModel(model);

    std::cout << "The importer found " << importer->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < importer->issueCount(); ++i) {
        std::cout << importer->issue(i)->description() << std::endl;
    }

    // auto printer = libcellml::Printer::create();
    // std::cout << printer->printModel(flatModel) << std::endl;

    printModelToTerminal(flatModel);

    // STEP 3: Analyse the flattened model.

    //  3.a Create an Analyser item.
    auto analyser = libcellml::Analyser::create();

    //  3.b Submit the flattened model to the analyser.
    analyser->analyseModel(flatModel);

    //  3.c Check for issues.
    std::cout << "The analyser found " << analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        std::cout << analyser->issue(i)->description() << std::endl;
    }

}