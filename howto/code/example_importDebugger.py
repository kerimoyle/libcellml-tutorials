from libcellml import Analyser, Importer, Model, Parser, Printer, Validator

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
    
    analyser.analyseModel(flat_model)
    print(' - the analyser found {} issues'.format(analyser.issueCount()))
    for i in range(0, analyser.issueCount()):
        print('    - {}'.format(analyser.issue(i).description()))
    print()

    # STEP 4
    # The Validator and Analyser classes process only the contents of concrete items (ie: not the contents of 
    # imported items) of a model.
    # After successfully resolving a model's imports using an importer, the importer will store instances
    # of all of the dependencies of the resolved model.  These are accessible through the "library" function.
    # We can ascertain that all of import dependencies meet the diagnostic checks of the Validator and the 
    # Analyser individually by iterating through the importer's library.

    # Loop through the importer library and call the validator for each model.  
    for m in range(0, importer.libraryCount()):

        # Retrieve the library model by index, m.
        validator.validateModel(importer.library(m))

        # Retrieve the key under which it's stored: this will be the URL at which the imported model was found.
        print("The validator found {} issues in {}.".format(validator.issueCount(),importer.key(m)))
        for i in range(0, validator.issueCount()):
            print("    - {}".format(validator.issue(i).description()))

    print()

    # STEP 5
    # Fix the validation errors in the imported files.
    # According to the printout above, we need to add units to the "iNeedUnits"
    # variable, to be found inside the "importExample3.cellml" file.
    # To fix this, we need to fix the model inside the "importExample3.cellml" file.
    # When the original_model's imports were resolved, this model was added to the
    # library in the Importer.  We can retrieve the model from there for repair.

    # Retrieve from the library by key.  Note the inclusion of the directory used to
    # resolve imports for the original model is included in the key string.
    imported_model1 = importer.library('resources/importExample3.cellml')

    # Add units to the variable that needs them to fix the validation error.
    imported_model1.component('importThisOneToo').variable('iNeedUnits').setUnits('dimensionless')

    # Check that the issues have been fixed.
    validator.validateModel(imported_model1)
    print('\nInvestigating the repaired model:')
    print('The validator found {} issues.'.format(validator.issueCount()))

    for i in range(0, validator.issueCount()):
        print('    - {}'.format(validator.issue(i).description()))
    print()

    # STEP 6
    # Repeat steps 4 and 5 using the Analyser instead of the Validator.

    # Loop through the importer library and call the analyser for each model.  
    for m in range(0, importer.libraryCount()):

        # Retrieve the library model by index, m.
        analyser.analyseModel(importer.library(m))

        # Retrieve the key under which it's stored: this will be the URL at which the imported model was found.
        print('The analyser found {} issues in {}.'.format(analyser.issueCount(),importer.key(m)))
        for i in range(0, analyser.issueCount()):
            print('    - {}'.format(analyser.issue(i).description()))
    
    print()

    # Fix the error by adding a MathML block to the component named "concreteComponent" 
    # in the "importExample2b.cellml" model.
    imported_model2 = importer.library("resources/importExample2b.cellml")
    mathml = \
        '<math xmlns="http://www.w3.org/1998/Math/MathML" xmlns:cellml="http://www.cellml.org/cellml/2.0#">\n'\
        '  <apply>\n'\
        '    <eq/>\n'\
        '    <ci>iAmNotCalculated</ci>\n'\
        '    <cn cellml:units="dimensionless">3</cn>\n'\
        '  </apply>\n'\
        '</math>'
    imported_model2.component('concreteComponent').setMath(mathml)

    # Check that the issue has been fixed.
    analyser.analyseModel(imported_model2)
    print('\nInvestigating the repaired model:')
    print('The analyser found {} issues.'.format(analyser.issueCount()))
    for i in range(0,analyser.issueCount()):
        print('    - {}'.format(analyser.issue(i).description()))
    print()

    # Recreate the flattened model, and check it again.  This will use the updated model
    # in the importer library as its source.
    flat_model = importer.flattenModel(original_model)

    validator.validateModel(flat_model)
    print('\nInvestigating the flattened model:')
    print('The validator found {} issues.'.format(validator.issueCount()))
    for i in range(0, validator.issueCount()):
        print('    - '.format(validator.issue(i).description()))

    analyser.analyseModel(flat_model)
    print('The analyser found {} issues.'.format(analyser.issueCount()))
    for i in range(0, analyser.issueCount()):
        print('    - '.format(analyser.issue(i).description()))

    # STEP 7
    # Print the collection of repaired import models to files.
    # To avoid over-writing existing files, we'll write the corrected files to a separate
    # directory called "repaired/".  Note that the relationship between the files needs
    # to be maintained, so even files that have not been changed need to be written
    # to the new location.

    # Write the original model to a file.
    printer = Printer()
    model_string = printer.printModel(original_model)
    write_file = open("repaired/resources/importExample1.cellml", "w")
    write_file.write(model_string)
    
    # Write the dependency models in the importer library to files.
    for m in range(0, importer.libraryCount()):
        write_file = open("repaired/"+importer.key(m), "w")
        model_string = printer.printModel(importer.library(m))
        write_file.write(model_string)
    
    print('The corrected models has been written to the \'repaired/resources/\' directory') 

    # END
