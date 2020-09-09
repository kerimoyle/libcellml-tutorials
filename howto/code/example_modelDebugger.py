from libcellml import Analyser, Importer, Model, Parser, Printer, Validator

if __name__ == '__main__':
    # STEP 1
    # Parse an existing CellML model from a file.
    read_file = open('debugAnalysisExample.cellml', 'r')
    parser = Parser()
    model = parser.parseModel(read_file.read())

    # STEP 2
    # Resolve any imports and flatten the model for analysis.
    importer = Importer()

    # Resolve the imports.
    importer.resolveImports(model, '')

    # Check for issues.
    print('The importer found {} issues.'.format(importer.issueCount()))
    for i in range(0, importer.issueCount()):
        issue = importer.issue(i)
        print(issue.description())

    # Flatten the model.
    model = importer.flattenModel(model)

    # STEP 3
    # Create an Validator instance and pass the model to it for processing.
    validator = Validator()
    validator.validateModel(model)

    # Print any issues to the terminal.
    print('The validator found {} issues.'.format(validator.issueCount()))
    for i in range(0, validator.issueCount()):
        issue = validator.issue(i)
        print(issue.description())

    # STEP 4
    # Fix the validation errors.

    # Add units to the variable 'b' in component 'validationErrors'.
    model.component('validationErrors').variable('b').setUnits('dimensionless')

    # Change the name of the variable 'iShouldBeNamed_c' to be 'c'.
    model.component('validationErrors').variable('iShouldBeNamed_c').setName('c')

    # Check again.
    validator.validateModel(model)
    print('The validator found {} issues.'.format(validator.issueCount()))

    # STEP 5
    # The Analyser will find errors in the logic or mathematical formulation of the model's equations,
    # so may return issues even when the model is valid (syntactically correct).
    analyser = Analyser()
    analyser.analyseModel(model)

    print('The analyser found {} issues.'.format(analyser.issueCount()))
    for i in range(0, analyser.issueCount()):
        issue = analyser.issue(i)
        print(issue.description())

    # STEP 6
    # Fix the analysis errors.  This may need several iterations of checking before all errors have been 
    # resolved.

    # Variable 'd' in component 'importedComponent' is initialised using variable 'e', but it is not a constant.
    model.component('importedComponent').variable('d').setInitialValue(22)

    analyser.analyseModel(model)

    print('The analyser found {} issues.'.format(analyser.issueCount()))
    for i in range(0, analyser.issueCount()):
        issue = analyser.issue(i)
        print(issue.description())

    # Variable 'x' in component 'nonInitialisedStateVariable' is used in an ODE, but it is not initialised.
    model.component('nonInitialisedStateVariable', True).variable('x').setInitialValue(0.0)

    # Variable 'x' in component 'overconstrainedModel' is computed more than once.
    mathml = \
        '<math xmlns="http://www.w3.org/1998/Math/MathML" xmlns:cellml="http://www.cellml.org/cellml/2.0#">\n'\
        '  <apply>\n'\
        '    <eq/>\n'\
        '   <ci>x</ci>\n'\
        '    <cn cellml:units="dimensionless">3</cn>\n'\
        '  </apply>\n'\
        '</math>'
    model.component('overconstrainedModel', True).setMath(mathml)

    # Variable 'x' in component 'uncomputedVariable' is not computed.
    model.component('uncomputedVariable').setMath(mathml)

    # Variable 'a' in component 'validationErrors' is not computed.
    # Variable 'c' in component 'validationErrors' is not computed.
    model.component('validationErrors').variable('c').setInitialValue(1.0)

    # Check again.
    validator.validateModel(model)
    print('The validator found {} issues.'.format(validator.issueCount()))
    for i in range(0, validator.issueCount()):
        issue = validator.issue(i)
        print(issue.description())

    analyser.analyseModel(model)
    print('The analyser found {} issues.'.format(analyser.issueCount()))
    for i in range(0, analyser.issueCount()):
        issue = analyser.issue(i)
        print(issue.description())

    # STEP 7
    # Write the flattened, validated, analysed model to a serialised CellML string.
    printer = Printer()
    model_string = printer.printModel(model)

    # Write the serialised string to a file.
    write_file = open("debugAnalysisExampleFixed.cellml", "w")
    write_file.write(model_string)

    # END

