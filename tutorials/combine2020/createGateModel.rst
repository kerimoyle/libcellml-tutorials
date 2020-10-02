..  _combine_createGateModel:

Tutorial 1: Creating a model using the API
==========================================

By the time you have worked through this tutorial you will be able to:
- Assemble a model using the API; 
- Use the diagnostic Validator class to identify errors in the model's syntax; 
- Use the diagnostic Analyser class to identify errors in the model's mathematical construction; and
- Serialise the model to CellML format for output.

**Requirements (C++)**

- :download:`CMakeLists.txt` The CMake file for building this tutorial;
- :download:`createGateModel.cpp` Either the skeleton code, or ..
- :download:`createGateModel_completed.cpp` the completed tutorial code.

**Requirements (Python)**

- :download:`createGateModel.py` Either the skeleton code, or ..
- :download:`createGateModel_completed.py` the completed tutorial code.

.. contents:: Contents
:local:

Step 1: Set up the model
------------------------

.. container:: dothis

    **1.a** The first step is to create a `Model` item which will later contain the component and the units it needs.

      
auto model = libcellml::Model::create();

.. container:: dothis

    **1.b** Each CellML element must have a name, which is set using the setName() function.
    model->setName("GateModel");

.. container:: dothis

    **1.c** We'll create a wrapper component whose only job is to encapsulate the other components.
    This makes is a lot easier for this model to be reused, as the connections between components internal to this one won't need to be re-established.
    Note that the constructor for all named CellML entities is overloaded, so you can pass it the name string at the time of creation.
    Create a component named "gate".

auto gate = libcellml::Component::create("gate");

.. container:: dothis

    **1.d** Finally we need to add the component to the model.  
    This sets it at the top-level of the components' encapsulation hierarchy.
    All other components need to be added to this component, rather than the model.
    Add the component to the model using the Model::addComponent() function.

model->addComponent(gate);

Step 2: Create the gateEquations component
------------------------------------------

.. container:: dothis

    **2.a** Create a gateEquations component, name it "gateEquations" and add it to the model.

auto gateEquations = libcellml::Component::create("gateEquations");

.. container:: dothis

    **2.b** Add the new gateEquations component to the gate component.

gate->addComponent(gateEquations);

.. container:: dothis

    **2.c** Add the mathematics to the gateEquations component.

std::string equation =
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


