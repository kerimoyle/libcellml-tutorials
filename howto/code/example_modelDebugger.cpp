/**
 *  DEMONSTRATION: Interaction for model validation and analysis tools.
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

    std::string inFileName = "resources/debugAnalysisExample.cellml";
    std::ifstream inFile(inFileName);
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();

    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(inFileContents.str());

    // STEP 2
    // Resolve any imports and flatten the model for analysis.
    auto importer = libcellml::Importer::create();

    // Resolve the imports.
    importer->resolveImports(model, "resources/");

    // Check for issues.
    std::cout << "The importer found "<<importer->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < importer->issueCount(); ++i) {
        auto issue = importer->issue(i);
        std::cout << issue->description() << std::endl;
    }

    // Flatten the model.
    model = importer->flattenModel(model);

    // STEP 3
    // Create an Validator instance and pass the model to it for processing.
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);

    // Print any issues to the terminal.
    std::cout << "The validator found "<<validator->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < validator->issueCount(); ++i) {
        auto issue = validator->issue(i);
        std::cout << issue->description() << std::endl;
    }

    // STEP 4
    // Fix the validation errors.

    // Add units to the variable 'b' in component 'validationErrors'.
    model->component("validationErrors")->variable("b")->setUnits("dimensionless");

    // Change the name of the variable 'iShouldBeNamed_c' to be 'c'.
    model->component("validationErrors")->variable("iShouldBeNamed_c")->setName("c");

    // Check again.
    validator->validateModel(model);
    std::cout << "The validator found "<<validator->issueCount() << " issues." << std::endl;

    // STEP 5
    // The Analyser will find errors in the logic or mathematical formulation of the model's equations,
    // so may return issues even when the model is valid (syntactically correct).
    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(model);

    std::cout << "The analyser found "<<analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        auto issue = analyser->issue(i);
        std::cout << issue->description() << std::endl;
    }

    // STEP 6
    // Fix the analysis errors.  This may need several iterations of checking before all errors have been 
    // resolved.

    // Variable 'd' in component 'importedComponent' is initialised using variable 'e', but it is not a constant.
    model->component("importedComponent")->variable("d")->setInitialValue(22);

    analyser->analyseModel(model);
    std::cout << "The analyser found "<<analyser->issueCount() << " issues." << std::endl;
    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        auto issue = analyser->issue(i);
        std::cout << issue->description() << std::endl;
    }

    // Variable 'x' in component 'nonInitialisedStateVariable' is used in an ODE, but it is not initialised.
    model->component("nonInitialisedStateVariable", true)->variable("x")->setInitialValue(0.0);

    // Variable 'x' in component 'overconstrainedModel' is computed more than once.
    std::string mathml = 
        "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\">\n"
        "  <apply>\n"
        "    <eq/>\n"
        "    <ci>x</ci>\n"
        "    <cn cellml:units=\"dimensionless\">3</cn>\n"
        "  </apply>\n"
        "</math>\n";
    model->component("overconstrainedModel", true)->setMath(mathml);

    // Variable 'x' in component 'uncomputedVariable' is not computed.
    model->component("uncomputedVariable")->setMath(mathml);

    // Variable 'a' in component 'validationErrors' is not computed.
    // Variable 'c' in component 'validationErrors' is not computed.
    model->component("validationErrors")->variable("c")->setInitialValue(1.0);

    // Check again.
    validator->validateModel(model);
    analyser->analyseModel(model);

    std::cout << "The validator found "<<validator->issueCount() << " issues." << std::endl;
        for(size_t i = 0; i < validator->issueCount(); ++i) {
        auto issue = validator->issue(i);
        std::cout << issue->description() << std::endl;
    }

    std::cout << "The analyser found "<<analyser->issueCount() << " issues." << std::endl;

    for(size_t i = 0; i < analyser->issueCount(); ++i) {
        auto issue = analyser->issue(i);
        std::cout << issue->description() << std::endl;
    }

    // STEP 7
    // Write the flattened, validated, analysed model to a serialised CellML string.
    auto printer = libcellml::Printer::create();
    std::string serialisedModelString = printer->printModel(model);

    // Write the serialised string to a file.
    outFileName = "debugAnalysisExampleFixed.cellml";
    outFile.open(outFileName);
    outFile << serialisedModelString;
    outFile.close();

    // END
}
