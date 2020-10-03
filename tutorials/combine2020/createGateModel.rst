..  _combine_createGateModel:

Tutorial 1: Creating a model using the API
==========================================

.. container:: shortlist

    By the time you have worked through this tutorial you will be able to:

    - Assemble a model using the API; 
    - Use the diagnostic Validator class to identify errors in the model's syntax; 
    - Use the diagnostic Analyser class to identify errors in the model's mathematical construction; and
    - Serialise the model to CellML format for output.

.. container:: shortlist

    **Requirements (C++)**

    - :download:`CMakeLists.txt<CMakeLists_GateModel.txt>` The CMake file for building this tutorial;
    - :download:`createGateModel.cpp<createGateModel.cpp>` Either the skeleton code, or ..
    - :download:`createGateModel_completed.cpp<createGateModel_completed.cpp>` the completed tutorial code.

.. container:: shortlist

    **Requirements (Python)**

    - :download:`createGateModel.py<createGateModel.py>` Either the skeleton code, or ..
    - :download:`createGateModel_completed.py<createGateModel_completed.py>` the completed tutorial code.

.. contents:: Contents
    :local:

Overview
--------
This is the first tutorial in a series designed to walk the user through some of libCellML's functionality available in the API.
Its goal is to create from scratch a voltage-independent ion gate model.
The theory of this kind of gate can be found on the :ref:`Ion Gate theory page<theory_iongates>`.

One of the goals of the CellML format (which must be supported by libCellML) is the construction of models that are reusable.
Throughout these tutorials we will create entities in such a way as to enable their reuse as easily as possible.  
The basic structure of this model highlights that too.

.. container:: shortlist

    We will create a model with three components:
    - An *equations* component, which contains all of the working pieces and mathematics specific to the model;
    - A *parameters* component, which contains any hard-coded parameters or values specific to this model; and
    - A wrapper component, which is the parent of the other two.

This arrangement means that it's simple to import this model into others, as well as to locate or over-ride parameter values or reuse equations.
Each of the components created throughout this series of tutorials will follow this same structure.

Step 1: Set up the model
------------------------

.. container:: useful

  :api:`Model class<Model>`
    - create
    - setName
    - addComponent
  :api:`Component class<Component>`
    - create
    - setName
    - addComponent

.. container:: dothis

    **1.a** The first step is to create a `Model` item which will later contain the component and the units it needs.
    Use the :code:`Model` create function to make a new instance.

.. container:: dothis

    **1.b** Each CellML element must have a name, which is set using the :code:`setName` function.
    Set the name of the model to be "GateModel".
    
We'll create a wrapper component whose only job is to encapsulate the other components.
This makes is a lot easier for this model to be reused, as the connections between components internal to this one won't need to be re-established.
Note that the constructor for all named CellML entities is overloaded, so you can pass it the name string at the time of creation.

.. container:: dothis

    **1.c** Create a new :code:`Component` item named "gate" using the overloaded constructor.

Finally we need to add the component to the model.  
This sets it at the top-level of the component encapsulation hierarchy.
All other components will then need to be added to this component, rather than to the model.

.. container:: dothis

    **1.d** Add the component to the model using the :code:`addComponent` function.

.. tabs::

    .. tab:: C++ snippet

        .. literalinclude:: ../combine2020/code/createGateModel.cpp
            :language: c++
            :start-after: // STEP 1
            :end-before: // END STEP 1 

    .. tab:: Python snippet

        .. literalinclude:: ../combine2020/code/createGateModel.py
            :language: python
            :start-after: # STEP 1
            :end-before: # END STEP 1

.. container:: toggle

    .. container:: header

        Show C++ snippet

    .. literalinclude:: ../combine2020/code/createGateModel.cpp
        :language: c++
        :start-after: // STEP 1
        :end-before: // END STEP 1 


.. container:: toggle

    .. container:: header

        Show Python snippet

    .. literalinclude:: ../combine2020/code/createGateModel.py
        :language: python
        :start-after: # STEP 1
        :end-before: # END STEP 1







.. container:: toggle

  .. container:: header

    Show C++ code




.. container:: toggle

  .. container:: header

    Show Python code

  .. code-block:: python

        model = Model()
        model.setName('GateModel')
        gate = Component('gate')
        model.addComponent(gate)

Step 2: Create the gateEquations component
------------------------------------------
Inside the wrapper component you created in Step 1 we need to create two more: an equations component, and a parameters component.
In this step we'll construct the equations component.

.. container:: dothis

    **2.a** Create a new equations component named "gateEquations".

.. container:: dothis

    **2.b** Add the new gateEquations component to the gate component.

.. container:: toggle

    .. container:: header

        Show C++ code

    .. code-block:: c++

        // Create a new component using the overloaded constructor.
        auto gateEquations = libcellml::Component::create("gateEquations");

        // Add the new component to the existing gate component.
        gate->addComponent(gateEquations);

.. container:: toggle

    .. container:: header

        Show Python code

    .. code-block:: python

        gate_equations = Component('gateEquations')
        gate.addComponent(gate_equations)

Since this is an *equations*-flavoured component, it should contain the bulk of the calculations and mathematics for the gate.
Maths is added using MathML2 (no other levels are supported) strings.  

In this example we need to represent just one equation:

.. math::

    \frac{dX}{dt} = \alpha_{X}\left( 1 - X \right) - \beta_{X}.X

If you're happy to write your own MathML2 string then please go ahead, but if you'd rather not you can use the code provided under the code toggles further down the page.

.. container:: useful

    - :api:`Component class<Component>`
        - :code:`setMath`
        - :code:`appendMath`
        - :code:`math`

.. container:: dothis

    **2.c** Construct a string representing the MathML of the equation above.
    You will need to enclose the string with the appropriate header and footer.
    These are provided for you in the skeleton code, or simply copy them from below.
    Use the :code:`setMath` and :code:`appendMath` functions to add your strings to the equations component.

.. container:: toggle

    .. container:: header

        Show C++ code
    
    .. code-block:: c++

        std::string mathHeader = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\">\n";
        std::string mathFooter = "</math>";
        std::string equationX =
            "  <apply><eq/>\n"
            "    <apply><diff/>\n"
            "      <bvar><ci>t</ci></bvar>\n"
            "      <ci>X</ci>\n"
            "    </apply>\n" 
            "    <apply><minus/>\n"
            "      <apply><times/>\n"
            "        <ci>alpha_X</ci>\n"
            "        <apply><minus/>\n"
            "          <cn cellml:units=\"dimensionless\">1</cn>\n"
            "          <ci>X</ci>\n"
            "        </apply>\n" 
            "      </apply>\n" 
            "      <apply><times/>\n"
            "        <ci>beta_X</ci>\n"
            "        <ci>X</ci>\n"
            "      </apply>\n" 
            "    </apply>\n" 
            "  </apply>\n"; 

        gateEquations->setMath(mathHeader);
        gateEquations->appendMath(equation);
        gateEquations->appendMath(mathFooter);

.. container:: toggle

    .. container:: header

        Show Python code
    
    .. code-block:: python

        # Python code here TODO



auto validator = libcellml::Validator::create();
validator->validateModel(model);
std::cout << "The validator has found " << validator->issueCount() << " issues." << std::endl;
for(size_t i = 0; i < validator->issueCount(); ++i) {
    std::cout << validator->issue(i)->description() << std::endl;
}

.. container:: dothis

    **2.d** Add the missing variables to the gateEquations component, and validate again.
    Expect errors relating to missing units.

gateEquations->addVariable(libcellml::Variable::create("t"));
gateEquations->addVariable(libcellml::Variable::create("alpha_X"));
gateEquations->addVariable(libcellml::Variable::create("beta_X"));
gateEquations->addVariable(libcellml::Variable::create("X"));

validator->validateModel(model);
std::cout << "The validator has found " << validator->issueCount() << " issues." << std::endl;
for(size_t i = 0; i < validator->issueCount(); ++i) {
    std::cout << validator->issue(i)->description() << std::endl;
}

.. container:: dothis

    **2.e** Create the units which will be needed by your variables and add them to the model.
    Use the setUnits function to associate them with the appropriate variables.  
    Validate again, and expect no errors.

auto ms = libcellml::Units::create("ms");
ms->addUnit("second", "milli");
model->addUnits(ms);

auto per_ms = libcellml::Units::create("per_ms");
per_ms->addUnit("second", "milli", -1);
model->addUnits(per_ms);

gateEquations->variable("t")->setUnits(ms);
gateEquations->variable("alpha_X")->setUnits(per_ms);
gateEquations->variable("beta_X")->setUnits(per_ms);
gateEquations->variable("X")->setUnits("dimensionless");

validator->validateModel(model);
std::cout << "The validator has found " << validator->issueCount() << " issues." << std::endl;
for(size_t i = 0; i < validator->issueCount(); ++i) {
    std::cout << validator->issue(i)->description() << std::endl;
}
std::cout << std::endl;


Step 3: Analyse the mathematical construction of the model
----------------------------------------------------------

.. container:: dothis

    **3.a** Create an Analyser item and submit the model for processing. 

auto analyser = libcellml::Analyser::create();
analyser->analyseModel(model);

.. container:: dothis

    **3.b** Just like the Validator class, the Analyser class keeps track of issues. 
    Retrieve these and print to the terminal. Expect errors related to 
    un-computed variables and missing initial values.

std::cout << "The analyser has found " << analyser->issueCount() << " issues." << std::endl;
for(size_t i = 0; i < analyser->issueCount(); ++i) {
    std::cout << analyser->issue(i)->description() << std::endl;
}
std::cout << std::endl;

In order to avoid hard-coding values here, we will need to connect to external 
values to initialise the X variable and provide the value for alpha_X and beta_X.
This means three things need to happen:
    - we need to create an external component to hold variable values;
    - we need to create external variables in that component; 
    - we need to specify the connections between variables; and
    - we need to permit external connections on the variables.

.. container:: dothis

    **3.c** Create a component which will store the hard-coded values for initialisation.
    Name it "gateParameters", and add it to the top-level gate component as a sibling of the gateEquations component.

auto gateParameters = libcellml::Component::create("gateParameters");
gate->addComponent(gateParameters);

.. container:: dothis

    **3.d** Create appropriate variables in this component, and set their units.
    Use the setInitialValue function to initialise them.
{
    auto X = libcellml::Variable::create("X");
    X->setUnits("dimensionless");
    X->setInitialValue(0);
    gateParameters->addVariable(X);

    auto alpha = libcellml::Variable::create("alpha");
    alpha->setUnits(per_ms);
    alpha->setInitialValue(0.1);
    gateParameters->addVariable(alpha);

    auto beta = libcellml::Variable::create("beta");
    beta->setUnits(per_ms);
    beta->setInitialValue(0.5);
    gateParameters->addVariable(beta);
}

.. container:: dothis

    **3.e** Specify a variable equivalence between the gateEquations variables and the parameter variables.
    Validate the model again, expecting errors related to the variable interface types.

libcellml::Variable::addEquivalence(gateEquations->variable("X"), gateParameters->variable("X"));
libcellml::Variable::addEquivalence(gateEquations->variable("alpha_X"), gateParameters->variable("alpha"));
libcellml::Variable::addEquivalence(gateEquations->variable("beta_X"), gateParameters->variable("beta"));

validator->validateModel(model);
std::cout << "The validator has found " << validator->issueCount() << " issues." << std::endl;
for(size_t i = 0; i < validator->issueCount(); ++i) {
    std::cout << validator->issue(i)->description() << std::endl;
}
std::cout << std::endl;

.. container:: dothis

    **3.f** Set the variable interface type according to the recommendation from the validator.
    This can either be done individually using the Variable::setInterfaceType() function, or en masse for all the model's interfaces using the Model::fixVariableInterfaces() function.
    Validate and analyse again, expecting no errors. 

model->fixVariableInterfaces();

validator->validateModel(model);
std::cout << "The validator has found " << validator->issueCount() << " issues." << std::endl;
for(size_t i = 0; i < validator->issueCount(); ++i) {
    std::cout << validator->issue(i)->description() << std::endl;
}
std::cout << std::endl;

analyser->analyseModel(model);
std::cout << "The analyser has found " << analyser->issueCount() << " issues." << std::endl;
for(size_t i = 0; i < analyser->issueCount(); ++i) {
    std::cout << analyser->issue(i)->description() << std::endl;
}
std::cout << std::endl;

.. container:: gotcha

    GOTCHA! Even though both the Analyser and Validator have given their approval to the model as it stands, in order for this to be reusable by other models in the future, we need to be able to connect to variables in the top-level component(s).
    By default the interface type given to a variable is "none", which prevents connection.
    Thus, we need to over-ride the "fixed" interface type for the time variable and set it to "public". 
    It's worth checking that your top-level components are accessible as a matter of course.

.. container:: dothis

    **3.g** Set the time variable in the gate equations component to have a public interface.
gateEquations->variable("t")->setInterfaceType("public");

Step 4: Serialise and output the model
--------------------------------------


    **4.a** Create a Printer instance and use it to serialise the model.
    This creates a string containing the CellML-formatted version of the model.
    Write this to a file called "GateModel.cellml".

auto printer = libcellml::Printer::create();
std::ofstream outFile("GateModel.cellml");
outFile << printer->printModel(model);
outFile.close();


