/**
 * This program is intended to demonstrate a workflow typical of a simulation
 * tool developer who would like to be able to interact with CellML model files.
 * The general principles are:
 * - parse a CellML model file;
 * - resolve any import dependencies in the model;
 * - validate the model (check for semantic/syntactic errors);
 * - debug the model (check for modelling errors);
 * - generate runnable code in C and/or Python; and
 * - output in the desired format.
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "utilities.h"

int main()
{
    // STEP 1
    // Parse a CellML file into a model.

    // Read the file containing the CellML model into a string.
    std::string inFileName = "simulationExample.cellml";
    std::ifstream inFile(inFileName);
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();

    std::cout << "Opening the CellML file" << std::endl;

    // Create a libCellML Parser, and use it to parse the fileContents
    // string and convert it into a CellML Model structure.
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(inFileContents.str());
    printIssues(parser);

    // STEP 2
    // Resolve the import dependencies (if any) and flatten the model.

    if(model->hasUnresolvedImports()) {
        auto importer = libcellml::Importer::create();

        // Submit the model to the importer and the absolute location 
        // against which the import reference paths will be resolved.
        importer->resolveImports(model, "");

        printIssues(importer);

        // Print a list of dependencies for the current unflattened model.
        printImportDependencies(model);

        // Retrieve a "flattened" (ie: import-free) model from the importer,
        // and use it to over-write the current model.cd 
        model = importer->flattenModel(model);

        printImportDependencies(model);
    }

    // STEP 3
    // Validate the model: check for syntactic and semantic errors.

    // Create a Validator instance and pass the model for checking.
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);
    auto isValid = validator->errorCount() == 0;
    printIssues(validator);

    // STEP 4
    // Analyse the model: check for mathematical and modelling errors.
    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(model);
    printIssues(analyser);

    // STEP 5
    // Generate runnable code in other language formats for this model.

    // Create a Generator instance.  Note that by default this uses the C language profile.
    auto generator = libcellml::Generator::create();

    // Pass the generator the model for processing.
    generator->setModel(analyser->model());

    // Retrieve and write the interface code (*.h) and implementation code (*.c) to files.
    std::ofstream outFile("sineComparisonExample.h");
    outFile << generator->interfaceCode();
    outFile.close();

    outFile.open("sineComparisonExample.c");
    outFile << generator->implementationCode();
    outFile.close();

    // If required, change the generator profile to Python.
    auto profile = libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::PYTHON);
    generator->setProfile(profile);

    // Retrieve and write the implementation code (*.py) to a file.
    outFile.open("sineComparisonExample.py");
    outFile << generator->implementationCode();
    outFile.close();

    // END
}
