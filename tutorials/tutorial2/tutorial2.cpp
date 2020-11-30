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

    //  1.b
    //      Create a Parser instance, and submit your string for serialisation
    //      into a new model.

    //  1.c
    //      Use the printModel utility function to display the contents of the 
    //      parsed model in the terminal.

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Validate the model                  " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    //  2.a
    //      Create a Validator and pass the model into it.

    //  2.b   
    //      Check the number of issues returned from the validator.

    //  2.c  
    //      Retrieve the issues, and print their description, url, reference, and
    //      type of item stored to the terminal.  The type of stored item is
    //      available as an enum, which can be turned into a string for output using
    //      the utility function, getItemTypeFromEnum(type).

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Fix the issues reported             " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    //  Validator issue[0]:
    //      Description: Variable '1st' in component 'i_am_a_component' does not have a valid name attribute. CellML identifiers must not begin with a European numeric character [0-9].
    //      Type of item stored: VARIABLE
    //      URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.1
    //      See section 2.8.1.1 in the CellML specification.
    //
    //  3.a
    //      Retrieve the variable named '1st' from the component named 'i_am_a_component' and change its name
    //      to 'a'.


    //  Validator issue[1]:
    //      Description: Variable 'b' in component 'i_am_a_component' does not have any units specified.
    //      Type of item stored: VARIABLE
    //      URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    //      See section 2.8.1.2 in the CellML specification.
    //
    //  3.b
    //      Retrieve the variable directly from the issue using the Issue::variable() function to return it.
    //      Note that we can only do this because we know that the item type stored is a VARIABLE.
    //      Set its units to be "dimensionless".


    //  Validator issue[2]:
    //     Description: Variable 'c' in component 'i_am_a_component' has an invalid initial value 'this_variable_doesnt_exist'. Initial values must be a real number string or a variable reference.
    //     Type of item stored: VARIABLE
    //     URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.2.2
    //     See section 2.8.2.2 in the CellML specification.
    //
    //  For this step we're going to pretend that we don't know the item type stored with the issue.
    //  We can retrieve its item using the item() function, which will return a std::any item.  We
    //  can retrieve its type using the cellmlElementType() function to return the CellmlElementType enum,
    //  and then cast the item accordinly.
    //
    //  3.c
    //      Use the item() function to retrieve a std::any cast of the item from the third issue.  
    //      Use the cellmlElementType() to check that its type is a VARIABLE, and then cast
    //      into a VariablePtr using std::any_cast so that you can use it as normal.
    //      Set its initial value to 20.


    //  Validator issue[3]:
    //      Description: Variable 'd' in component 'i_am_a_component' has a units reference 'i_dont_exist' which is neither standard nor defined in the parent model.
    //      Type of item stored: VARIABLE
    //      URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    //      See section 2.8.1.2 in the CellML specification.
    //  
    //      This error is similar in implication to that in 3.b: the validator is reporting that it can't find
    //      the Units item specified by a Variable.  It could be fixed in two different ways:
    //      - by supplying a Units item called "i_dont_exist"; or
    //      - by changing the name of the units which the variable requires to one that is available.
    //
    //  3.d
    //      Change the name of the units required by variable 'd' to be those which are called 'i_am_a_units_item'.
    //      You will need to retrieve these units from the model in order to pass them to the variable.


    //  Validator issue[4]:
    //      Description: MathML ci element has the child text 'a' which does not correspond with any variable names present in component 'i_am_a_component'.
    //      Type of item stored: MATH
    //      URL: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB12.html?issue=2.12.3
    //      See section 2.12.3 in the CellML specification.
    //
    //  This issue is already resolved by fixing the name of the variable in step 3.a.  

    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "  STEP 4: Check and output the corrected model " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    //  4.a
    //      Validate the corrected model again and check that there are no more issues.

    //  4.b
    //      Print the corrected model to the terminal.

    //  4.c
    //      Print corrected model to a file.

    //  4.d
    //      Go and have a cuppa, you're done!
}
