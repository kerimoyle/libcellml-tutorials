/**
 * COMBINE TUTORIAL ?: Annotating a mystery model
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
    //      Read the mystery file.
    std::ifstream inFile("MysteryModel.cellml");
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();

    //  1.b 
    //      Create a Parser item.
    auto parser = libcellml::Parser::create();

    //  1.c 
    //      Use the parser to deserialise the contents of the string you've read
    //      and return the model.
    auto model = parser->parseModel(inFileContents.str());

    //  1.d 
    //      Check that the parser has not raised any issues.
    printIssues(parser);

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Find 'marco'		                            " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //  2.a
    //      Create an Annotator item and use the setModel function to pass in the parsed
    //      mystery model.
    auto annotator = libcellml::Annotator::create();
    annotator->setModel(model);

    //  end 2.a

    // The item function returns a libcellml::AnyItem, a std::pair whose:
    //     - first attribute is a libcellml::CellMLElement enumeration; and
    //     - second attribute is a std::any cast of the item itself.
    //  2.b
    //      Retrieve the item with an id of "marco".  Use the helper function
    //      getItemTypeAsString to convert the enumeration of its type into a
    //      string for printing to the terminal.
    libcellml::AnyItem marcoItem = annotator->item("marco");
    std::cout << "The item with ID 'marco' is a " << getItemTypeAsString(marcoItem.first) << std::endl;

    // The item with ID 'marco' is a VARIABLE

    //  2.c
    //      Check that the annotator has not reported any issues.
    printIssues(annotator);

    //  2.d
    //      Now that we know the marco item's type using its first attribute (it should
    //      be a libcellml::CellMLElement::VARIABLE) we can cast it into a useable item
    //      using std::any_cast.  Cast the second attribute of the macro item into a
    //      libcellml::VariablePtr item.
    auto marcoVariable = std::any_cast<libcellml::VariablePtr>(marcoItem.second);

    //  end 2

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Find 'polo'		                            " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //  3.a
    //      Now try the same procedure to find the item with id pf "polo".
    //      Retrieve the item and print its type to the terminal.
    auto poloItem = annotator->item("polo");
    std::cout << "The type of item with ID 'polo' is " << getItemTypeAsString(poloItem.first) << std::endl;

    //  3.b
    //      The item type returned is libcellml::CellMLElement::UNDEFINED ... so we 
    //      need to check what the annotator has to say about it. 
    //      Retrieve the issues from the annotator and print to the terminal.
    printIssues(annotator);

    // Recorded 1 issues:
    // Issue [0] is a WARNING:
    //     description: The id 'polo' occurs 6 times in the model so a unique item cannot be located.
    //     stored item type: UNDEFINED

    //  3.c
    //      Since the id is not unique, we need to retrieve a vector of all items 
    //      with that id to investigate them.
    //      Use the items function to retrieve the vector of items with id "polo", 
    //      and iterate through it printing the different types to the terminal.
    auto poloItems = annotator->items("polo");
    std::cout << "The items with an id of 'polo' have types of:" << std::endl;
    size_t index = 0;
    for (auto &item : poloItems)
    {
        std::cout << "  - [" << index << "] " << getItemTypeAsString(item.first) << std::endl;
        ++index; 
    }

    //     The items with an id of 'polo' have types of:
    //   - [0] UNITS
    //   - [1] UNITS
    //   - [2] UNIT
    //   - [3] VARIABLE
    //   - [4] RESET
    //   - [5] RESET_VALUE

    //  end 3.c
    //      The item we want has type libcellml::CellMLElements::UNIT, and we'd like it
    //      to be unique.  We need to change the other items to have other (also unique)
    //      ids.  The Annotator class can create a unique id for an item using the assignId function.
    //      This is overloaded so that you can pass in any libCellML item, as well as an AnyItem
    //      pair.  NB: You need to be aware of the default types assigned when passing in CellML items
    //      without a corresponding item type.  These are listed in the documentation.

    //  3.d
    //      Assign an automatic id to all of the items with id "polo", except for the one whose
    //      type is UNIT.
    poloItem = poloItems.at(2);
    assert(poloItem.first == libcellml::CellMLElement::UNIT);
    poloItems.erase(poloItems.begin() + 2);

    for (auto &item : poloItems) {
        annotator->assignId(item);
    }

    //  3.e
    //      Check that the id of "polo" is now unique in the model by calling the 
    //      isUnique function.
    assert(annotator->isUnique("polo"));

    //  end 3.e

    //  Now we know that there is only one item in the model with id "polo", and we also know
    //  that it has type UNIT.  This means that we can retrieve a UnitItem item directly from the
    //  annotator rather than needing to cast it using the std::any_cast.  Instead of calling
    //  the annotator's item function, call the Annotator::unit function with the id "polo" to return the 
    //  unit item directly.

    //  3.f
    //      Retrieve the UnitItem with id polo without casting.
    auto poloUnit = annotator->unit("polo");

    //  The UnitItem is another std::pair with:
    //      - first attribute is the libcellml::Units parent item; and
    //      - second attribute is the index of this Unit within the parent.
    
    //  end 3.f

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 4: See who else is lurking in this pool            " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //  Now that we've found Marco and fixed the duplicates of Polo, we'd like to know
    //  what other ids are being used in this model.
    //  4.a
    //      Use the Annotator::ids function to return a vector of id strings used in the model, and 
    //      print them to the terminal.
    std::cout << "The id strings used in the model are:" << std::endl;
    auto ids = annotator->ids();
    for(auto &id :ids) {
        std::cout << "  - '"<< id << "'" << std::endl;
    }

    //  end 4.a
    //  The hex strings printed are those which have been automatically generated by the assignId
    //  function; we can also see the 'marco' and 'polo' ids as expected.

    //  4.b
    //      Use the duplicateIds function to return a vector of those ids which have been duplicated in 
    //      the model, and print them to the terminal.
    std::cout << "Duplicated id strings are:" << std::endl;
    auto duplicatedIds = annotator->duplicateIds();
    for(auto &id :duplicatedIds) {
        std::cout << "  - '" << id << "'" << std::endl;
    }

    //  4.c
    //      Loop through all of the model's components and print their id to the terminal.
    //      Use the assignIds string with an item type (libcellml::CellMLElement::COMPONENT)
    //      to give all of the items of that type a new unique id.  Print the ids again and
    //      notice that the blanks have been filled with automatically generated strings, 
    //      but existing ids are unchanged. 
    std::cout << "The components have ids:" << std::endl;
    for(size_t i = 0; i < model->componentCount(); ++i) {
        std::cout << "  - '" << model->component(i)->id() << "'" << std::endl;
    }

    annotator->assignIds(libcellml::CellMLElement::COMPONENT);

    std::cout << "The components have ids:" << std::endl;
    for(size_t i = 0; i < model->componentCount(); ++i) {
        std::cout << "  - '" << model->component(i)->id() << "'" <<std::endl;
    }

    //  end 4

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 5: See who else is lurking around the corner      " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //      The final step is to make sure that imported items can have their annotations
    //      tracked back to their sources too.  

    //  5.a
    //      Retrieve an item with id of "whoAmIAndWhereDidIComeFrom" and print its item type
    //      to the terminal.
    auto whoAmIAndWhereDidIComeFrom = annotator->item("whoAmIAndWhereDidIComeFrom");
    std::cout << "The type of item with ID 'whoAmIAndWhereDidIComeFrom' is " << getItemTypeAsString(whoAmIAndWhereDidIComeFrom.first) << std::endl;
    
    //  5.b
    //      Cast it into a CellML item of the appropriate type.
    auto units = std::any_cast<libcellml::UnitsPtr>(whoAmIAndWhereDidIComeFrom.second);

    //  5.c
    //      Use the Component::isImport() function to verify that it is imported.
    assert(units->isImport());

    //  5.d
    //      Create an Importer instance and use it to resolve this model's imports.
    //      Check that it has not raised any issues.
    auto importer = libcellml::Importer::create();
    importer->resolveImports(model, "");
    printIssues(importer);

    //  5.e
    //      Retrieve all the information needed to locate any annotations on the 
    //      original item:
    //           - the URL from which it was imported; and
    //           - the id of the item in the original model.
    //      Print these to the terminal.
    auto url = units->importSource()->url();
    auto reference = units->importReference();
    auto importedId = units->importSource()->model()->units(reference)->id();

    std::cout << "The units with id 'whoAmIAndWhereDidIComeFrom' came from:" << std::endl;
    std::cout << "  - url: " << url << std::endl;
    std::cout << "  - id: " << importedId << std::endl;
    
    //  end 5

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 6: Give up and go home                            " << std::endl;
    std::cout << "----------------------------------------------------------"<< std::endl;

    //  6.a
    //      Finally, we decide that it's too cold for swimming, and want to nuke all the ids
    //      and go home.
    //      Use the clearAllIds function to completely remove all id strings from the model.
    //      Check that they have gone by repeating step 4.a to print any ids to the terminal.
    annotator->clearAllIds();
    ids = annotator->ids();
    std::cout << "There are " << ids.size() << " ids in the model." << std::endl;

    //  6.b
    //      Go looking for Marco, but he's gone home already.
    //      Try and retrieve an item with id "marco" and check that a null pointer is returned.
    //      Retrieve and print any issues to the terminal.
    marcoItem = annotator->item("marco");
    std::cout << "The type of item with ID 'marco' is " << getItemTypeAsString(marcoItem.first) << std::endl;
    printIssues(annotator);

    //  6.c
    //      Regret nuking our friends and make plans to return tomorrow and
    //      annotate everything.  Use the assignAllIds function to give an automatic
    //      id to everything in the model.
    annotator->assignAllIds();

    //  6.d
    //      Try to retrieve duplicated ids from the annotator as in step 4.b, and
    //      check that it returns an empty list.
    duplicatedIds = annotator->duplicateIds();
    std::cout << "There are " << duplicatedIds.size() << " duplicated ids left in the model." << std::endl;

    //  end 6
}
