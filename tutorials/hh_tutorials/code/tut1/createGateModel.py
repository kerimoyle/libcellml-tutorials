'''
COMBINE2020 libCellML Tutorial 1: Creating a generic gate model

By the time you have worked through this tutorial you will be able to:
     - Assemble a model using the API 
     - Use the diagnostic Validator class to identify errors in the 
       model's syntax 
     - Use the diagnostic Analyser class to identify errors in the 
       model's mathematical construction and
     - Serialise the model to CellML format for output.
'''

from libcellml import Analyser, Component, Model, Printer, Units, Validator, Variable

from utilities import print_issues, print_model, get_cellml_element_type_from_enum, get_issue_level_from_enum

if __name__ == '__main__':

    #  Setup useful things.
    math_header = '<math xmlns="http://www.w3.org/1998/Math/MathML" xmlns:cellml="http://www.cellml.org/cellml/2.0#">\n'
    math_footer = '</math>'

    print('----------------------------------------------------------')
    print('   STEP 1: Setup the model                                ')
    print('----------------------------------------------------------')

    #  1.a 
    #      The first step is to create a Model item which will later contain the component and 
    #      the units it needs.  

    #  1.b 
    #      Each CellML element must have a name, which is set using the setName() function.

    #  1.c 
    #      We'll create a wrapper component whose only job is to encapsulate the other components.
    #      This makes is a lot easier for this model to be reused, as the connections between
    #      components internal to this one won't need to be re-established.
    #      Note that the constructor for all named CellML entities is overloaded, so 
    #      you can pass it the name string at the time of creation.
    #      Create a component named 'gate'.

    #  1.d Finally we need to add the component to the model.  This sets it at the top-level of 
    #      the components' encapsulation hierarchy.  All other components need to be added 
    #      to this component, rather than the model.
    #      Add the component to the model using the Model::addComponent() function.

    #  1.e
    #      Print the model to the terminal using the print_model helper function and 
    #      check it is what you'd expect.

    print('----------------------------------------------------------')
    print('   STEP 2: Create the gateEquations component             ')
    print('----------------------------------------------------------')

    #  2.a 
    #     Create a gateEquations component and name it 'gateEquations'.

    #  2.b 
    #     Add the new gateEquations component to the gate component.

    #  2.c 
    #     Add the mathematics to the gateEquations component.

    #  2.d
    #      Print the model to the terminal using the print_model helper function and 
    #      check it is what you'd expect.  Include the second argument as True so that 
    #      the maths is included.
    
    print('----------------------------------------------------------')
    print('   STEP 3: Validate the model                             ')
    print('----------------------------------------------------------')

    #  Once the mathematics has been added to the component, and the component to the 
    #  model, we can make use of the diagnostic messages within the Validator class
    #  to tell us what else needs to be done.  

    #  3.a 
    #      Create a Validator instance, and pass it your model for processing using the 
    #      validateModel function.  

    #  Calling the validator does not return anything: we have to go looking for issues 
    #  that it found during processing.  When a problem is found, an Issue item is created
    #  containing:
    #      - a description string explaining the problem
    #      - a URL at which more information is available
    #      - an std::any item relevant to the problem, if available
    #      - a level indicator and
    #      - a cause indicator relevant to the stored item.
    #  We can use these issues as we need to.  The simplest way is to print the descriptions
    #  to the terminal.

    #  Two helper functions have been provided for this tutorial that will help printing 
    #  enumerated values to the terminal.  These are:
    #      - get_issue_level_from_enum, and
    #      - get_cellml_element_type_from_enum. 

    #  3.b 
    #      Retrieve the number of issues encountered using the validator.issueCount() function,
    #      then retrieve the issue items from the validator using their index and the validator.issue(index)
    #      function.  Print the information from each issue to the terminal.

    print('----------------------------------------------------------')
    print('   STEP 4: Add the variables                              ')
    print('----------------------------------------------------------')

    #  The issues reported by the validator are related to the MathML string that we entered 
    #  in Step 2 requiring variables which don't yet exist. These must be created, named, 
    #  and added to their parent component.

    #  4.a 
    #      Create items for the missing variables and add them to the gateEquations component.
    #      You will need to be sure to give them names which match exactly those reported by the
    #      validator, or are present in the MathML string.  

    #  4.b
    #      Validate again, and expect errors relating to missing units.
    #      Note that you can use the helper function print_issues(validator) to print your
    #      issues to the screen instead of repeating the code from 3.b.

    print('----------------------------------------------------------')
    print('   STEP 5: Add the units                                  ')
    print('----------------------------------------------------------')

    #  The validator has reported that the four variables are missing units attributes.  
    #  In this example none of the units exist yet, so we need to create all of them. 
    #  The variables' units should be:
    #      - t, time has units of *milliseconds*
    #      - X, gate status has units of *dimensionless*
    #      - alpha_X and beta_X, rates, have units of *per millisecond*.

    # 5.a 
    #      Create the units which will be needed by your variables and add them to the model.
   
    #  5.b
    #      Add Unit items to the units you created to define them.

    #  5.c
    #      Add the Units to the model (not the component) so that other components can make 
    #      use of them too.

    #  5.d
    #      Use the setUnits function to associate them with the appropriate variables.  

    #  5.e
    #      Validate again, and expect no errors.

    print('----------------------------------------------------------')
    print('   STEP 6: Analyse the model                              ')
    print('----------------------------------------------------------')

    #  6.a 
    #      Create an Analyser item and submit the model for processing. 

    #  6.b 
    #      Just like the Validator class, the Analyser class keeps track of issues. 
    #      Retrieve these and print to the terminal. Expect errors related to 
    #      un-computed variables and missing initial values.

    #  In order to avoid hard-coding values here, we will need to connect to external 
    #  values to initialise the X variable and provide the value for alpha_X and beta_X.
    #  This means that:
    #      - we need to create an external component to hold variable values
    #      - we need to create external variables in that component 
    #      - we need to specify the connections between variables and
    #      - we need to permit external connections on the variables.
    
    #  6.c 
    #      Create a component which will store the hard-coded values for initialisation.
    #      Name it 'gateParameters', and add it to the top-level gate component as a sibling
    #      of the gateEquations component.

    #  6.d 
    #      Create appropriate variables in this component, and set their units.
    #      Use the setInitialValue function to initialise them.

    #  6.e 
    #      Specify a variable equivalence between the gateEquations variables and the parameter variables.
    #      Validate the model again, expecting errors related to the variable interface types.

    #  6.f 
    #      Set the variable interface type according to the recommendation from the validator.
    #      This can either be done individually using the Variable::setInterfaceType() function, or 
    #      en masse for all the model's interfaces using the Model::fixVariableInterfaces() function.
    #      Validate and analyse again, expecting no errors. 

    print('----------------------------------------------------------')
    print('   STEP 7: Sanity check                                   ')
    print('----------------------------------------------------------')

    #  7.a 
    #      Print the model to the terminal using the helper function print_model.

    #      Looking at the printout we see that the top-level component has no variables.  
    #      Even though this is clearly a valid situation (as proved by 4.f), it's not
    #      going to make this model easy to reuse.  We need to make sure that any input and
    #      output variables are also connected into the top level gate component.  
    
    #  7.b
    #      Create intermediate variables for time t and gate status X in the gate component,
    #      and ensure they have a public and private interface to enable two-way connection.
    #      You may also need to set a public and private connection onto t and X in the
    #      equations component too.
    
    #  7.c 
    #      Connect the intermediate variables to their respective partners in the equations
    #      component, and recheck the model.

    print('----------------------------------------------------------')
    print('   STEP 8: Serialise and output the model                 ')
    print('----------------------------------------------------------')

    #  8.a 
    #      Create a Printer instance and use it to serialise the model.  This creates a string
    #      containing the CellML-formatted version of the model.  Write this to a file called
    #     'GateModel.cellml'.
