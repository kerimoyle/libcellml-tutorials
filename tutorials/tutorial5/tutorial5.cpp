/**
 *  TUTORIAL 6: Annotating a mystery model
 *
 * 	This tutorial is a guide to playing Marco Polo using libCellML.
 *
 *	By the time you have worked through this tutorial you will be able to:
 * 		- Parse a CellML file into a Model instance;
 * 		- Determine the type of item with a given id;
 * 		- Use the Annotator class to retrieve an item using only its id
 *string;
 * 		- Repair duplicated id strings within the model scope; and
 * 		- Automatically generate and assign unique ids to any or all
 *items.
 *
 * Background:
 * 		"Marco Polo" is a game played with many people.  One person calls
 *      "Marco" with their eyes closed.  Others answer "Polo" and the first person
 *      must find them by following the sound.  In this tutorial you are given two id
 *      strings - "marco" and "polo" - and a mystery CellML model file.  We will work
 *      through how the Annotator class can be used to locate the desired objects.
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "tutorial_utilities.h"

int main()
{

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 1: Parse a mystery model                          " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  1.a 
    //      Read the mystery file, MysteryModel.cellml.

    //  1.b 
    //      Create a Parser item.
    
    //  1.c 
    //      Use the parser to deserialise the contents of the string you've read
    //      and return the model.

    //  1.d 
    //      Check that the parser has not raised any issues.

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Find 'marco'		                            " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //  2.a
    //      Create an Annotator item and use the setModel function to pass in the parsed
    //      mystery model.

    // The item function returns a libcellml::AnyItem, a std::pair whose:
    //     - first attribute is a libcellml::CellmlElementType enumeration; and
    //     - second attribute is a std::any cast of the item itself.

    //  2.b
    //      Retrieve the item with an id of "marco".  Use the helper function
    //      getCellmlElementTypeFromEnum to convert the enumeration of its type into a
    //      string for printing to the terminal.
   
    //  2.c
    //      Check that the annotator has not reported any issues.
   
    //  2.d
    //      Now that we know the marco item's type using its first attribute (it should
    //      be a libcellml::CellmlElementType::VARIABLE) we can cast it into a usable item
    //      using std::any_cast.  Cast the second attribute of the macro item into a
    //      libcellml::VariablePtr item.
   
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Find 'polo'		                            " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //  3.a
    //      Now try the same procedure to find the item with id of "polo".
    //      Retrieve the item and print its type to the terminal.
   
    //  3.b
    //      The item type returned is libcellml::CellmlElementType::UNDEFINED ... so we 
    //      need to check what the annotator has to say about it. 
    //      Retrieve the issues from the annotator and print to the terminal.
   
    // Recorded 1 issues:
    // Issue [0] is a WARNING:
    //     description: The id 'polo' occurs 6 times in the model so a unique item cannot be located.
    //     stored item type: UNDEFINED

    //      Since the id is not unique, we need to retrieve a vector of all items 
    //      with that id to investigate them.

    //  3.c
    //      Use the items function to retrieve the vector of items with id "polo", 
    //      and iterate through it printing the different types to the terminal.

    //     The items with an id of 'polo' have types of:
    //   - [0] UNITS
    //   - [1] UNITS
    //   - [2] UNIT
    //   - [3] VARIABLE
    //   - [4] RESET
    //   - [5] RESET_VALUE

    //      The item we want has type libcellml::CellMLElements::UNIT, and we'd like it
    //      to be unique.  We need to change the other items to have other (also unique)
    //      ids.  The Annotator class can create a unique id for an item using the assignId function.
    //      This is overloaded so that you can pass in any libCellML item, as well as an AnyItem
    //      pair.  NB: You need to be aware of the default types assigned when passing in CellML items
    //      without a corresponding item type.  These are listed in the documentation.

    //  3.d
    //      Assign an automatic id to all of the items with id "polo", except for the one whose
    //      type is UNIT.

    //  3.e
    //      Check that the id of "polo" is now unique in the model by calling the 
    //      isUnique function.


    //  Now we know that there is only one item in the model with id "polo", and we also know
    //  that it has type UNIT.  This means that we can retrieve a Unit item directly from the
    //  annotator rather than needing to cast it using the std::any_cast.  Instead of calling
    //  the annotator's item function, call the Annotator::unit function with the id "polo" to return the 
    //  unit item directly.

    //  3.f
    //      Retrieve the Unit with id polo without casting.

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 4: See who else is lurking in this pool            " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //  Now that we've found Marco and fixed the duplicates of Polo, we'd like to know
    //  what other ids are being used in this model.

    //  4.a
    //      Use the Annotator::ids function to return a vector of id strings used in the model, and 
    //      print them to the terminal.

    //  The hex strings printed are those which have been automatically generated by the assignId
    //  function; we can also see the 'marco' and 'polo' ids as expected.

    //  4.b
    //      Use the duplicateIds function to return a vector of those ids which have been duplicated in 
    //      the model, and print them to the terminal.

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 5: See who else is lurking around the corner      " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //      The final step is to make sure that imported items can have their annotations
    //      tracked back to their sources too.  

    //  5.a
    //      Retrieve an item with id of "whoAmIAndWhereDidIComeFrom" and print its item type
    //      to the terminal.

    //  5.b
    //      Cast it into a CellML item of the appropriate type.

    //  5.c
    //      Use the Component::isImport() function to verify that it is imported.

    //  5.d
    //      Create an Importer instance and use it to resolve this model's imports.
    //      Check that it has not raised any issues.

    //  5.e
    //      Retrieve all the information needed to locate any annotations on the 
    //      original item:
    //           - the URL from which it was imported; and
    //           - the id of the item in the original model.
    //      Print these to the terminal.

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 6: Give up and go home                            " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //  6.a
    //      Loop through all of the model's components and print their id to the terminal.
    //      Use the assignIds string with an item type (libcellml::CellmlElementType::COMPONENT)
    //      to give all of the items of that type a new unique id.  Print the ids again and
    //      notice that the blanks have been filled with automatically generated strings, 
    //      but existing ids are unchanged. 

    //  6.b
    //      Finally, we decide that it's too cold for swimming, and want to nuke all the ids
    //      and go home.
    //      Use the clearAllIds function to completely remove all id strings from the model.
    //      Check that they have gone by repeating step 4.a to print any ids to the terminal.

    //  6.c
    //      Go looking for Marco, but he's gone home already.
    //      Try and retrieve an item with id "marco" and check that a null pointer is returned.
    //      Retrieve and print any issues to the terminal.

    //  6.d
    //      Regret nuking our friends and make plans to return tomorrow and
    //      annotate everything.  Use the assignAllIds function to give an automatic
    //      id to everything in the model.

    //  6.e
    //      Try to retrieve duplicated ids from the annotator as in step 4.b, and
    //      check that it returns an empty list.

}
