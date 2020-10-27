"""
 COMBINE2020 libCellML TUTORIAL 3: Debugging the sodium channel model
 
 By the time you have worked through this part of the tutorial you will be able to:
  - Parse an existing CellML file and deserialise it into a model instance
  - Use the diagnostic Validator class to identify issues in the model's definition 
  - Use the std.any_cast to retrieve items which need repair from validator Issue items
  - Use the Importer class to resolve imports and identify issues and
  - Use the diagnostic Analyser class to identify issues in the model's mathematical formulation.
"""

from libcellml import Analyser, Component, Importer, CellmlElementType, Model, Parser, Printer, Units, Validator, Variable

from tutorial_utilities import print_issues, print_model, get_cellml_element_type_from_enum, get_issue_level_from_enum, print_encapsulation

if __name__ == '__main__':


    print('----------------------------------------------------------')
    print('   STEP 1: Parse the existing sodium channel model        ')
    print('----------------------------------------------------------')

    #         The Parser class is used to deserialise a CellML string into a Model instance.
    #         This means that you're responsible for finding, opening and reading the.cellml 
    #         file into a single string.  The Parser will then read that string and return a model.

    #  1.a 
    #      Read a CellML file into a string.
    read_file = open("sodiumChannelModel_broken.cellml")

    #  1.b 
    #      Create a Parser item. 
    parser = Parser()

    #  1.c 
    #      Use the parser to deserialise the contents of the string you've read and return the model.
    model = parser.parseModel(read_file.read())

    #  1.d 
    #      Print the parsed model to the terminal for viewing.
    print_model(model, False)

    #  end 1

    print('----------------------------------------------------------')
    print('   STEP 2: Validate the parsed model                      ')
    print('----------------------------------------------------------')

    #  2.a 
    #      Create a Validator item and validate the model.
    validator = Validator()
    validator.validateModel(model)

    #  end 2.a

    #      Each Validator issue contains:
    #          - a description of the problem
    #          - the reference heading in the normative specification which affects this issue
    #          - a URL at which the informative specification notes can be found
    #          - an std.any item storing the CellML element most relevant to the issue and
    #          - a level indication.  

    #  2.b
    #      Retrieve any issues from the validator and print their descriptions and help URL to the terminal.
    #      If you're already familiar with these you can use the print_issues function instead.
    print('The validator found {} issues.'.format(validator.issueCount()))
    for i in range(0, validator.issueCount()):
        issue = validator.issue(i)
        print('Issue {}: {}'.format(i,issue.description()))
        print('  reference: {}'.format(issue.referenceHeading()))
        print('  see: {}'.format(issue.url()))
        print('  stored item type: {}'.format(get_cellml_element_type_from_enum(issue.cellmlElementType())))
        print()

    #  end 2

    print('----------------------------------------------------------')
    print('   STEP 3: Repair the parsed model                        ')
    print('----------------------------------------------------------')

    #      The messages returned from the validator (and other classes) should (!) have enough information 
    #      to enable you to know what the problem is.  In the case of the validator class, the URL listed
    #      contains additional resources and examples related to the issue, as well as its section in the
    #      CellML normative specification.

    #      In some situations more than one Issue will be generated from a single cause: this is the case
    #      with issues 0 and 1 here:

    # Issue 0: CellML identifiers must not contain any characters other than [a-zA-Z0-9_].
    #   reference: 1.3.1.1
    #   see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specA03.html?issue=1.3.1.1
    # Issue 1: Component 'mGateEquations!' does not have a valid name attribute.
    #   reference: 2.7.1
    #   see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB07.html?issue=2.7.1

    #  3.a
    #      Fetch the component using its name, and set the name to something valid.  Note that when finding
    #      a Component item, setting an optional second argument to `True` will search the entire encapsulation
    #      hierarchy for a component with that name, and not only the direct children of the model.
    #      You can follow the URL for information about what makes a valid name, and use the Component.setName
    #      function to fix it.
    model.component('mGateEquations!', True).setName('mGateEquations')

    #  end 3.a

    #  The messages below indicate that we're missing a Units item named 'mS_per_cm2'. 

    # Issue 2: Variable 'Na_conductance' in component 'sodiumChannelEquations' has a units reference 'mS_per_cm2' which is neither standard nor defined in the parent model.
    #   reference: 2.8.1.2
    #   see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    # Issue 3: Variable 'g_Na' in component 'sodiumChannelEquations' has a units reference 'mS_per_cm2' which is neither standard nor defined in the parent model.
    #   reference: 2.8.1.2
    #   see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    # Issue 4: Variable 'g_Na' in component 'sodiumChannelParameters' has a units reference 'mS_per_cm2' which is neither standard nor defined in the parent model.
    #   reference: 2.8.1.2
    #   see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2

    #  3.b
    #      Create an appropriate Units item (note that S stands for 'siemens'), and add it to your model.
    mS_per_cm2 = Units('mS_per_cm2')
    mS_per_cm2.addUnit('siemens', 'milli')
    mS_per_cm2.addUnit('metre', 'centi', -2)
    model.addUnits(mS_per_cm2)

    #  end 3.b

    #      As with 3.a, here we have more than one issue generated from the same cause: in this case, 
    #      we haven't specified units for a variable.
    
    # Issue 5: CellML identifiers must contain one or more basic Latin alphabetic characters.
    #   reference: 1.3.1.1
    #   see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specA03.html?issue=1.3.1.1
    # Issue 6: Variable 'V' in component 'sodiumChannel' does not have a valid units attribute. The attribute given is ''.
    #   reference: 2.8.1.2
    #   see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB08.html?issue=2.8.1.2
    #  Issue 8: Variable 'V' in component 'sodiumChannel' has units of '' and an equivalent variable 'V' in component 'sodiumChannelEquations' with non-matching units of 'mV'. The mismatch is: 
    #   reference: 
    #   see: 
    
    #  Each issue generated contains a pointer to the item to which it refers. We can retrieve the affected item 
    #  directly from the issue in one of two ways:
    #      - retrieving an AnyItem pair (whose 'first' attribute is an enum of the CellmlElementType 
    #        and 'second' attribute is the item itself), or
    #      - since we know that the type of item in this error is a CellmlElementType.VARIABLE, we can call the 
    #        convenience method Issue.variable() to return the variable which needs attention.
    #  (Of course you could retrieve it using the name of its parent component and its name too - this is just another way!)
    
    #  3.c
    #      Check that the item to be returned from the issue is in fact an CellmlElementType.VARIABLE by calling the Issue.type()
    #      function.  Retrieve the variable missing units from the issue.  Set its units to be millivolts.
    issue6 = validator.issue(6)
    print('Issue 6 is a {}'.format(get_cellml_element_type_from_enum(issue6.cellmlElementType())))
    issue6.variable().setUnits(model.units('mV'))

    #  end 3.c

    #  The error below indicates that a child Unit references something which can't be found.  
    
    #  Issue 7: Units reference 'i_dont_exist' in units 'mV' is not a valid reference to a local units or a standard unit type.
    #      reference: 2.6.1
    #      see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specB06.html?issue=2.6.1
    
    #  You have a few different options for how to fix this one. 
    #  1) The manual way.  The issue description tells us the name of the Units where the issue is, and the
    #     name of the units required by one of its children.  We can locate both items using these names,
    #     remove the problem child and replace it with a correct one.  
    #  2) The pointer way. As with the example in 3.c, we can retrieve a structure representing the
    #     Unit child directly from the issue.  Since (as above) we know it will be a Unit item, we can
    #     call the issue.unit() function to retrieve it.  The Unit item consists of a pointer to its
    #     parent Units item, and the index of the relevant child.  
    #  3) The roundabout option. Since the error is saying that Units named 'i_dont_exist' are missing, 
    #     we could simply provide them by creating a Units item and adding it to the model.  
    #  
    #  3.d
    #      Choose your preferred method and use it to retrieve the problem unit attributes and print them all to
    #      to the terminal.  Then fix the issue.
    prefix = ''
    id = ''
    exponent = 0.0
    multiplier = 0.0
    mV = model.units('mV')
    # prefix, exponent, multiplier, id = mV.unitAttributes('i_dont_exist')
    print(mV.unitAttributes('i_dont_exist'))
    print('The units \'mV\' child has attributes: base units = \'i_dont_exist\', prefix = \'{p}\', exponent = \'{e}\', and multiplier = \'{m}\'.'.format(p=prefix, e=exponent, m=multiplier))

    # Method 1:
    # mV.removeUnit('i_dont_exist')
    # mV.addUnit('volt', 'milli')

    # Method 2:
    issue7 = validator.issue(7)
    print('Issue 7 is a {}'.format(get_cellml_element_type_from_enum(issue7.cellmlElementType())))
    issue7_units = issue7.unit().units()
    issue7_units.removeUnit(issue7.unit().index())
    issue7_units.addUnit('volt', 'milli')

    # Method 3:
    # missing_units = Units('i_dont_exist')
    # missing_units.addUnit('volt', 'milli')
    # model.addUnits(missing_units) 

    #  end 3.d

    #  The final validator issue refers to the fact that we need to explicitly specify how other components
    #  can access each of the variables in this component.
    
    # Issue 9: Variable 't' in component 'sodiumChannelEquations' has no interface type set. The interface type required is 'public_and_private'.
    #   reference: 3.10.8
    #   see: https://cellml-specification.readthedocs.io/en/latest/reference/formal_and_informative/specC10.html?issue=3.10.8
    
    #  3.e
    #      Retrieve the variable either using the issue pointer method, or using the name method, and set its 
    #      interface to be the required type.
    issue9 = validator.issue(9)
    issue9.variable().setInterfaceType('public_and_private')

    #  3.f 
    #      Revalidate the model and confirm that the errors have gone.
    validator.validateModel(model)
    print_issues(validator)

    #  3.g
    #      Even though the model is free from validation errors, we still need to make sure it
    #      represents what we want it to.  Print the model to the terminal and check its structure.
    print_model(model)

    #  3.h 
    #      Use the addComponent functions to rearrange the components as needed until you have the
    #      required structure.  Validate the model again.             
    imported_gate_m = model.component('importedGateM', True)
    m_gate_equations = model.component('mGateEquations', True)
    m_gate = model.component('mGate', True)
    m_gate_equations.addComponent(imported_gate_m)
    m_gate.addComponent(m_gate_equations)

    validator.validateModel(model)
    print_issues(validator)
    print_encapsulation(model)

    #  end 3

    print('----------------------------------------------------------')
    print('   STEP 4: Resolve the imports ')
    print('----------------------------------------------------------')

    #      It's important to remember that the imports are merely instructions for how
    #      components or units items should be located: only their syntax is checked by the
    #      validator, not that the files exist or contain the required information.  To debug
    #      the imported aspects of the model, we need to use an Importer class.

    #      To resolve the imports, we need a path to a base location against which any
    #      relative file addresses can be resolved.  For this tutorial, if you've downloaded 
    #      the files into the same directory as the code, so simply using an empty string is sufficient.
    #      If they're another directory, make sure to end your path with a slash, '/'.
    #      Use the function importer.resolveImports(model, path).
    #  4.a 
    #      Create an Importer instance and use it to resolve the model.
    importer = Importer()
    importer.resolveImports(model, '')

    #  4.b 
    #      Similarly to the validator, the importer will log any issues it encounters.
    #      Retrieve these and print to the terminal (you can do this manually or using the
    #      convenience function as before).
    print_issues(importer)

    #  end 4.b
    #  Importer error[0]:
    #     Description: Import of component 'importedGateH' from 'GateModel.cellml' requires 
    #     component named 'i_dont_exist' which cannot be found.

    #     We need to change the import reference for the component to be 'gateEquations' instead
    #     of 'i_dont_exist'.  You can either retrieve the component using its name or directly
    #     from the issue.  Use the Component.setImportReference() function to fix the issue.
    #  4.c 
    #      Fix the issues reported by the importer.  This needs to be an iterative process as
    #      more files become available to the importer.
    issue0 = importer.issue(0)
    issue0.component().setImportReference('gateEquations')

    #  end 4.c
    #      The second issue reported is a circular dependency. This is contained in files that
    #      haven't (yet) been seen at all by you, the user.  It's included here to highlight the
    #      fact that the Importer class opens and instantates all required dependencies, and that
    #      some of those dependencies may have problems of their own.  

    #  Issue [1] is a WARNING:
    #     description: Cyclic dependencies were found when attempting to resolve components in model 'CircularReferences'. The dependency loop is:
    #      - component 'importedGateH' is imported from 'i_dont_exist' in 'GateModel.cellml'
    #      - component 'importedGateM' is imported from 'gateEquations' in 'GateModel.cellml'
    #      - component 'controller' is imported from 'controller' in 'CircularControllerReference.cellml'
    #      - component 'controller' is imported from 'controller2' in 'CircularControllerReference2.cellml'
    #      - component 'controller2' is imported from 'controller' in 'CircularControllerReference.cellml' and
    #      - component 'controller' is imported from 'controller2' in 'CircularControllerReference2.cellml'.
    #     stored item type: UNDEFINED
    #   
    #      To fix this, we have two options: 
    #          - to open and repair the file which is actually broken, or
    #          - to switch the import source in this current model to one which doesn't have circular imports.
    #  4.d
    #      In this example we can change the import of the controller component to have url of
    #      'SodiumChannelController.cellml'.
    model.component('controller', True).importSource().setUrl('SodiumChannelController.cellml')

    #  4.e 
    #      Clear all issues from the importer using the removeAllIssues() function.
    #      Resolve the imports again and check that there are no further issues.
    importer.removeAllIssues()
    importer.resolveImports(model, '')
    print_issues(importer)

    #  end 4

    print('----------------------------------------------------------')
    print('   STEP 5: Validate the imported dependencies ')
    print('----------------------------------------------------------')

    #      At this stage we've validated the local model, and we've used the Importer class
    #      to retrieve all of its import dependencies.  These dependencies are stored in
    #      the importer's library, and have not yet been validated or analysed.  
    #      Useful functions:
    #          - Importer.libraryCount() returns the number of stored models
    #          - Importer.library(index) returns the model at the given index
    #          - Importer.key(index) returns a key string that could be used to retrieve the model too
    #          - Importer.library(keystring) returns the model at the given key.

    #  5.a 
    #      Use a simple loop to validate each of the models stored in the importer's library.
    for i in range(0, importer.libraryCount()):
        print('Imported model at key: {}'.format(importer.key(i)))
        validator.validateModel(importer.library(i))
        print_issues(validator)
    
    #  end 5.a

    #  Note that the two files creating the circular import in 4.a are still in the
    #  library.

    #  To limit ourselves to only those models which are still relevant as the import
    #  dependencies of our repaired model, we can iterate through our model's ImportSource
    #  items instead.  As soon as the model's imports have been resolved, all these will 
    #  point to instantiated models within the importer.
    #  Useful functions:
    #          - Model.importSourceCount()
    #          - Model.importSource(size_t index) and
    #          - ImportSource.model()
    #          - ImportSource.url()
    #  5.b
    #      Loop through the model's import source items and print their urls to the terminal.
    #      You'll notice that these have been used as the keys in the importer library.
    #      Check that the importer library's models are the same as that attached to the
    #      import source item.
    for i in range(0, model.importSourceCount()):
        print('Import source [{}]:'.format(i))
        print('     url = {}'.format(model.importSource(i).url()))
        print('     model = {}'.format(model.importSource(i).model()))
        print('     library[url] = {}'.format(importer.library(model.importSource(i).url())))

    #  end 5

    print('----------------------------------------------------------')
    print('   STEP 6: Analyse the model(s) ')
    print('----------------------------------------------------------')

    #      As with the validator, the Analyser class is a diagnostic class which will check
    #      whether the mathematical representation is ready for simulation.  This involves
    #      making sure that variables are contained in equations, that integrated variables
    #      have initial conditions, and that there are no over- or under-constrained sets 
    #      of equations.
    #      Since this model uses imports, the real mathematical model is hidden from the 
    #      Analyser (just as it was from the validator).  The way around this is to 
    #      use the Importer class to create a flat (ie: import-free) version of the same
    #      model.  If the flat model meets the analyser's checks, then the importing version
    #      will too.

    #  6.a 
    #      Create an Analyser instance and pass in the model for analysis.
    #      Useful functions: Analyser.analyseModel(Model)
    analyser = Analyser()
    analyser.analyseModel(model)

    #  6.b 
    #      Retrieve and print the issues from the analysis to the screen.  We expect to see 
    #      messages related to un-computed variables, since anything which is imported is 
    #      missing from this model.
    print_issues(analyser)

    print()
    #  6.c 
    #      Create a flattened version of the model print it to the screen.
    #      Notice that any comments indicating that a component was an import have been
    #      removed as these components have been instantiated in the flattened model.
    #      Useful functions:
    #          - Importer.flattenModel(Model) will return a flattened copy.
    flat_model = importer.flattenModel(model)
    print_model(flat_model)

    #  6.d 
    #      Analyse the flattened model and print the issues to the screen.
    analyser.analyseModel(flat_model)
    print_issues(analyser)

    #  end 6.d

    #      The issue returned from the analyser says that we're trying to use two different variables
    #      as the base variable of integration, and the CellML code generation facility (which the analyser
    #      is tied to) does not support this yet. It's still valid CellML though! In this example, the real
    #      problem is that these two variables are talking about the same thing, but haven't been connected
    #      to one another yet.
    
    #   Issue [0] is an ERROR:
    #      description: Variable 't' in component 'importedGateM' and variable 't' in component
    #                  'importedGateH' cannot both be the variable of integration.
    #      stored item type: VARIABLE
    
    #  6.e 
    #      Create any necessary variable equivalences so that these two variables are connected. You
    #      can refer to your printout of the model's structure to help if need be, and remember that only
    #      variables in a sibling or parent/child relationship can be connected.
    #      Useful function: Variable.addEquivalence(v1, v2) will create an equivalence between the 
    #                       variables v1 and v2.

    Variable.addEquivalence(model.component('importedGateM', True).variable('t'), 
                                        model.component('mGateEquations', True).variable('t'))
    Variable.addEquivalence(model.component('mGate', True).variable('t'), 
                                        model.component('mGateEquations', True).variable('t'))

    #  6.f Re-flatten and re-analyse the model and print the issues to the terminal.
    analyser.analyseModel(importer.flattenModel(model))
    print_issues(analyser)

    #  end 6.f
    #      Now we see the importance of checking iteratively for issues in the analyser class!  
    #      The nature of this class means that frequently it is unable to continue processing
    #      when an issue is encountered.  It's not unusual to fix one issue only to find twenty more!
    #      Two of the errors reported deal with non-initialised variables.  Looking at the model
    #      printout we can see that this is because the integrated variable X (in both the imported
    #      gates) hasn't been connected to its local variable h or m in the appropriate 'parameters'
    #      component.
    #  6.g
    #      Create all required connections needed to connect these variables.
    #      Re-flatten, re-analyse and print the issues to the terminal.
    Variable.addEquivalence(model.component('importedGateM', True).variable('X'), 
                                        model.component('mGateEquations', True).variable('m'))
    Variable.addEquivalence(model.component('mGateParameters', True).variable('m'), 
                                        model.component('mGateEquations', True).variable('m'))
    Variable.addEquivalence(model.component('importedGateH', True).variable('X'), 
                                        model.component('hGateEquations', True).variable('h'))
    Variable.addEquivalence(model.component('hGateParameters', True).variable('h'), 
                                        model.component('hGateEquations', True).variable('h'))                                    
    analyser.analyseModel(importer.flattenModel(model))
    print_issues(analyser)

    #  end 6.g 
    #      The nice thing about issues in this class is that frequently a few issues refer to the
    #      same single problem.  The remainder of the issues reported deal with variables that are
    #      not computed.  This could mean any one of:
    #      1- the variable is not included in any equations (it's completely unused)
    #      2- the variable is included in an equation, but the equation can't be evaluated 
    #        (contains some other uncomputed variable(s))
    #      3- the variable is a constant that should have a value assigned or
    #      4- the variable hasn't been connected to the rest of its definition (usually it's this one!).
    #      Because the 'is not computed' errors are cascading by nature, frequently fixing just one
    #      will resolve many others.  
    #      Hints: 
    #          - There is at least one of each kind of problem
    #          - There's a convenience function provided which will print the equivalent variable set
    #            for a given variable.  You can use the item stored by each issue and this function to
    #            check for missing connections: printEquivalentVariableSet(variable)
    #          - the addEquivalence function returns a boolean indicating success or otherwise.  If you
    #            check this as you go it will alert you quickly if you're trying to make an illegal 
    #            connection.
    #  6.h
    #      From the printout of your model and the issues listed, determine what needs to happen in 
    #      order to make the model viable, and do it.  Check that your final analysis contains no issues.

    # Connect the m_gate to its surroundings.
    Variable.addEquivalence(model.component('importedGateM', True).variable('alpha_X'), 
                                        model.component('mGateEquations', True).variable('alpha_m'))
    Variable.addEquivalence(model.component('importedGateM', True).variable('beta_X'), 
                                        model.component('mGateEquations', True).variable('beta_m'))
    Variable.addEquivalence(model.component('mGate', True).variable('V'), 
                                        model.component('mGateEquations', True).variable('V'))
    Variable.addEquivalence(model.component('mGate', True).variable('m'), 
                                        model.component('mGateEquations', True).variable('m'))

    # E_Na in sodiumChannelParameters needs to be initialised to 40.
    model.component('sodiumChannelParameters', True).variable('E_Na').setInitialValue(40)

    # i_am_redundant in m_gateParameters is not required.
    model.component('mGateParameters', True).removeVariable('i_am_redundant')

    analyser.analyseModel(importer.flattenModel(model))
    print_issues(analyser)

    #  end 6

    print('----------------------------------------------------------')
    print('   STEP 7: Serialise and print the repaired model         ')
    print('----------------------------------------------------------')

    #  7.a 
    #      Create a Printer instance and use it to print the CellML-formatted version of
    #      the repaired model to a string.  Remember we'll still be printing the original
    #      version of the model, not the flattened one!
    printer = Printer()
    printed_model_string = printer.printModel(model)
    
    #  7.b 
    #      Write the string to a file named 'SodiumChannelModel.cellml'.
    write_file = open('SodiumChannelModel.cellml', 'w')
    write_file.write(printed_model_string)
    write_file.close()

    #  end

    print('The repaired sodium channel model has been written to SodiumChannelModel.cellml.')

