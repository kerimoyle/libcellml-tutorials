"""
    This program is intended to demonstrate a workflow typical of a simulation
    tool developer who would like to be able to interact with CellML model files.
    The general principles are:
    - parse a CellML model file;
    - resolve any import dependencies in the model;
    - validate the model (check for semantic/syntactic errors);
    - debug the model (check for modelling errors);
    - generate runnable code in C and/or Python; and
    - output in the desired format.
"""

from libcellml import Component, Variable

if __name__ == '__main__':

    # STEP 1
    # Parse the model from a CellML file.

    # Create a libCellML Parser, and use it to parse the fileContents
    # string and convert it into a CellML Model structure.
    read_file = open("resources/simulationExample.cellml", "r")
    parser = Parser()
    model = parser.parseModel(read_file.read())
    print_issues_to_terminal(parser)

    # STEP 2
    # Resolve any import dependencies (if present) in the model.

    if(model.hasUnresolvedImports()) {

        # Create an Importer instance.
        importer = Importer()

        # Submit the model to the importer and the absolute location 
        # against which the import reference paths will be resolved.
        importer.resolveModelImports(model, "resources/")
        print_issues_to_terminal(importer)

        # Print a list of sources that this model requires. This list will 
        # be empty after the model has been flattened.
        print_import_dependencies(model)

        # Retrieve a "flattened" (ie: import-free) model from the importer,
        # and use it to over-write the current model.
        model = importer.flattenModel(model)
    }

    # STEP 3
    # Validate the model: check for syntactic and semantic errors.

    # Create a Validator instance and pass the model for checking.
    validator = Validator()
    validator.validateModel(model)
    print_issues_to_terminal(validator)

    # STEP 4
    # Analyse a model: check for mathematical and modelling errors.
    analyser = Analyser()
    analyser.analyseModel(model)
    print_issues_to_terminal(analyser)

    # STEP 5
    # Generate runnable code in other language formats for this model.

    # Create a Generator instance.  Note that by default this is the C language.
    generator = Generator()

    # Pass the generator the analysed model for processing.
    generator.processModel(analyser.model())
    print_issues_to_terminal(generator)

    # STEP 6
    # Retrieve and write the interface code (*.h) and implementation code (*.cpp) to files.
    write_file = open("sineComparisonExample.h", "w")
    write_file.write(generator.interfaceCode())
    write_file.close()

    write_file = open("sineComparisonExample.cpp", "w")
    write_file.write(generator.implementationCode())
    write_file.close()

    # If required, change the generator profile to Python and reprocess the model.
    profile = GeneratorProfile(GeneratorProfile.Profile.PYTHON)
    generator.setProfile(profile)
    generator.processModel(model)

    # Retrieve and write the implementation code (*.py) to a file.
    write_file = open("sineComparisonExample.py", "w")
    write_file.write(generator.implementationCode())
    write_file.close()

    # END
    
}

