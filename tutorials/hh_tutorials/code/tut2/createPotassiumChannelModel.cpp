/**
 * COMBINE2020 libCellML TUTORIAL 2: Creating a potassium channel model
 *
 *  By the time you have worked through this tutorial you will be able to:
 *      - Assemble a multi-component model using the API; 
 *      - Import items from existing models for reuse here;
 *      - Inter-connect the components using the equivalent variables
 *        functionality;
 *      - Use the diagnostic Validator class to identify errors in the
 *        model's syntax; 
 *      - Use the diagnostic Analyser class to identify errors in the 
 *        model's mathematical formulation; and
 *      - Serialise the model to CellML format for output.
 */

#include <iostream>
#include <fstream>

#include <libcellml>

#include "utilities.h"

int main()
{
    //  Setup useful things.
    std::string mathHeader = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\">\n";
    std::string mathFooter = "</math>";

    // Overall the model structure will be:
    // model: PotassiumChannelModel
    //     component: controller <-- imported from PotassiumChannelController.cellml
    //     component: potassiumChannel
    //         component: potassiumChannelEquations
    //                 component: nGateEquations
    //                     component: gateEquations <-- imported from GateModel.cellml, component gateEquations
    //                 component: nGateParameters <-- created here so that the parameters are specific to the nGateEquations.
    //         component: kChannelParameters

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 1: Define the model setup " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    //  1.a 
    //      Create a Model and name it appropriately.

    //  1.b 
    //      Create a wrapping component and name it "potassiumChannel".
  
    //  1.c 
    //      Add the component to the model.
  
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Define the potassium channel equations component " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    //  2.a 
    //      Create a Component instance for the equations and name it "potassiumChannelEquations".  
    //      Add it to the wrapper component you created above.
  
    //      The mathematics of a component is specified as a MathML 2 string (NB: higher versions 
    //      of MathML are not supported), and is added to the component using setMath() and 
    //      appendMath() functions.  
    //      Your string needs to contain the namespaces for MathML and for CellML: these have been
    //      provided for you in the mathHeader string above.

    //  2.b 
    //      Define the maths inside the potassiumChannelEquations component.
     
    //  2.c 
    //      Once the mathematics has been added to the component, and the component to the 
    //      model, we can make use of the diagnostic messages within the Validator class
    //      to tell us what else needs to be done.  
    //      Create a Validator instance, and pass it your model for processing using the 
    //      validateModel function.  

    //      Calling the validator does not return anything: we have to go looking for issues 
    //      that it found during processing.  When a problem is found, an Issue item is created
    //      containing:
    //          - a description string explaining the problem;
    //          - a URL at which more information is available;
    //          - an std::any item relevant to the problem, if available;
    //          - a level indicator; and
    //          - a cause indicator relevant to the stored item.
    //  We can use these issues as we need to.  The simplest way is to print the descriptions
    //  to the terminal.

    //  2.d
    //      Retrieve the number of issues encountered using the validator->issueCount() function,
    //      then retrieve the issue items from the validator using their index and the validator->issue(index)
    //      function.

    //  2.e 
    //      Create the variables needed and add them to the potassium channel component.
    //      Revalidate and expect errors related to variables without units.

    //  2.f 
    //      Create the missing Units items and add them to the model. These are:
    //      - milli-volts
    //      - milli-seconds
    //      - milli-moles
    //      - micro-Amperes per square centimetre
    //      - milli-Siemens per square centimetre

    //  2.g 
    //      Set the units on each of the variables.  
    //      Call the validator again, and expect there to be no errors.

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Create the nGate and nGateEquations components  " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    //  STEP 3: Create the nGate and its child components:
    //          - The nGateEquations has some of the working of a generic gate 
    //            (which we'll import from GateModel.cellml), but instead of constant values for alpha and beta, 
    //            we'll introduce a voltage dependence.
    //          - the nGateParameters component allows us to specify those parameters specific to the movement
    //            of potassium.

    //  3.a 
    //      Create a component, name it "nGate", and add it to the equations component.

    //  3.b 
    //      Create a component, name it "nGateEquations" and add it to the nGate component.

    //  3.c 
    //      Add the mathematics to the nGateEquations component and validate the model.
    //      Expect errors relating to missing variables.

    //  3.d 
    //      Add the missing variables to the nGateEquations component, and validate again.
    //      Expect errors relating to units missing from the variables.

    //  The only two Units which aren't available already are:
    //      - per millisecond 
    //      - per millivolt millisecond
    //      Remember that you'll need to give these names that are the same as those needed by the 
    //      variables.  In this case they are "per_ms" and "per_mV_ms".

    //  3.e
    //      Create the missing units and add them to the model.  

    //  3.f
    //      Associate the correct units items with the variables which need them.
    //      Revalidate the model, expecting there to be no errors reported.

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 4: Specify imports for the generic gate component " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    // STEP 4: Import the generic gate's equations component.
    //      The generic gate model (in GateModel.cellml) has two components: 
    //          - "gateEquations" which solves an ODE for the gate status parameter, X
    //          - "gateParameters" which sets the values of alpha, beta, and initialises X
    //      We will import only the "gateEquations" component and set it to be a child 
    //      of the nGateEquations component.  This means we can introduce the voltage
    //      dependence for the alpha and beta, and using a specified initial value for 
    //      the gate's status.  Note that the variable 'n' in the nGateEquations is 
    //      equivalent to the generic gate's variable 'X'.

    //  Imports require three things:
    //      - a destination for the imported item. This could be a Component or Units item.
    //      - a model to import for the imported item from.  This is stored in an ImportSource
    //        item containing the URL of the model to read.
    //      - the name of the item to import.  This is called the "import reference" and 
    //        is stored by the destination item.

    //  4.a 
    //      Create an ImportSource item and set its URL to be "GateModel.cellml".

    //  4.b 
    //      Create a destination component for the imported gate component, and add this to 
    //      the nGateEquations component. 

    //  4.c 
    //      Set the import reference on the component you just created to be the name
    //      of the component in the GateModel.cellml file that you want to use.  In this
    //      example, it is "gateEquations".

    //  4.d 
    //      Associate the import source with the component using the setImportSource function.
    //      Note that this step also makes the import source available to other items through the 
    //      Model::importSource(index) function.  This way the same model file can be used as a 
    //      source for more than one item.

    //  4.e 
    //      Validate the model and confirm that there are no issues.

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 5: Specify imports for the controller component " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    //  STEP 5: Repeat Step 4 to import a controller component.  This should be 
    //          at the top of the encapsulation hierarchy, and should import the component
    //          named "controller" from the file "PotassiumChannelController.cellml".

    //  5.a 
    //      Repeat steps 4.a-d for the controller component.  Put it at the top level of
    //      the encapsulation hierarchy.

    //  5.b Validate the model and confirm that there are no issues.


    //  At this point we've defined the equations that govern the potassium channel's operation.
    //  From here on, our goal is to make sure that the CellML representation of these equations
    //  is valid (using the Validator) and solvable (using the Analyser).
    
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 6: Analyse the model " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    // STEP 6: We will introduce the Analyser class here so that its use as a debugging 
    //         tool can be demonstrated.  Of course, we know ahead of time that there
    //         is still a lot of connections to be created between the components, but
    //         an analyser can help us to find them.

    // A reminder: we're aiming for a potassium channel component which can accept two external
    // parameters - time, t (ms) and voltage, V (mV) - and use them to calculate a potassium 
    // current, i_K (microA_per_cm2). 
    // A utility function printModel() has been provided to help you to see what's going 
    // on inside your model.

    //  6.a 
    //      Create an Analyser item and pass it the model for checking with the analyseModel function.

    //  6.b 
    //      The analyser is similar to the Validator and keeps a record of issues it encounters.
    //      Retrieve these and print to the terminal, just as you've done for the validator.
    //      Expect messages related to un-computed variables.

    //  Even though all of the messages we see are "variable not calculated" errors, we can divide
    //  them into different categories:
    //  - those variables which are constants whose value has not been set yet;
    //  - those variables whose calculation depends on as-yet un-calculated variables;
    //  - those variables which need to be connected to where their calculation happens; and
    //  - those variables which aren't present in any equation.

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 7: Define the constants " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    //      Use the printModel() function to show your current model contents. This should
    //      show that we have currently got variables only in the nGateEquations and potassiumChannelEquations
    //      components.  These need to have sibling parameters components created to hold any hard-coded
    //      values or initial conditions that are required.
    //  7.a 
    //      Print the model to the terminal.

    //      Create parameters siblings components for the equations components, and add the variables that 
    //      they will require.  These are:
    //      - potassium channel parameters
    //          - E_K (-87)
    //          - g_K (36)
    //      - nGate parameters
    //          - n (0.325)
    //      You can either do this by creating the variables from scratch (as in Step 3.d) but
    //      because these are intended to be duplicates of existing variables, but in another 
    //      component, we can simply add a cloned variable to the parameters component.

    //  7.b
    //      Create parameters components for the equations components, and add cloned versions of
    //      any variables which need to be given a value into the new parameters components.

    //  7.c 
    //      In order for other encapsulating components to access these variables, they also need to have
    //      intermediate variables in the nGate or potassium channel components too.  This is only true
    //      of variables that you want to be available to the outside.  In this example, we need to add
    //      the variable "n" to the nGate in order that its parent (the potassium channel equations) can 
    //      access it.

    //  7.d 
    //      Create variable connections between these variables and their counterparts in the equations
    //      components.  Validate, expecting errors related to missing or incorrect interface types.

    //  7.e 
    //      Set the required interface types as listed by the validator.  This can be done individually using the 
    //      Variable::setInterfaceType() function, or automatically using the Model::fixVariableInterfaces()
    //      function.  Validate again, expecting no validation errors.

    //      If we were to analyse the model again now we would we still have the same set of errors 
    //      as earlier as we haven't given a value to any of our parameters.  We can use the 
    //      Variable::setInitialValue() function to give these values to the following variables 
    //      in the parameters components:
    //      - potassium channel parameters:
    //          - E_K = -87 [mV]
    //          - g_K = 36 [milliS_per_cm2]
    //      - nGate parameters
    //          - n = 0.325 [dimensionless]

    //  7.f
    //      Set the constant values on the variables.  Analyse the model again, expecting 
    //      that the calculation errors related to these constants have been solved.

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 8: Connect the 'input' variables                    " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    // STEP 8: Looking at the variables listed we can see that some of our "external" or "input"
    //  variables are listed more than once.  These are the voltage, V, and time, t.  Time
    //  is needed in every equations component, including the imported gate component.
    //  Voltage is needed by the potassium channel and nGate equations components.

    //      Printing the model to the terminal we'll notice the components which contain V and t 
    //      variables.  Connections between the variables in any two components are only possible
    //      when those components are in a sibling-sibling or parent-child relationship.  We can see
    //      from the printed structure that the top-level potassiumChannel component doesn't have any 
    //      variables, and neither does the nGate component.  We'll need to create intermediate
    //      variables in those components to allow connections to be made through them.  
    //  8.a
    //      Use the printModel function to print your model to the terminal.

    //  8.b 
    //      Create dummy variables for time and voltage using the cloning technique described in 
    //      Step 7.b, and add a clone to each appropriate component.

    //  8.c 
    //      Connect these variables to their counterparts as needed.

    //  8.d 
    //      Fix the variable interfaces and validate the model, expecting no errors.

    //  8.e 
    //      Analyse the model and expect that errors related to voltage and time now occur only in the
    //      top-level potassium channel component.  Because this needs to be connected to the imported
    //      controller component, they'll be addressed later in Step 10.

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 9: Connect the 'calculated' variables               " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    // STEP 9: Now we need to make sure that all of the calculated variables can move through
    //         the model properly.  In this example, the only calculated variable is n, the gate
    //         status.  This is calculated by solving the ODE in the nGate equations component,
    //         but needs to be initialised by the nGate parameters component, and its value
    //         passed back to the potassium channel equations component. 

    //  9.a 
    //      Make the required variable connections as described above.

    //  9.b 
    //      Fix the variable interfaces for the model, and validate, expecting no errors.

    //  9.c 
    //      Analyse the model, expecting that the errors related to the n variable have been resolved.

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 10: Connect to imported components                  " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    //  STEP 10:
    //  At this point, we have made all the connections we can between existing variables and components.
    //  (You can verify this for yourself by printing your model to the terminal again if you like.) 
    //  Now the problem we have is that we need to connect to variables inside imported components, 
    //  but these don't exist in our model yet: the import sources that we created in Steps 4 and 5
    //  are simply a recipe; they don't actually create anything.

    //  In order to connect to variables in imported components, we can create dummy variables inside them.
    //  These will be overwritten when the imports are resolved and the model flattened, at which time
    //  the imported variables will replace the dummy ones.  As with other steps, we have a choice here.
    //  We can manually create variables or clone existing ones into the destination components we have
    //  already created; or we can make use of the Importer class to help us manage these. We're going to 
    //  do the latter now.

    //  10.a 
    //      Create an Importer item.

    //      Resolving imports for a model triggers the importer to go searching for all of the
    //      information required by this model's imports, even through multiple generations of import layers.
    //      It also instantiates each of those requirements into the importer's own library.
    //      You could use the Model::hasUnresolvedImports() function to test whether the operation was
    //      successful or not; expecting it to be true before resolution, and false afterwards.

    //  10.b
    //      Pass the model and the path to the GateModel.cellml file into the Importer::resolveImports
    //      function.

    //  10.c  Check the Importer for issues and print any found to the terminal - we do not expect any at this stage.

    //       The components that we want to reuse from the GateModel.cellml and PotassiumChannelController.cellml
    //       are now available to us in two ways:
    //       - through the model() function of the destination component's ImportSource item; or
    //       - as an item in the importer's library.  The library items can be retrieved either by index
    //         or by key, where the key is the name of the file that was resolved. 

    //  10.d
    //       Iterate through the items in the library (Importer::libraryCount() will give you
    //       the total), and print its keys to the terminal.  The keys can be retrieved as a 
    //       string from the Importer::key(index) function.  At this stage we expect only one model in the library.

    //  10.e
    //      We can simply use a clone of the imported components to define dummy variables in the 
    //      destination component.
    //      Create dummy components from the resolved imported components. You can get these from the 
    //      library or from the import source's model (or one of each, to prove to yourself that it works
    //      either way!).

    //      GOTCHA: Note that when an item is added to a new parent, it is automatically removed from 
    //         its original parent.  Iterating through a set of children is best done in descending
    //         index order or using a while loop so that child items are not skipped as the indices change.
    //  10.f
    //      Iterate through the variables in each dummy component, and add a clone of each variable 
    //      to the destination component. 

    //      More connections are needed.  These should include:
    //          - (nGate equations component : imported gate component)
    //          - n : X
    //          - alpha_n : alpha_X
    //          - beta_n : beta_X
    //          - t : t

    //  10.g
    //      Connect all the variables in the nGate equations component to the dummy variables
    //      in the imported gate component.
    //      Repeat for the controller component and the potassium channel component.
    //      Fix the variable interfaces and validate the model, expecting there to be no errors.

    //  10.h
    //      Make sure that the output variable from this component - the potassium current - 
    //      is available at the top level, and with a public interface.  You'll need to create
    //      a dummy variable in the potassium channel component and link it appropriately.

    // The Analyser class can only operate on a flat (ie: import-free) model. In order
    // to do the final check before serialising our model for output, we will use the Importer
    // to create a flattened version of the model to submit for analysis.

    //  10.i 
    //      Create a flattened version of the final model using the Importer::flattenModel(model)
    //      function.  Run this through the analyser and expect there to be no issues reported.

    //  Note that at this point an analysis of the unflattened model will still show errors,
    //  but that's totally fine.

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "   STEP 11: Output the model                                " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    //  11.a 
    //      Create a Printer instance and use it to serialise the model.  This creates a string
    //      containing the CellML-formatted version of the model.  Write this to a file called
    //      "PotassiumChannelModel.cellml".

}
