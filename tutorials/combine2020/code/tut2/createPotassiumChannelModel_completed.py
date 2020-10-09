"""
 COMBINE2020 libCellML TUTORIAL 2: Creating a potassium channel model

  By the time you have worked through this tutorial you will be able to:
      - Assemble a multi-component model using the API 
      - Import items from existing models for reuse here
      - Inter-connect the components using the equivalent variables
        functionality
      - Use the diagnostic Validator class to identify errors in the
        model's syntax 
      - Use the diagnostic Analyser class to identify errors in the 
        model's mathematical formulation and
      - Serialise the model to CellML format for output.
"""

from libcellml import Analyser, Component, Importer, ImportSource, Model, Printer, Units, Validator, Variable

from tutorial_utilities import print_issues, print_model, get_item_type_from_enum, get_issue_level_from_enum

if __name__ == '__main__':

    #  Setup useful things.
    math_header = '<math xmlns="http://www.w3.org/1998/Math/MathML" xmlns:cellml="http://www.cellml.org/cellml/2.0#">\n'
    math_footer = '</math>'

    # Overall the model structure will be:
    # model: PotassiumChannelModel
    #     component: controller <-- imported from PotassiumChannelController.cellml
    #     component: potassiumChannel
    #         component: potassiumChannelEquations
    #                 component: n_gate_equations
    #                     component: gateEquations <-- imported from GateModel.cellml, component gateEquations
    #                 component: n_gate_parameters <-- created here so that the parameters are specific to the n_gate_equations.
    #         component: k_channel_parameters

    print('------------------------------------------------------------')
    print('   STEP 1: Define the model setup                           ')
    print('------------------------------------------------------------')

    #  1.a 
    #      Create a Model and name it appropriately.
    model = Model('PotassiumChannelModel')

    #  1.b 
    #      Create a wrapping component and name it 'potassiumChannel'.
    k_channel = Component('potassiumChannel')

    #  1.c 
    #      Add the component to the model.
    model.addComponent(k_channel)

    #  end 1

    print('------------------------------------------------------------')
    print('   STEP 2: Define the potassium channel equations component ')
    print('------------------------------------------------------------')

    #  2.a 
    #      Create a Component instance for the equations and name it 'potassiumChannelEquations'.  
    #      Add it to the wrapper component you created above.
    k_channel_equations = Component('potassiumChannelEquations')
    k_channel.addComponent(k_channel_equations)

    #  end 2.a
    #      The mathematics of a component is specified as a MathML 2 string (NB: higher versions 
    #      of MathML are not supported), and is added to the component using setMath() and 
    #      appendMath() functions.  
    #      Your string needs to contain the namespaces for MathML and for CellML: these have been
    #      provided for you in the math_header string above.

    #  2.b 
    #      Define the maths inside the potassiumChannelEquations component.
    equation_iK = \
        '  <apply><eq/>\n'\
        '    <ci>i_K</ci>\n'\
        '    <apply><times/>\n'\
        '       <apply><power/>\n'\
        '           <ci>n</ci>\n'\
        '           <cn cellml:units="dimensionless">4</cn>\n'\
        '       </apply>\n'\
        '       <ci>g_K</ci>\n'\
        '       <apply><minus/>\n'\
        '           <ci>V</ci>\n'\
        '           <ci>E_K</ci>\n'\
        '       </apply>\n'\
        '    </apply>\n'\
        '  </apply>\n'

    k_channel_equations.setMath(math_header)
    k_channel_equations.appendMath(equation_iK)
    k_channel_equations.appendMath(math_footer)
     
    #  2.c 
    #      Once the mathematics has been added to the component, and the component to the 
    #      model, we can make use of the diagnostic messages within the Validator class
    #      to tell us what else needs to be done.  
    #      Create a Validator instance, and pass it your model for processing using the 
    #      validateModel function.  
    validator = Validator()
    validator.validateModel(model)

    #  end 2.c

    #      Calling the validator does not return anything: we have to go looking for issues 
    #      that it found during processing.  When a problem is found, an Issue item is created
    #      containing:
    #          - a description string explaining the problem
    #          - a URL at which more information is available
    #          - an std.any item relevant to the problem, if available
    #          - a level indicator and
    #          - a cause indicator relevant to the stored item.
    #  We can use these issues as we need to.  The simplest way is to print the descriptions
    #  to the terminal.

    #  2.d
    #      Retrieve the number of issues encountered using the validator.issueCount() function,
    #      then retrieve the issue items from the validator using their index and the validator.issue(index)
    #      function.
    print('The validator has found {} issues.'.format(validator.issueCount()))
    for i in range(0, validator.issueCount()):
        print(validator.issue(i).description())
    print()

    #  2.e 
    #      Create the variables needed and add them to the potassium channel component.
    #      Revalidate and expect errors related to variables without units.
    k_channel_equations.addVariable(Variable('E_K'))
    k_channel_equations.addVariable(Variable('i_K'))
    k_channel_equations.addVariable(Variable('g_K'))
    k_channel_equations.addVariable(Variable('V'))
    k_channel_equations.addVariable(Variable('t'))
    k_channel_equations.addVariable(Variable('n'))

    validator.validateModel(model)
    print_issues(validator)

    #  2.f 
    #      Create the missing Units items and add them to the model. These are:
    #      - milli-volts
    #      - milli-seconds
    #      - milli-moles
    #      - micro-Amperes per square centimetre
    #      - milli-Siemens per square centimetre

    mV = Units('mV')
    mV.addUnit('volt', 'milli')

    microA_per_cm2 = Units('microA_per_cm2')
    microA_per_cm2.addUnit('ampere', 'micro')
    microA_per_cm2.addUnit('metre', 'centi', -2.0)

    mS_per_cm2 = Units('milliS_per_cm2')
    mS_per_cm2.addUnit('siemens', 'milli')
    mS_per_cm2.addUnit('metre', 'centi', -2.0)

    ms = Units('ms')
    ms.addUnit('second', 'milli')

    mM = Units('mM')
    mM.addUnit('mole', 'milli')

    model.addUnits(ms)
    model.addUnits(mV)
    model.addUnits(mM)
    model.addUnits(microA_per_cm2)
    model.addUnits(mS_per_cm2)

    #  2.g 
    #      Set the units on each of the variables.  
    #      Call the validator again, and expect there to be no errors.
    k_channel_equations.variable('E_K').setUnits(mV)
    k_channel_equations.variable('i_K').setUnits(microA_per_cm2)
    k_channel_equations.variable('g_K').setUnits(mS_per_cm2)
    k_channel_equations.variable('V').setUnits(mV)
    k_channel_equations.variable('t').setUnits(ms)
    k_channel_equations.variable('n').setUnits('dimensionless')

    validator.validateModel(model)
    print_issues(validator)

    #  end 2

    print('------------------------------------------------------------')
    print('   STEP 3: Create the n_gate and n_gate_equations components   ')
    print('------------------------------------------------------------')

    #  STEP 3: Create the n_gate and its child components:
    #          - The n_gate_equations has some of the working of a generic gate 
    #            (which we'll import from GateModel.cellml), but instead of constant values for alpha and beta, 
    #            we'll introduce a voltage dependence.
    #          - the n_gate_parameters component allows us to specify those parameters specific to the movement
    #            of potassium.

    #  3.a 
    #      Create a component, name it 'nGate', and add it to the equations component.
    n_gate = Component('nGate')
    k_channel_equations.addComponent(n_gate)

    #  3.b 
    #      Create a component, name it 'nGateEquations' and add it to the n_gate component.
    n_gate_equations = Component('nGateEquations')
    n_gate.addComponent(n_gate_equations)

    #  3.c 
    #      Add the mathematics to the n_gate_equations component and validate the model.
    #      Expect errors relating to missing variables.
    equation_alpha_n = \
        '  <apply><eq/>\n'\
        '    <ci>alpha_n</ci>\n'\
        '    <apply><divide/>\n'\
        '      <apply><times/>\n'\
        '        <cn cellml:units="per_mV_ms">0.01</cn>\n'\
        '        <apply><plus/>\n'\
        '          <ci>V</ci>\n'\
        '          <cn cellml:units="mV">10</cn>\n'\
        '        </apply>\n'\
        '      </apply>\n'\
        '      <apply><minus/>\n'\
        '        <apply><exp/>\n'\
        '          <apply><divide/>\n'\
        '            <apply><plus/>\n'\
        '              <ci>V</ci>\n'\
        '              <cn cellml:units="mV">10</cn>\n'\
        '            </apply>\n'\
        '            <cn cellml:units="mV">10</cn>\n'\
        '          </apply>\n'\
        '        </apply>\n'\
        '        <cn cellml:units="dimensionless">1</cn>\n'\
        '      </apply>\n'\
        '    </apply>\n'\
        '  </apply>\n'

    equation_beta_n = \
        '  <apply><eq/>\n'\
        '    <ci>beta_n</ci>\n'\
        '    <apply><times/>\n'\
        '      <cn cellml:units="per_ms">0.125</cn>\n'\
        '      <apply><exp/>\n'\
        '        <apply><divide/>\n'\
        '          <ci>V</ci>\n'\
        '          <cn cellml:units="mV">80</cn>\n'\
        '        </apply>\n'\
        '      </apply>\n'\
        '    </apply>\n'\
        '  </apply>\n'

    n_gate_equations.setMath(math_header)
    n_gate_equations.appendMath(equation_alpha_n)
    n_gate_equations.appendMath(equation_beta_n)
    n_gate_equations.appendMath(math_footer)
    
    validator.validateModel(model)
    print_issues(validator)

    #  3.d 
    #      Add the missing variables to the n_gate_equations component, and validate again.
    #      Expect errors relating to units missing from the variables.
    n_gate_equations.addVariable(Variable('t'))
    n_gate_equations.addVariable(Variable('V'))
    n_gate_equations.addVariable(Variable('alpha_n'))
    n_gate_equations.addVariable(Variable('beta_n'))
    n_gate_equations.addVariable(Variable('n'))
    
    validator.validateModel(model)
    print_issues(validator)

    #  end 3.d

    #  The only two Units which aren't available already are:
    #      - per millisecond 
    #      - per millivolt millisecond
    #      Remember that you'll need to give these names that are the same as those needed by the 
    #      variables.  In this case they are 'per_ms' and 'per_mV_ms'.

    #  3.e
    #      Create the missing units and add them to the model.  
    per_ms = Units('per_ms')
    per_ms.addUnit('second', 'milli', -1)
    model.addUnits(per_ms)

    per_mV_ms = Units('per_mV_ms')
    per_mV_ms.addUnit('second', 'milli', -1)
    per_mV_ms.addUnit('volt', 'milli', -1)
    model.addUnits(per_mV_ms)

    #  3.f
    #      Associate the correct units items with the variables which need them.
    #      Revalidate the model, expecting there to be no errors reported.
    n_gate_equations.variable('t').setUnits(ms)
    n_gate_equations.variable('V').setUnits(mV)
    n_gate_equations.variable('alpha_n').setUnits(per_ms)
    n_gate_equations.variable('beta_n').setUnits(per_ms)
    n_gate_equations.variable('n').setUnits('dimensionless')

    validator.validateModel(model)
    print_issues(validator)

    #  end 3

    print('------------------------------------------------------------')
    print('   STEP 4: Specify imports for the generic gate component   ')
    print('------------------------------------------------------------')

    # STEP 4: Import the generic gate's equations component.
    #      The generic gate model (in GateModel.cellml) has two components: 
    #          - 'gateEquations' which solves an ODE for the gate status parameter, X
    #          - 'gateParameters' which sets the values of alpha, beta, and initialises X
    #      We will import only the 'gateEquations' component and set it to be a child 
    #      of the n_gate_equations component.  This means we can introduce the voltage
    #      dependence for the alpha and beta, and using a specified initial value for 
    #      the gate's status.  Note that the variable 'n' in the n_gate_equations is 
    #      equivalent to the generic gate's variable 'X'.

    #  Imports require three things:
    #      - a destination for the imported item. This could be a Component or Units item.
    #      - a model to import for the imported item from.  This is stored in an ImportSource
    #        item containing the URL of the model to read.
    #      - the name of the item to import.  This is called the 'import reference' and 
    #        is stored by the destination item.

    #  4.a 
    #      Create an ImportSource item and set its URL to be 'GateModel.cellml'.
    gate_import_source = ImportSource()
    gate_import_source.setUrl('GateModel.cellml')

    #  4.b 
    #      Create a destination component for the imported gate component, and add this to 
    #      the n_gate_equations component. 
    imported_gate = Component('importedGate')
    n_gate_equations.addComponent(imported_gate)

    #  4.c 
    #      Set the import reference on the component you just created to be the name
    #      of the component in the GateModel.cellml file that you want to use.  In this
    #      example, it is 'gateEquations'.
    imported_gate.setImportReference('gateEquations')

    #  4.d 
    #      Associate the import source with the component using the setImportSource function.
    #      Note that this step also makes the import source available to other items through the 
    #      Model.importSource(index) function.  This way the same model file can be used as a 
    #      source for more than one item.
    imported_gate.setImportSource(gate_import_source)

    #  4.e 
    #      Validate the model and confirm that there are no issues.
    validator.validateModel(model)
    print_issues(validator)

    #  end 4

    print('------------------------------------------------------------')
    print('   STEP 5: Specify imports for the controller component     ')
    print('------------------------------------------------------------')

    #  STEP 5: Repeat Step 4 to import a controller component.  This should be 
    #          at the top of the encapsulation hierarchy, and should import the component
    #          named 'controller' from the file 'PotassiumChannelController.cellml'.

    #  5.a 
    #      Repeat steps 4.a-d for the controller component.  Put it at the top level of
    #      the encapsulation hierarchy.
    controllerImportSource = ImportSource()
    controllerImportSource.setUrl('PotassiumChannelController.cellml')

    controller = Component('controller')
    controller.setImportReference('controller')
    controller.setImportSource(controllerImportSource)
    model.addComponent(controller)

    #  5.b Validate the model and confirm that there are no issues.
    validator.validateModel(model)
    print_issues(validator)

    #  end 5

    #  At this point we've defined the equations that govern the potassium channel's operation.
    #  From here on, our goal is to make sure that the CellML representation of these equations
    #  is valid (using the Validator) and solvable (using the Analyser).
    
    print('------------------------------------------------------------')
    print('   STEP 6: Analyse the model                                ')
    print('------------------------------------------------------------')

    # STEP 6: We will introduce the Analyser class here so that its use as a debugging 
    #         tool can be demonstrated.  Of course, we know ahead of time that there
    #         is still a lot of connections to be created between the components, but
    #         an analyser can help us to find them.

    # A reminder: we're aiming for a potassium channel component which can accept two external
    # parameters - time, t (ms) and voltage, V (mV) - and use them to calculate a potassium 
    # current, i_K (microA_per_cm2). 
    # A utility function print_model has been provided to help you to see what's going 
    # on inside your model.

    #  6.a 
    #      Create an Analyser item and pass it the model for checking with the analyseModel function.
    analyser = Analyser()
    analyser.analyseModel(model)

    #  6.b 
    #      The analyser is similar to the Validator and keeps a record of issues it encounters.
    #      Retrieve these and print to the terminal, just as you've done for the validator.
    #      Expect messages related to un-computed variables.
    print_issues(analyser)

    #  end 6

    #  Even though all of the messages we see are 'variable not calculated' errors, we can divide
    #  them into different categories:
    #  - those variables which are constants whose value has not been set yet
    #  - those variables whose calculation depends on as-yet un-calculated variables
    #  - those variables which need to be connected to where their calculation happens and
    #  - those variables which aren't present in any equation.

    print('------------------------------------------------------------')
    print('   STEP 7: Define the constants                             ')
    print('------------------------------------------------------------')

    #      Use the print_model() function to show your current model contents. This should
    #      show that we have currently got variables only in the n_gate_equations and potassiumChannelEquations
    #      components.  These need to have sibling parameters components created to hold any hard-coded
    #      values or initial conditions that are required.
    #  7.a 
    #      Print the model to the terminal.
    print_model(model, True)

    #  end 7.a 
    #      Create parameters siblings components for the equations components, and add the variables that 
    #      they will require.  These are:
    #      - potassium channel parameters
    #          - E_K (-87)
    #          - g_K (36)
    #      - n_gate parameters
    #          - initial value for n (dimensionless)
    #      You can either do this by creating the variables from scratch (as in Step 3.d) but
    #      because these are intended to be duplicates of existing variables, but in another 
    #      component, we can simply add a cloned variable to the parameters component.
    #  7.b
    #      Create parameters components for the equations components, and add cloned versions of
    #      any variables which need to be given a value into the new parameters components.
    k_channel_parameters = Component('potassiumChannelParameters')
    k_channel.addComponent(k_channel_parameters)
    k_channel_parameters.addVariable(k_channel_equations.variable('E_K').clone())
    k_channel_parameters.addVariable(k_channel_equations.variable('g_K').clone())

    n_gate_parameters = Component('nGateParameters')
    n_gate.addComponent(n_gate_parameters)
    n_gate_parameters.addVariable(n_gate_equations.variable('n').clone())

    #  7.c 
    #      In order for other encapsulating components to access these variables, they also need to have
    #      intermediate variables in the n_gate or potassium channel components too.  This is only true
    #      of variables that you want to be available to the outside.  In this example, we need to add
    #      the variable 'n' to the n_gate in order that its parent (the potassium channel equations) can 
    #      access it.
    n_gate.addVariable(n_gate_equations.variable('n').clone())

    #  7.d 
    #      Create variable connections between these variables and their counterparts in the equations
    #      components.  Validate, expecting errors related to missing or incorrect interface types.
    Variable.addEquivalence(k_channel_parameters.variable('E_K'), k_channel_equations.variable('E_K'))
    Variable.addEquivalence(k_channel_parameters.variable('g_K'), k_channel_equations.variable('g_K'))
    Variable.addEquivalence(n_gate.variable('n'), n_gate_equations.variable('n'))

    validator.validateModel(model)
    print_issues(validator)

    #  7.e 
    #      Set the required interface types as listed by the validator.  This can be done individually using the 
    #      Variable.setInterfaceType() function, or automatically using the Model.fixVariableInterfaces()
    #      function.  Validate again, expecting no validation errors.
    model.fixVariableInterfaces()

    validator.validateModel(model)
    print_issues(validator)
    
    #  end 7.e 
    #      If we were to analyse the model again now we would we still have the same set of errors 
    #      as earlier as we haven't given a value to any of our parameters.  We can use the 
    #      Variable.setInitialValue() function to give these values to the following variables 
    #      in the parameters components:
    #      - potassium channel parameters:
    #          - E_K = -87 [mV]
    #          - g_K = 36 [milliS_per_cm2]
    #      - n_gate parameters
    #          - n = 0.325 [dimensionless]
    #  7.f
    #      Set the constant values on the variables.  Analyse the model again, expecting 
    #      that the calculation errors related to these constants have been solved.
    k_channel_parameters.variable('E_K').setInitialValue(-87)
    k_channel_parameters.variable('g_K').setInitialValue(36)
    n_gate_parameters.variable('n').setInitialValue(0.325)

    analyser.analyseModel(model)
    print_issues(analyser)

    #  end 7

    print('------------------------------------------------------------')
    print('   STEP 8: Connect the input variables                      ')
    print('------------------------------------------------------------')

    # STEP 8: Looking at the variables listed we can see that some of our 'external' or 'input'
    #  variables are listed more than once.  These are the voltage, V, and time, t.  Time
    #  is needed in every equations component, including the imported gate component.
    #  Voltage is needed by the potassium channel and n_gate equations components.

    #      Printing the model to the terminal we'll notice the components which contain V and t 
    #      variables.  Connections between the variables in any two components are only possible
    #      when those components are in a sibling-sibling or parent-child relationship.  We can see
    #      from the printed structure that the top-level potassiumChannel component doesn't have any 
    #      variables, and neither does the n_gate component.  We'll need to create intermediate
    #      variables in those components to allow connections to be made through them.  
    #  8.a
    #      Use the print_model function to print your model to the terminal.
    print_model(model)

    #  8.b 
    #      Create dummy variables for time and voltage using the cloning technique described in 
    #      Step 7.b, and add a clone to each appropriate component.
    k_channel.addVariable(k_channel_equations.variable('t').clone())
    k_channel.addVariable(k_channel_equations.variable('V').clone())
    n_gate.addVariable(k_channel_equations.variable('t').clone())
    n_gate.addVariable(k_channel_equations.variable('V').clone())
    k_channel_parameters.addVariable(k_channel_equations.variable('V').clone())

    #  8.c 
    #      Connect these variables to their counterparts as needed.
    Variable.addEquivalence(n_gate.variable('t'), n_gate_equations.variable('t'))
    Variable.addEquivalence(n_gate.variable('V'), n_gate_equations.variable('V'))
    Variable.addEquivalence(n_gate.variable('t'), k_channel_equations.variable('t'))
    Variable.addEquivalence(n_gate.variable('V'), k_channel_equations.variable('V'))
    Variable.addEquivalence(k_channel.variable('t'), k_channel_equations.variable('t'))
    Variable.addEquivalence(k_channel.variable('V'), k_channel_equations.variable('V'))
    Variable.addEquivalence(k_channel_parameters.variable('V'), k_channel_equations.variable('V'))

    #  8.d 
    #      Fix the variable interfaces and validate the model, expecting no errors.
    model.fixVariableInterfaces()

    validator.validateModel(model)
    print_issues(validator)

    #  8.e 
    #      Analyse the model and expect that errors related to voltage and time now occur only in the
    #      top-level potassium channel component.  Because this needs to be connected to the imported
    #      controller component, they'll be addressed later in Step 10.
    analyser.analyseModel(model)
    print_issues(analyser)

    # end 8

    print('------------------------------------------------------------')
    print('   STEP 9: Connect the calculated variables                 ')
    print('------------------------------------------------------------')

    # STEP 9: Now we need to make sure that all of the calculated variables can move through
    #         the model properly.  In this example, the only calculated variable is n, the gate
    #         status.  This is calculated by solving the ODE in the n_gate equations component,
    #         but needs to be initialised by the n_gate parameters component, and its value
    #         passed back to the potassium channel equations component. 

    #  9.a 
    #      Make the required variable connections as described above.
    Variable.addEquivalence(n_gate_parameters.variable('n'), n_gate_equations.variable('n'))
    Variable.addEquivalence(k_channel_equations.variable('n'), n_gate.variable('n'))
    Variable.addEquivalence(n_gate.variable('n'), n_gate_equations.variable('n'))

    #  9.b 
    #      Fix the variable interfaces for the model, and validate, expecting no errors.
    model.fixVariableInterfaces()
    validator.validateModel(model)
    print_issues(validator)

    #  9.c 
    #      Analyse the model, expecting that the errors related to the n variable have been resolved.
    analyser.analyseModel(model)
    print_issues(analyser)

    #  end 9

    print('------------------------------------------------------------')
    print('   STEP 10: Connect to imported components                  ')
    print('------------------------------------------------------------')

    #  STEP 10:
    #  At this point, we have made all the connections we can between existing variables and components.
    #  (You can verify this for yourself by printing your model to the terminal again if you like.) 
    #  Now the problem we have is that we need to connect to variables inside imported components, 
    #  but these don't exist in our model yet: the import sources that we created in Steps 4 and 5
    #  are simply a recipe they don't actually create anything.
    print_model(model)

    #  In order to connect to variables in imported components, we can create dummy variables inside them.
    #  These will be overwritten when the imports are resolved and the model flattened, at which time
    #  the imported variables will replace the dummy ones.  As with other steps, we have a choice here.
    #  We can manually create variables or clone existing ones into the destination components we have
    #  already created or we can make use of the Importer class to help us manage these. We're going to 
    #  do the latter now.

    #  10.a 
    #      Create an Importer item.
    importer = Importer()

    #  end 10.a 
    #      Resolving imports for a model triggers the importer to go searching for all of the
    #      information required by this model's imports, even through multiple generations of import layers.
    #      It also instantiates each of those requirements into the importer's own library.
    #      You could use the Model.hasUnresolvedImports() function to test whether the operation was
    #      successful or not expecting it to be true before resolution, and false afterwards.

    #  10.b
    #      Pass the model and the path to the GateModel.cellml file into the Importer.resolveImports
    #      function.
    importer.resolveImports(model, '')

    #  10.c  Check the Importer for issues and print any found to the terminal - we do not expect any at this stage.
    print_issues(importer)

    #  end 10.c
    #       The components that we want to reuse from the GateModel.cellml and PotassiumChannelController.cellml
    #       are now available to us in two ways:
    #       - through the model() function of the destination component's ImportSource item or
    #       - as an item in the importer's library.  The library items can be retrieved either by index
    #         or by key, where the key is the name of the file that was resolved.  
    #  10.d
    #       Iterate through the items in the library (Importer.libraryCount() will give you
    #       the total), and print its keys to the terminal.  The keys can be retrieved as a 
    #       string from the Importer.key(index) function.  At this stage we expect only one model in the library.
    print('The importer has {} models in the library.'.format(importer.libraryCount()))
    for i in range(0, importer.libraryCount()):
        print(' library({}) = {}'.format(i, importer.key(i)))
    print()

    #  10.e
    #      We can simply use a clone of the imported components to define dummy variables in the 
    #      destination component.
    #      Create dummy components from the resolved imported components. You can get these from the 
    #      library or from the import source's model (or one of each, to prove to yourself that it works
    #      either way!).
    dummy_gate = imported_gate.importSource().model().component(imported_gate.importReference()).clone()
    dummy_controller = importer.library('PotassiumChannelController.cellml').component(controller.importReference()).clone()
    
    #      GOTCHA: Note that when an item is added to a new parent, it is automatically removed from 
    #         its original parent.  Iterating through a set of children is best done in descending
    #         index order or using a while loop so that child items are not skipped as the indices change.
    #  10.f
    #      Iterate through the variables in each dummy component, and add a clone of each variable 
    #      to the destination component. 
    while(dummy_gate.variableCount()):
        imported_gate.addVariable(dummy_gate.variable(0))
    
    while(dummy_controller.variableCount()):
        controller.addVariable(dummy_controller.variable(0))

    #      More connections are needed.  These should include:
    #          - (n_gate equations component : imported gate component)
    #          - n : X
    #          - alpha_n : alpha_X
    #          - beta_n : beta_X
    #          - t : t

    #  10.g
    #      Connect all the variables in the n_gate equations component to the dummy variables
    #      in the imported gate component.
    #      Repeat for the controller component and the potassium channel component.
    #      Fix the variable interfaces and validate the model, expecting there to be no errors.
    Variable.addEquivalence(n_gate_equations.variable('n'), imported_gate.variable('X'))
    Variable.addEquivalence(n_gate_equations.variable('alpha_n'), imported_gate.variable('alpha_X'))
    Variable.addEquivalence(n_gate_equations.variable('beta_n'), imported_gate.variable('beta_X'))
    Variable.addEquivalence(n_gate_equations.variable('t'), imported_gate.variable('t'))
    Variable.addEquivalence(controller.variable('t'), k_channel.variable('t'))
    Variable.addEquivalence(controller.variable('V'), k_channel.variable('V'))

    #  10.h
    #      Make sure that the output variable from this component - the potassium current - 
    #      is available at the top level, and with a public interface.  You'll need to create
    #      a dummy variable in the potassium channel component and link it appropriately.
    k_channel.addVariable(k_channel_equations.variable('i_K').clone())
    Variable.addEquivalence(k_channel_equations.variable('i_K'), k_channel.variable('i_K'))
    k_channel.variable('i_K').setInterfaceType('public_and_private')
    model.fixVariableInterfaces()

    validator.validateModel(model)
    print_issues(validator)

    #  end 10.h

    # The Analyser class can only operate on a flat (ie: import-free) model. In order
    # to do the final check before serialising our model for output, we will use the Importer
    # to create a flattened version of the model to submit for analysis.

    #  10.i 
    #      Create a flattened version of the final model using the Importer.flattenModel(model)
    #      function.  Run this through the analyser and expect there to be no issues reported.
    flatModel = importer.flattenModel(model)
    analyser.analyseModel(flatModel)
    print_issues(analyser)

    #  end 10.i
    #  Note that at this point an analysis of the unflattened model will still show errors,
    #  but that's totally fine.

    print('------------------------------------------------------------')
    print('   STEP 11: Output the model                                ')
    print('------------------------------------------------------------')

    #  11.a 
    #      Create a Printer instance and use it to serialise the model.  This creates a string
    #      containing the CellML-formatted version of the model.  Write this to a file called
    #      'PotassiumChannelModel.cellml'.
    printer = Printer()
    write_file = open('PotassiumChannelModel.cellml', 'w')
    write_file.write(printer.printModel(model))
    write_file.close()

    #  end

    print('The created model has been written to PotassiumChannelModel.cellml')
