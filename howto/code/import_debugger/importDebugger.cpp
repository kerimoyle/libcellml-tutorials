/**
 *  DEMONSTRATION: Debug importing models.
 * 
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

int main()
{
    // STEP 1
    // Parse an existing CellML model from a file.

    std::string inFileName = "resources/importExample1.cellml";
    std::ifstream inFile(inFileName);
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();

    auto parser = libcellml::Parser::create();
    auto originalModel = parser->parseModel(inFileContents.str());

    // STEP 2
    // Create an Importer to resolve the imports in the model.
    auto importer = libcellml::Importer::create();

    // Resolve the imports.
    importer->resolveImports(originalModel, "resources/");

    // Check for issues.
    std::cout << std::endl;
    std::cout << "The importer found "<<importer->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < importer->issueCount(); ++i) {
        auto issue = importer->issue(i);
        std::cout << issue->description() << std::endl;
    }
    std::cout << std::endl;

    // STEP 3
    // The analysis tools - the Validator and Analyser - will read only the submitted
    // model; they do not look into any of the imported items, so they can't check them.
    // In order to retain the import structure but be able to use the diagnostic tools, 
    // we can either:
    //  - create a flattened copy of the model for testing, which can be used to
    //    identify mistakes in the unflattened model too; or
    //  - make use of the Importer's library to iterate through all of the model's 
    //    imported dependencies, and check them individually.

    // Create a Validator and Analyser and submit the original, unflattened model.
    // We don't expect either of these to report any issues because all of the mistakes are
    // in imported models which are not read by these classes.
    auto validator = libcellml::Validator::create();
    validator->validateModel(originalModel);

    std::cout << "Investigating the original model:" << std::endl;
    std::cout << " - the validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << "    - " << validator->issue(i)->description() << std::endl;
    }

    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(originalModel);
    std::cout << " - the analyser found " << analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        std::cout << "    - " << analyser->issue(i)->description() << std::endl;
    }

    // Create a flattened version to demonstrate the diagnostics.
    auto flatModel = importer->flattenModel(originalModel);

    // Repeat the validation and analysis above on the flattened model, noting that the 
    // flat model contains errors that were hidden in the original one.
    validator->validateModel(flatModel);
    std::cout << std::endl << "Investigating the flattened model:" << std::endl;
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << "    - " << validator->issue(i)->description() << std::endl;
    }
    analyser->analyseModel(flatModel);
    std::cout << "The analyser found "<< analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        std::cout << "    - " << analyser->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    // STEP 4
    // The Validator and Analyser classes process only the contents of concrete items (ie: not the contents of 
    // imported items) of a model.
    // After successfully resolving a model's imports using an importer, the importer will store instances
    // of all of the dependencies of the resolved model.  These are accessible through the "library" function.
    // We can ascertain that all of import dependencies meet the diagnostic checks of the Validator and the 
    // Analyser individually by iterating through the importer's library.

    // Loop through the importer library and call the validator for each model.  
    for(size_t m = 0; m < importer->libraryCount(); ++m) {

        // Retrieve the library model by index, m.
        validator->validateModel(importer->library(m));

        // Retrieve the key under which it's stored: this will be the URL at which the imported model was found.
        std::cout << "The validator found "<<validator->issueCount() << " issues in " <<importer->key(m) << std::endl;
        for(size_t i = 0; i < validator->issueCount(); ++i) {
            std::cout << "    - " << validator->issue(i)->description() << std::endl;
        }
    }
    std::cout << std::endl;

    // STEP 5
    // Fix the validation errors in the imported files.
    // According to the printout above, we need to add units to the "iNeedUnits"
    // variable, to be found inside the "importExample3.cellml" file.
    // To fix this, we need to fix the model inside the "importExample3.cellml" file.
    // When the originalModel's imports were resolved, this model was added to the
    // library in the Importer.  We can retrieve the model from there for repair.

    // Retrieve from the library by key.  Note the inclusion of the directory used to
    // resolve imports for the original model is included in the key string.
    auto importedModel1 = importer->library("resources/importExample3.cellml");

    // Add units to the variable that needs them to fix the validation error.
    importedModel1->component("importThisOneToo")->variable("iNeedUnits")->setUnits("dimensionless");

    // Check that the issues have been fixed.
    validator->validateModel(importedModel1);
    std::cout << std::endl << "Investigating the repaired model:" << std::endl;
    std::cout << "The validator found "<<validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << "    - " << validator->issue(i)->description() << std::endl;
    }

    // STEP 6
    // Repeat steps 4 and 5 using the Analyser instead of the Validator.

    // Loop through the importer library and call the analyser for each model.  
    for(size_t m = 0; m < importer->libraryCount(); ++m) {

        // Retrieve the library model by index, m.
        analyser->analyseModel(importer->library(m));

        // Retrieve the key under which it's stored: this will be the URL at which the imported model was found.
        std::cout << "The analyser found " << analyser->issueCount() << " issues in " <<importer->key(m) << std::endl;
        for(size_t i = 0; i < analyser->issueCount(); ++i) {
            std::cout << "    - " << analyser->issue(i)->description() << std::endl;
        }
    }
    std::cout << std::endl;

    // Fix the error by adding a MathML block to the component named "concreteComponent" 
    // in the "importExample2b.cellml" model.
    auto importedModel2 = importer->library("resources/importExample2b.cellml");
    std::string mathml = 
        "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\">\n"
        "  <apply>\n"
        "    <eq/>\n"
        "    <ci>iAmNotCalculated</ci>\n"
        "    <cn cellml:units=\"dimensionless\">3</cn>\n"
        "  </apply>\n"
        "</math>\n";
    importedModel2->component("concreteComponent")->setMath(mathml);

    // Check that the issue has been fixed.
    analyser->analyseModel(importedModel2);
    std::cout << std::endl << "Investigating the repaired model:" << std::endl;
    std::cout << "The analyser found " << analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        std::cout << "    - " << analyser->issue(i)->description() << std::endl;
    }
    std::cout << std::endl;

    // Recreate the flattened model, and check it again.  This will use the updated model
    // in the importer library as its source.
    flatModel = importer->flattenModel(originalModel);

    validator->validateModel(flatModel);
    std::cout << std::endl << "Investigating the flattened model:" << std::endl;
    std::cout << "The validator found "<<validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        std::cout << "    - " << validator->issue(i)->description() << std::endl;
    }

    analyser->analyseModel(flatModel);
    std::cout << "The analyser found "<<analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        std::cout << "    - " << analyser->issue(i)->description() << std::endl;
    }

    // STEP 7
    // Print the collection of repaired import models to files.
    // To avoid over-writing existing files, we'll write the corrected files to a separate
    // directory called "repaired/".  Note that the relationship between the files needs
    // to be maintained, so even files that have not been changed need to be written
    // to the new location.

    // Write the original model to a file. 
    auto printer = libcellml::Printer::create();
    std::ofstream outFile("repaired/import_debugger/importExample1.cellml");
    outFile << printer->printModel(originalModel);
    outFile.close();

    // Write the dependency models in the importer library to files.
    for(size_t m = 0; m < importer->libraryCount(); ++m) {
        outFile.open("repaired/" + importer->key(m));
        outFile << printer->printModel(importer->library(m));
        outFile.close();
    }

    std::cout << "The corrected models has been written to the 'repaired/resources/' directory" << std::endl;

    // END
}


