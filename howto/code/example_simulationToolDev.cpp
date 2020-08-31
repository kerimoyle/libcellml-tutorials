/**
 * This program is intended to demonstrate a workflow typical of a simulation
 * tool developer who would like to be able to interact with CellML model files.
 * The general principles are:
 * - parse a CellML model file;
 * - resolve any import dependencies in the model;
 * - validate the model (check for semantic/syntactic errors);
 * - debug the model (check for modelling errors);
 * - generate runnable code in C or Python; and
 * - output in the desired format.
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

int main()
{
    // STEP 1.  Parse a CellML file into a model.

    // Read the file containing the CellML model into a string.
    std::string inFileName = "resources/example.cellml";
    std::ifstream inFile(inFileName);
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();

    std::cout << "Opening the CellML file" << std::endl;

    // Create a libCellML Parser, and use it to parse the fileContents
    // string and convert it into a CellML Model structure.
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(inFileContents.str());
    printIssuesToTerminal(parser);

    // STEP 2.  Resolve the import dependencies (if any).

    if(model->hasUnresolvedImports()) {
        auto importer = libcellml::Importer::create();

        // Submit the model to the importer and the absolute location 
        // against which the import reference paths will be resolved.
        importer->resolveModelImports(model, "resources/");

        printErrorsToTerminal(importer);

        // Retrieve a "flattened" (ie: import-free) model from the importer,
        // and use it to over-write the current model.
        model = importer->flattenModel();
    }

    // STEP 3.  Validate the model: check for syntactic and semantic errors.

    // Create a Validator instance and pass the model for checking.
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);
    printErrorsToTerminal(validator);

    // STEP 4.  TODO Analyse a model: check for mathematical and modelling errors.
    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(model);
    printErrorsToTerminal(analyser);

    // STEP 5.  Generate runnable code in other language formats for this model.

    // Create a Generator instance.  Note that by default this is the C language.
    auto generator = libcellml::Generator::create();

    // Pass the generator the model for processing.
    generator->processModel(model);
    printErrorsToTerminal(generator);

    // Retrieve and write the interface code (*.h) and implementation code (*.c) to files.
    std::ofstream outFile("example.h");
    outFile << generator->interfaceCode();
    outFile.close();

    outFile.open("example.c");
    outFile << generator->implementationCode();
    outFile.close();

    // If required, change the generator profile to Python and reprocess the model.
    auto profile = libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::PYTHON);
    generator->setProfile(profile);
    generator->processModel(model);

    // Retrieve and write the implementation code (*.py) to a file.
    outFile.open("example.py");
    outFile << generator->implementationCode();
    outFile.close();

}

