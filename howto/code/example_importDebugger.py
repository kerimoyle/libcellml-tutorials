from libcellml import Analyser, Importer, Model, Parser, Printer, Validator

# START PRINT COMPONENT
def print_component(component, spacer):
    for v in range(0, component.variableCount()):
        variable = component.variable(v)
        if(variable.units()):
            print("{}        - Variable: '{}' has units '{}'.".format(spacer, variable.name(), variable.units().name()))
        else:
            print("{}        - Variable: '{}' has no units.".format(spacer, variable.name()))

# END PRINT COMPONENT

# START IMPORT FUNCTION
def print_import_dependencies(model, spacer):
    # Function to recursively iterate through the import dependencies in this model, and 
    # print their URL and what they require to the terminal.

    # If there are no imports, then print the concrete items.
    if model.hasUnresolvedImports() or (model.importSourceCount() == 0):
        for u in range(0, model.unitsCount()):
            print("{}    - Units: '{}'".format(spacer, model.units(u).name()))
        for c in range(0, model.componentCount()):
            print("{}    - Component: '{}'".format(spacer, model.component(c).name()))
        return
    
    for i in range(0, model.importSourceCount()):
        # Each import source should have its own model pointer attached now.
        import_source = model.importSource(i)
        for u in range(0, import_source.unitsCount()):
            print("{}    - Units: '{}' imports '{}' from '{}'".format(
                spacer, 
                import_source.units(u).name(), 
                import_source.units(u).importReference(), 
                import_source.url()))
        
        for c in range(0, import_source.componentCount()):
            component = import_source.component(c)
            print("{}    - Component: '{}' imports '{}' from '{}'".format(
                spacer, 
                component.name(), 
                component.importReference(), 
                import_source.url()))
            print_component(component, spacer)

        big_spacer = spacer + "    "
        # Recursively process imports.
        print_import_dependencies(import_source.model(), big_spacer)

# END IMPORT FUNCTION

if __name__ == '__main__':
    # STEP 1
    # Parse an existing CellML model from a file.
    read_file = open('resources/importExample1.cellml', 'r')
    parser = Parser()
    original_model = parser.parseModel(read_file.read())

    # STEP 2
    # Create an Importer to resolve the imports in the model.
    importer = Importer()

    # Resolve the imports.
    importer.resolveImports(original_model, 'resources/')

    # Check for issues.
    print('The importer found {} issues.'.format(importer.issueCount()))
    for i in range(0,importer.issueCount()):
        print(importer.issue(i).description())
    print()

    # STEP 3
    # The analysis tools - the Validator and Analyser - will read only the submitted
    # model they do not look into any of the imported items, so they can't check them.
    # In order to retain the import structure but be able to use the diagnostic tools, 
    # we can create a flattened copy of the model for testing.  This can be used to
    # identify mistakes in the unflattened model too.  

    # Create a Validator and Analyser and submit the original, unflattened model.
    # We don't expect either of these to report any issues.
    validator = Validator()
    validator.validateModel(original_model)

    print('Investigating the original model:')
    print(' - the validator found {} issues'.format(validator.issueCount()))
    for i in range(0, validator.issueCount()):
        print('    - {}'.format(validator.issue(i).description()))
    
    analyser = Analyser()
    analyser.analyseModel(original_model)
    print(' - the analyser found {} issues'.format(analyser.issueCount()))
    for i in range(0, analyser.issueCount()):
        print('    - {}'.format(analyser.issue(i).description()))
    print()

    # Create a flattened version for diagnostics.
    flat_model = importer.flattenModel(original_model)

    # Repeat the validation and analysis above on the flattened model.
    validator.validateModel(flat_model)
    print('Investigating the flattened model:')
    print(' - the validator found {} issues'.format(validator.issueCount()))
    for i in range(0, validator.issueCount()):
        print('    - {}'.format(validator.issue(i).description()))
    
    analyser.analyseModel(original_model)
    print(' - the analyser found {} issues'.format(analyser.issueCount()))
    for i in range(0, analyser.issueCount()):
        print('    - {}'.format(analyser.issue(i).description()))
    print()

    # STEP 4 
    # The issues reported boil down to just one issue really: that there is a 
    # variable named 'i_need_units' that requires units to be defined.  
    # Because of the import structure, this could be hidden inside the importing
    # hierarchy.  We can use a recursive function to print information on the imported
    # items within the unflattened hierarchy. 
    print('The import dependency hierarchy is:')
    spacer = ''
    print_import_dependencies(original_model, spacer)
    print()

    # STEP 5
    # Fix the error in the imported file, and remake the flattened model for checking.
    # According to the printout above, we need to add units to the 'i_need_units'
    # variable, to be found inside the 'importExample3.cellml' file.
    # To fix this, we need to fix the model inside the 'importExample3.cellml' file.
    # When the original_model's imports were resolved, this model was added to the
    # library in the Importer.  We can retrieve the model from there for repair.
    print('The importer contains {} models:'.format(importer.libraryCount()))
    for i in range(0, importer.libraryCount()):
        print('  - model {}: {}'.format(i, importer.key(i)))
    print()

    # Retrieve from the library by key.
    imported_model = importer.library('resources/importExample3.cellml')

    # Add units to the variable that needs them.
    imported_model.component('shared').variable('i_need_units').setUnits('dimensionless')

    # Recreate the flattened model, and check it again.  This will use the updated model
    # in the importer library as its source.
    flat_model = importer.flattenModel(original_model)

    print('Testing the re-flattened model:')
    validator.validateModel(flat_model)
    print(' - the validator found {} issues'.format(validator.issueCount()))
    for i in range(0, validator.issueCount()):
        print('    - {}'.format(validator.issue(i).description()))
    
    analyser.analyseModel(original_model)
    print(' - the analyser found {} issues'.format(analyser.issueCount()))
    for i in range(0, analyser.issueCount()):
        print('    - {}'.format(analyser.issue(i).description()))
    print()

    # STEP 6
    # Print the collection of repaired import models to files.
    # NOTE that this file should be named 'importExample3.cellml' in order
    # for the other files to find it.  It is not named that here so that this
    # example code can be used more than once with the same outputs. 
    printer = Printer()
    model_string = printer.printModel(imported_model)

    # Write the serialised string to a file.
    write_file = open("importExample3_repaired.cellml", "w")
    write_file.write(model_string)

    print("The corrected model has been written to 'importExample3_repaired.cellml'.")
    print()
    # END
