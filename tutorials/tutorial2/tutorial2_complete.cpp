/**
 *  TUTORIAL 2: ERROR CHECKING AND VALIDATION
 *
 *  By the time you have worked through Tutorial 2 you will be able to:
 *    - Use the Parser to report issues encountered when reading a file or
 *      deserialising a string; and
 *    - Use the Validator to check for issues related to a model's description
 *      as compared to the CellML2.0 specifications.
 *
 *  This tutorial assumes that you can already:
 *    - Read and deserialise a CellML model from a file;
 *    - Retrieve the name and id of models, components, and variables;
 *    - Navigate through the hierarchy of the model (model contains component(s)
 *      contain(s) variable(s) and maths); and
 *    - Serialise and print a Model structure to a CellML file.
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "../tutorial_utilities.h"

int main()
{
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "   TUTORIAL 2: ERROR CHECKING AND VALIDATION   " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    // ---------------------------------------------------------------------------
    //  STEP 1:   Create a CellML Model from the contents of a CellML file
    //
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "   STEP 1: Parse a file into a model           " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    //  1.a
    //      Read the contents of the tutorial2.cellml file into a string.
    std::string inFileName = "tutorial2.cellml";
    std::ifstream inFile(inFileName);
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();
    std::cout << "Opening the CellML file: '" << inFileName << "'" << std::endl;

    //  1.b
    //      Create a Parser instance, and submit your string for serialisation
    //      into a new model.
    auto parser = libcellml::Parser::create();
    auto model = parser->parseModel(inFileContents.str());

    //  1.c
    //      Use the printModel utility function to display the contents of the 
    //      parsed model in the terminal.
    printModel(model);

    //  end 1

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Validate the model                  " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    //  2.a
    //      Create a Validator and pass the model into it.
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);

    //  2.b   
    //      Check the number of issues returned from the validator.
    int numberOfValidationIssues = validator->issueCount();
    if (numberOfValidationIssues != 0) {
        std::cout << "The validator has found " << numberOfValidationIssues
                  << " issues!" << std::endl;

    //  2.c  
    //      Retrieve the issues, and print their description, url, reference, and
    //      type of item stored to the terminal.  The type of stored item is
    //      available as an enum, which can be turned into a string for output using
    //      the utility function, getItemTypeFromEnum(type).
        for (size_t e = 0; e < numberOfValidationIssues; ++e) {
            libcellml::IssuePtr validatorIssue = validator->issue(e);
            std::string issueSpecificationReference =
                validatorIssue->referenceHeading();

            std::cout << "  Validator issue[" << e << "]:" << std::endl;
            std::cout << "     Description: " << validatorIssue->description()
                      << std::endl;
            std::cout << "     Type of item stored: " << getCellmlElementTypeFromEnum(validatorIssue->cellmlElementType()) << std::endl;
            std::cout << "     URL: " << validatorIssue->url() << std::endl;
            if (issueSpecificationReference != "") {
                std::cout << "    See section " << issueSpecificationReference
                          << " in the CellML specification." << std::endl;
            }
        }
    }

    //  end 2

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Fix the issues reported             " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    //  Validator issue[0]:
    //      Description: Variable '1st' in component 'i_am_a_component' does not have a valid name attribute. CellML identifiers must not begin with a European numeric character [0-9].
    //      Type of item stored: VARIABLE
    //      URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.1
    //      See section 2.8.1.1 in the CellML specification.

    //  3.a
    //      Retrieve the variable named '1st' from the component named 'i_am_a_component' and change its name
    //      to 'a'.
    auto component = model->component("i_am_a_component", true);
    auto a = component->variable("1st");
    a->setName("a");
    // This could be done in a chain: model->component("i_am_a_component", true)->variable("1st")->setName("a");

    //  end 3.a 

    //  Validator issue[1]:
    //      Description: Variable 'b' in component 'i_am_a_component' has a units reference 'i_am_not_a_unit' which is neither standard nor defined in the parent model.
    //      Type of item stored: VARIABLE
    //      URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    //      See section 2.8.1.2 in the CellML specification.

    //  3.b
    //      Retrieve the variable directly from the issue using the Issue::variable() function to return it.
    //      Note that we can only do this because we know that the item type stored is a VARIABLE.
    //      Set its units to be "dimensionless".
    auto issue1 = validator->issue(1);
    auto b = issue1->variable();
    b->setUnits("dimensionless");

    //  This can be done in a chain too: validator->issue(1)->variable()->setUnits("dimensionless");
    //  end 3.b

    //  Validator issue[2]:
    //     Description: Variable 'c' in component 'i_am_a_component' has an invalid initial value 'this_variable_doesnt_exist'. Initial values must be a real number string or a variable reference.
    //     Type of item stored: VARIABLE
    //     URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.2.2
    //     See section 2.8.2.2 in the CellML specification.

    //  For this step we're going to pretend that we don't know the item type stored with the issue.
    //  We can retrieve its item using the item() function, which will return an AnyItem item.  The
    //  first attribute of an AnyItem is its CellmlElementType enum, and the second attribute is a
    //  std::any cast of the item itself.

    //  3.c
    //      Use the item() function to retrieve a std::any cast of the item from the third issue.  
    //      Use the cellmlElementType() to check that its type is a VARIABLE, and then cast
    //      into a VariablePtr using std::any_cast so that you can use it as normal.
    //      Set its initial value to 20.
    auto issue2 = validator->issue(2);
    auto item = issue2->item();
    assert(issue2->cellmlElementType() == libcellml::CellmlElementType::VARIABLE);
    auto c = std::any_cast<libcellml::VariablePtr>(item);
    c->setInitialValue(20.0);

    //  end 3.c

    //  Validator issue[3]:
    //     Description: Variable 'd' in component 'i_am_a_component' does not have any units specified.
    //     Type of item stored: VARIABLE
    //     URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    //     See section 2.8.1.2 in the CellML specification.

    //  3.d
    //      Prove to yourself that 


    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "  STEP 4: Check and output the corrected model " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    //  4.a
    //      Print the corrected model to the terminal using the utility function,
    //      printModel.
    printModel(model);

    //  4.b
    //      Validate the corrected model and check that there are no more issues.
    validator->validateModel(model);
    std::cout << "The validator found " << validator->issueCount()
              << " issues in the model." << std::endl;

    //  4.c
    //      Print corrected model to a file.
    auto printer = libcellml::Printer::create();
    std::string serialisedModelString = printer->printModel(model);
    std::string outFileName = "tutorial2_printed.cellml";
    std::ofstream outFile(outFileName);
    outFile << serialisedModelString;
    outFile.close();

    std::cout << "The corrected '" << model->name()
              << "' model has been printed to: " << outFileName << std::endl;

    //  4.d
    //      Go and have a cuppa, you're done!
}
