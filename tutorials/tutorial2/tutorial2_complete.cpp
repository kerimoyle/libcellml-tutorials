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

    //  3.a
    //  Validator issue[0]:
    //    Description: CellML identifiers must not begin with a European numeric
    //    character [0-9].
    //    See section 3.1.4 in the CellML specification.
    //  Validator issue[1]:
    //    Description: Variable does not have a valid name attribute.
    //    See section 11.1.1.1 in the CellML specification.
    // These issues refer to the same thing, but are better read in reverse order.
    // Variables (and everything else in CellML) which specify a name attribute
    // can must have the correct format.  Comparing the issue to the names of
    // entities printed in the terminal we can see that variable[0] in
    // component[0] doesn't have a valid name, so let's fix it.
    model->component(0)->variable(0)->setName("a");

    //  3.b
    //  Validator issue[2]:
    //    Description: Variable 'b' has an invalid units reference
    //    'i_am_not_a_unit' that does not correspond with a standard unit or units
    //    in the variable's parent component or model.
    //    See section 11.1.1.2 in the CellML specification.
    //  Variables must have a unit defined.  These can be either something from
    //  the built-in list within libCellML (which you can read in the
    //  specifications document), or something you define yourself.  We'll look at
    //  user defined units in Tutorial 3, but for now it's enough to see that the
    //  units which are associated with variable 'b' is not valid.  We'll change
    //  it to be 'dimensionless' instead.  NB items can be accessed through their
    //  name (as here) or their index (as above)
    model->component("i_am_a_component")
        ->variable("b")
        ->setUnits("dimensionless");

    //  3.c
    //  Validator issue[3]:
    //    Description: Variable 'c' has an invalid initial value
    //    'this_variable_doesnt_exist'. Initial values must be a real number
    //    string or a variable reference.
    //    See section 11.1.2.2 in the CellML specification.
    //  We can either access members by their index or their name, as shown above,
    //  or we can create a pointer to them instead.
    //  Initial values (if set) must be a valid variable name in the same
    //  component, or a real number.
    auto variableC = model->component(0)->variable("c");
    variableC->setInitialValue(20.0);

    //  3.d
    //  Validator issue[4]:
    //    Description: CellML identifiers must contain one or more basic Latin
    //    alphabetic characters.
    //    See section 3.1.3 in the CellML specification.
    //  Validator issue[5]:
    //    Description: Variable 'd' does not have a valid units attribute.
    //    See section 11.1.1.2 in the CellML specification.
    //  These two issues go together too.  Because we haven't defined a units
    //  attribute for variable 'd', it effectively has a blank name, which is not
    //  allowed.  Simply assigning a unit to the variable will fix both issues.
    auto variableD = model->component(0)->variable("d");
    variableD->setUnits("dimensionless");

    //  3.e
    //  Validator issue[6]:
    //    Description: MathML ci element has the child text 'a' which does not
    //    correspond with any variable names present in component
    //    'i_am_a_component' and is not a variable defined within a bvar element.
    //  The maths block is complaining that it is being asked to compute:
    //    a = b + c
    //  but in the component there was no variable called 'a'.  Since we
    //  corrected this earlier by naming the first variable in component[0] as 'a'
    //  this issue will be fixed already.


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
