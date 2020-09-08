from libcellml import Annotator, CellMLElement, Parser

if __name__ == "__main__":

    # STEP 1
    # Parse an existing CellML model from a file.
    read_file = open("annotationExample.cellml", "r")
    parser = Parser()
    model = parser.parseModel(read_file.read())

    # STEP 2
    # Create an Annotator instance and pass the model to it for processing.
    annotator = Annotator()
    annotator.setModel(model)

    # Retrieve a dictionary of all the id strings and the types of items they represent.
    dictionary = annotator.dictionary()
    print('Existing id strings are:')
    for i, itemType in dictionary:
        print(' {} = {}'.format(i, itemType))
    print()

    # STEP 3
    # Retrieve items by their id where the item type is known.

    # Retrieve a component with the id of 'yellow'. We can only do this because 
    # we have prior knowledge that the item with id of 'yellow' is actually 
    # a Component.  
    component = annotator.component('yellow')

    # The same applies to the other item types below.
    variable = annotator.variable('indigo')
    reset = annotator.reset('violet')
    importSource = annotator.importSource('orange')
    units = annotator.units('green')
    unit = annotator.unit('blue')
    connection = annotator.connection('beige')
    mapVariables = annotator.mapVariables('puce')

    # Some kinds of items are returned by their parent item.  These are:
    #  - componentRef: returns the Component with this id on its encapsulation item.
    componentRef = annotator.componentRef('black')

    #  - encapsulation: returns the Model with this id on its encapsulation item.
    encapsulation = annotator.encapsulation('brown')

    #  - resetValue: returns the Reset with this id on its reset value.
    resetValue = annotator.resetValue('taupe')

    #  - testValue: returns the Reset with this id on its test value.
    testValue = annotator.testValue('mauve')
    
    # In this example reset, resetValue and testValue will be the same because the
    # 'taupe' reset value and 'mauve' test value are in the 'violet' reset item.
    
    # STEP 4
    # Dealing with unique id strings where the item has an unknown type.

    # Check that the id is unique in the model scope:
    if annotator.isUnique('green'):
        # Retrieve item from the annotator by their unique id.
        itemOfUnknownType = annotator.item('green')

    # Because these could be any kind of item, they are stored in an AnyItem
    # type.  This is a tuple, where the first item is a CellMLElement enum 
    # indicating the item's type, and the second is the item itself. 

    # The type can be turned into a string using the Annotator.typeAsString 
    # function on the first item in the tuple.
    print('The item with id of "green" has type of "{}".'.format(annotator.typeAsString(itemOfUnknownType[0])))
    print()

    # STEP 5
    # Handling duplicate ID strings.

    # Find any duplicated ID strings inside the model.
    duplicatedIds = annotator.duplicateIds()
    print('There are {} duplicated ids in the model.'.format(len(duplicatedIds)))
    for i in duplicatedIds:
        print(' - ' + i)
    print()

    # Retrieve all items with the given id string. This returns a std.vector
    # of AnyItems which will need to be cast into libcellml items before they
    # can be used.  Note that duplicated ids are not valid CellML, and need 
    # to be fixed before the model can be used.
    allItemsWithDuplicateId1 = annotator.items('duplicateId1')

    # A new id string which is automatically generated and unique can be
    # assigned to these items.
    print('Before assigning automatic ids there are {} items with an id of "duplicateId1".'.format(annotator.duplicateCount('duplicateId1')))
    for item in allItemsWithDuplicateId1:
        annotator.assignId(item)
    
    # Now there are no more items with the duplicated id 'duplicateId1'
    # remaining in the model.
    allItemsWithDuplicateId1 = annotator.items('duplicateId1')
    print('After assigning automatic ids there are {} items with an id of "duplicateId1".'.format(annotator.duplicateCount('duplicateId1')))
    print()

    # It's straightforward to use a double loop to automatically assign new and unique ids to
    # any duplicated ids in the model.
    duplicatedIds = annotator.duplicateIds()
    for i in duplicatedIds:
        itemsWithThisId = annotator.items(i)
        for item in itemsWithThisId:
            annotator.assignId(item)

    # Get the list of duplicates again.
    duplicatedIds = annotator.duplicateIds()
    print('There are {} duplicated ids in the model.'.format(len(duplicatedIds)))

    # STEP 6
    # Automatically assign id strings to anything without them in the model.
    # This can be done by item type (eg: all the components, all the resets, etc)
    # or en masse to everything (excluding MathML) which can have an id attribute.
    
    # Print all component ids to the terminal.
    print('Before automatic ids are assigned by type:')
    print('  Component 1: {}'.format(model.component('component1').id()))
    print('  Component 2: {}'.format(model.component('component2').id()))
    print('  Component 3: {}'.format(model.component('component2').component('component3').id()))
    print('  Component 4: {}'.format(model.component('component4').id()))

    # Assigns an automatic id string to all Component items which don't already have one.
    annotator.assignIds(CellMLElement.COMPONENT)

    print('After automatic ids are assigned to component items:')
    print('  Component 1: {}'.format(model.component('component1').id()))
    print('  Component 2: {}'.format(model.component('component2').id()))
    print('  Component 3: {}'.format(model.component('component2').component('component3').id()))
    print('  Component 4: {}'.format(model.component('component4').id()))
    print()

    numberOfIds = len(annotator.dictionary())
    print('Before assigning all automatic ids, there are {} items with an id attribute.'.format(numberOfIds))

    # Automatically assign ids to everything in the model without one already.
    annotator.assignAllIds()

    # Check the dictionary again.
    numberOfIds = len(annotator.dictionary())
    print('After assigning all automatic ids:, there are {} items with an id attribute.'.format(numberOfIds))

    # Completely clear all ids in the model.
    annotator.clearAllIds()
    numberOfIds = len(annotator.dictionary())
    print('After clearing all ids, there are {} items with an id attribute.'.format(numberOfIds))

    # END
