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

#include "utilities.h"

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
    printModel(model, true);

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
    auto iAmAComponent = model->component("i_am_a_component", true);
    auto a = iAmAComponent->variable("1st");
    a->setName("a");
    //  This could be done in a chain without instantiating the component and variable:
    //      model->component("i_am_a_component", true)->variable("1st")->setName("a");

    //  end 3.a 

    //  Validator issue[1]:
    //      Description: Variable 'b' in component 'i_am_a_component' does not have any units specified.
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
    //  We can retrieve its item using the item() function, which will return a std::any item.  We
    //  can retrieve its type using the cellmlElementType() function to return the CellmlElementType enum,
    //  and then cast the item accordinly.

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
    //      Description: Variable 'd' in component 'i_am_a_component' has a units reference 'i_dont_exist' which is neither standard nor defined in the parent model.
    //      Type of item stored: VARIABLE
    //      URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    //      See section 2.8.1.2 in the CellML specification.

    //      This error is similar in implication to that in 3.b: the validator is reporting that it can't find
    //      the Units item specified by a Variable.  It could be fixed in two different ways:
    //      - by supplying a Units item called "i_dont_exist"; or
    //      - by changing the name of the units which the variable requires to one that is available.

    //  3.d
    //      Change the name of the units required by variable 'd' to be those which are called 'i_am_a_units_item'.
    //      You will need to retrieve these units from the model in order to pass them to the variable.
    auto iAmAUnitsItem = model->units("i_am_a_units_item");
    validator->issue(3)->variable()->setUnits(iAmAUnitsItem);

    //  end 3.d

    //  This issue was actually also caught by the Parser, which, like the Validator, is a Logger class.
    //  This means that it will keep track of anything it encounters when parsing a model.  You can try calling
    //  Parser::issueCount() etc and iterating through them (just like in 2.c) to see what you find.

    //  Validator issue[4]:
    //      Description: MathML ci element has the child text 'a' which does not correspond with any variable names present in component 'i_am_a_component'.
    //      Type of item stored: MATH
    //      URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB12.html?issue=2.12.3
    //      See section 2.12.3 in the CellML specification.

    //  This issue is already resolved by fixing the name of the variable in step 3.a.  

    //  end 3

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "  STEP 4: Check and output the corrected model " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    //  4.a
    //      Validate the corrected model again and check that there are no more issues.
    validator->validateModel(model);
    std::cout << "The validator found " << validator->issueCount()
              << " issues in the model." << std::endl;

    //  4.b
    //      Print the corrected model to the terminal.
    printModel(model, true);

    //  4.c
    //      Print corrected model to a file.
    auto printer = libcellml::Printer::create();
    std::string serialisedModelString = printer->printModel(model);

    std::string outFileName = "tutorial2_printed.cellml";
    std::ofstream outFile(outFileName);
    outFile << serialisedModelString;
    outFile.close();

    //  end 4

    std::cout << "The corrected '" << model->name()
              << "' model has been printed to: " << outFileName << std::endl;

    //  4.d
    //      Go and have a cuppa, you're done!
}
