/**
 *  DEMONSTRATION: Debug importing models.
 * 
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

// START PRINT COMPONENT
void printComponent(const libcellml::ComponentPtr &component, std::string &spacer) {
    for(size_t v = 0; v < component->variableCount(); ++v){
        auto variable = component->variable(v);
        std::cout << spacer << "        - Variable: '"<<variable->name()<<"'";
        if(variable->units()) {
            std::cout << " has units '" << variable->units()->name() << "'" << std::endl;
        }
        else {
            std::cout << " has no units" << std::endl;
        }
    }
}
// END PRINT COMPONENT

// START IMPORT FUNCTION
void printImportDependencies(const libcellml::ModelPtr &model, std::string &spacer) {

    // Function to recursively iterate through the import dependencies in this model, and 
    // print their URL and what they require to the terminal.

    // If there are no imports, then print the concrete items.
    if(model->hasUnresolvedImports() || (model->importSourceCount() == 0)) {
        for(size_t u = 0; u < model->unitsCount(); ++u) {
            std::cout << spacer << "    - Units: '"<<model->units(u)->name() << "'" << std::endl;
        }
        for(size_t c = 0; c < model->componentCount(); ++c) {
            std::cout << spacer << "    - Component: '"<<model->component(c)->name() << "'" << std::endl;
            printComponent(model->component(c), spacer);
        }
        return;
    }

    for(size_t i = 0; i < model->importSourceCount(); ++i) {
        // Each import source should have its own model pointer attached now.
        auto importSource = model->importSource(i);
        for(size_t u = 0; u < importSource->unitsCount(); ++u){
            std::cout << spacer << "    - Units: '"<<importSource->units(u)->name() 
                      << "' imports '"<<importSource->units(u)->importReference()<<"' from "<<importSource->url()<<std::endl;
        }
        for(size_t c = 0; c < importSource->componentCount(); ++c){
            auto component = importSource->component(c);
            std::cout << spacer << "    - Component: '"<<component->name() 
                      << "' imports '"<<component->importReference()<<"' from "<<importSource->url()<<std::endl;
            printComponent(component, spacer);
        }
        std::string bigSpacer = spacer + "    ";

        // Recursively process imports.
        printImportDependencies(importSource->model(), bigSpacer);
    }
}
// END IMPORT FUNCTION

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
    std::cout << "The importer found "<<importer->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < importer->issueCount(); ++i) {
        auto issue = importer->issue(i);
        std::cout << issue->description() << std::endl;
    }

    // STEP 3
    // The analysis tools - the Validator and Analyser - will read only the submitted
    // model; they do not look into any of the imported items, so they can't check them.
    // In order to retain the import structure but be able to use the diagnostic tools, 
    // we can create a flattened copy of the model for testing.  This can be used to
    // identify mistakes in the unflattened model too.  

    // Create a Validator and Analyser and submit the original, unflattened model.
    // We don't expect either of these to report any issues.
    auto validator = libcellml::Validator::create();
    validator->validateModel(originalModel);
    std::cout << "The validator found " << validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        auto issue = validator->issue(i);
        std::cout << issue->description() << std::endl;
    }

    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(originalModel);
    std::cout << "The analyser found " << analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        auto issue = analyser->issue(i);
        std::cout << issue->description() << std::endl;
    }

    // Create a flattened version for diagnostics.
    auto flatModel = importer->flattenModel(originalModel);

    // Repeat the validation and analysis above on the flattened model.
    validator->validateModel(flatModel);
    std::cout << "The validator found "<<validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        auto issue = validator->issue(i);
        std::cout << issue->description() << std::endl;
    }

    analyser->analyseModel(flatModel);
    std::cout << "The analyser found "<<analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        auto issue = analyser->issue(i);
        std::cout << issue->description() << std::endl;
    }

    // STEP 4 
    // The issues reported boil down to just one issue really: that there is a 
    // variable named "i_need_units" that requires units to be defined.  
    // Because of the import structure, this could be hidden inside the importing
    // hierarchy.  We can use a recursive function to print information on the imported
    // items within the unflattened hierarchy. 
    std::string spacer = "";
    printImportDependencies(originalModel, spacer);

    // STEP 5
    // Fix the error in the imported file, and remake the flattened model for checking.
    // According to the printout above, we need to add units to the "i_need_units"
    // variable, to be found inside the "importExample3.cellml" file.
    // To fix this, we need to fix the model inside the "importExample3.cellml" file.
    // When the originalModel's imports were resolved, this model was added to the
    // library in the Importer.  We can retrieve the model from there for repair.
    std::cout << "The Importer contains " <<importer->libraryCount() << " models:"<< std::endl;
    for(size_t i = 0; i < importer->libraryCount(); ++i) {
        std::cout << "  Model " << i << ": " << importer->key(i) << std::endl;
    }
    // Retrieve from the library by key.
    auto importedModel = importer->library("resources/importExample3.cellml");

    // Add units to the variable that needs them.
    importedModel->component("shared")->variable("i_need_units")->setUnits("dimensionless");

    // Recreate the flattened model, and check it again.  This will use the updated model
    // in the importer library as its source.
    flatModel = importer->flattenModel(originalModel);

    validator->validateModel(flatModel);
    std::cout << "The validator found "<<validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        auto issue = validator->issue(i);
        std::cout << issue->description() << std::endl;
    }

    analyser->analyseModel(flatModel);
    std::cout << "The analyser found "<<analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        auto issue = analyser->issue(i);
        std::cout << issue->description() << std::endl;
    }

    // STEP 6
    // Print the collection of repaired import models to files.
    // NOTE that this file should be named "importExample3.cellml" in order
    // for the other files to find it.  It is not named that here so that this
    // example code can be used more than once with the same outputs. 
    auto printer = libcellml::Printer::create();
    std::string serialisedModelString = printer->printModel(importedModel);

    // Write the serialised string to a file.
    std::string outFileName = "resources/importExample3_repaired.cellml";
    std::ofstream outFile(outFileName);
    outFile << serialisedModelString;
    outFile.close();
    std::cout << "The corrected model has been written to " << outFileName << std::endl;

    // END
}


