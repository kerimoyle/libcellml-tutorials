/**
 *  TUTORIAL 3: MODEL CREATION AND CODE GENERATION WITH THE API
 *
 *  By the time you have worked through Tutorial 3 you will be able to:
 *    - Create a new model and its child entities from scratch using the API;
 *    - Define custom combinations of built-in units;
 *    - Define your own custom units independent from the built-in units; and
 *    - Use the Generator to create C or Python code representing the model.
 *
 *  This tutorial assumes that you are comfortable with:
 *    - Accessing and adjusting names of items inside a model hierarchy (T2);
 *    - Creating a validator and using it to check a model for errors (T2);
 *    - Accessing the errors produced by a validator and using them to correct
 *      the model (T2); and
 *    - Serialising and printing a model to a CellML file (T1).
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "utilities.h"

int main()
{
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << " TUTORIAL 3: MODEL CREATION AND CODE GENERATION WITH THE API" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "   Step 1: Create a component                                " << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    //  1.a
    //      Create a Model instance, set its name and id.
    auto model = libcellml::Model::create("tutorial_3_model");
    model->setId("tutorial_3_model_id");

    //  Check that it worked.
    std::cout << "Model has name: '" << model->name() << "'" << std::endl;
    std::cout << "Model has id: '" << model->id() << "'" << std::endl;

    //  1.b   
    //      Create a Component instance to use as an integrator, set its attributes and
    //      add it to the model.
    auto component = libcellml::Component::create("predator_prey_component");
    model->addComponent(component);

    //  Check that it worked.
    std::cout << "Model has " << model->componentCount()
              << " components:" << std::endl;
    for (size_t c = 0; c < model->componentCount(); ++c) {
        std::cout << "  Component [" << c << "] has name: '"
                  << model->component(c)->name() << "'" << std::endl;
        std::cout << "  Component [" << c << "] has id: '"
                  << model->component(c)->id() << "'" << std::endl;
    }

    //  1.c
    //      Create the MathML2 strings representing the governing equations.
    std::string equation1 =
        "  <apply><eq/>\n"
        "    <ci>c</ci>\n"
        "    <apply><plus/>\n"
        "      <ci>a</ci>\n"
        "      <cn>2.0</cn>\n"
        "    </apply>\n"
        "  </apply>\n";

    //  1.d
    std::string equation2 =
        "  <apply><eq/>\n"
        "    <apply><diff/>\n"
        "      <bvar><ci>time</ci></bvar>\n"
        "      <ci>y_s</ci>\n"
        "    </apply>\n"
        "    <apply><plus/>\n"
        "      <apply><times/>\n"
        "        <ci>a</ci>\n"
        "        <ci>y_s</ci>\n"
        "      </apply>\n"
        "      <apply><times/>\n"
        "        <ci>b</ci>\n"
        "        <ci>y_s</ci>\n"
        "        <ci>y_f</ci>\n"
        "      </apply>\n"
        "    </apply>\n"
        "  </apply>\n";

    //  1.e
    std::string equation3 =
        "  <apply><eq/>\n"
        "    <apply><diff/>\n"
        "      <bvar><ci>time</ci></bvar>\n"
        "      <ci>y_f</ci>\n"
        "    </apply>\n"
        "    <apply><plus/>\n"
        "      <apply><times/>\n"
        "        <ci>c</ci>\n"
        "        <ci>y_f</ci>\n"
        "      </apply>\n"
        "      <apply><times/>\n"
        "        <ci>d</ci>\n"
        "        <ci>y_s</ci>\n"
        "        <ci>y_f</ci>\n"
        "      </apply>\n"
        "    </apply>\n"
        "  </apply>\n";

    //  1.f
    //      Create the header and footer strings.
    std::string mathHeader = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\">\n";
    std::string mathFooter = "</math>";

    //  1.g 
    //      Add the maths strings in to the component.
    component->setMath(mathHeader);
    component->appendMath(equation1);
    component->appendMath(equation2);
    component->appendMath(equation3);
    component->appendMath(mathFooter);

    //  1.h 
    //      Create a Validator instance and use it to check for issues so far.
    //      We expect there to be 18 errors found, related to missing variables
    //      in the component.  You can use the utility printIssues function 
    //      to print them to the terminal.
    auto validator = libcellml::Validator::create();
    validator->validateModel(model);
    printIssues(validator);

    //  end 1

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "   Step 2: Create the variables                              " << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    //  2.a 
    //      Create the variables listed by the validator: d, a, b, c, time, y_s, y_f.
    auto sharks = libcellml::Variable::create("y_s");
    auto fish = libcellml::Variable::create("y_f");
    auto time = libcellml::Variable::create("time");
    auto a = libcellml::Variable::create("a");
    auto b = libcellml::Variable::create("b");
    auto c = libcellml::Variable::create("c");
    auto d = libcellml::Variable::create("d");

    //  2.b 
    //      Add the variables into the component.
    component->addVariable(a);
    component->addVariable(b);
    component->addVariable(c);
    component->addVariable(d);
    component->addVariable(sharks);
    component->addVariable(fish);
    component->addVariable(time);

    //  2.c 
    //      Call the validator again to check.
    validator->validateModel(model);
    printIssues(validator);

    //  end 2

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "   Step 3: Create the units                                  " << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    //  3.a 
    //      Create units representing a month, or 2592000 seconds.
    auto month = libcellml::Units::create("month");
    month->addUnit("second", 0, 1, 2592000);  // base unit, prefix, exponent, multiplier
    model->addUnits(month);

    //  3.b 
    //      Create the per_month unit based on the month defined in 3.a.
    auto per_month = libcellml::Units::create("per_month");
    per_month->addUnit("month", -1);  // base unit, exponent
    model->addUnits(per_month);

    //  3.c 
    //      Create the sharks and fishes base units, "number_of_sharks" and "thousands_of_fish".
    auto number_of_sharks = libcellml::Units::create("number_of_sharks");
    auto thousands_of_fish = libcellml::Units::create("thousands_of_fish");
    model->addUnits(number_of_sharks);
    model->addUnits(thousands_of_fish);

    //  3.d 
    //      Create the combined units for the constants, "per_shark_month" and "per_fish_month".
    auto b_units = libcellml::Units::create("per_shark_month");
    b_units->addUnit("per_month");
    b_units->addUnit("number_of_sharks", -1);
    model->addUnits(b_units);

    auto d_units = libcellml::Units::create("per_1000fish_month");
    d_units->addUnit("per_month");
    d_units->addUnit("thousands_of_fish", -1);
    model->addUnits(d_units);

    //  3.e 
    //      Add the units to their variables using the setUnits function.
    time->setUnits(month);
    a->setUnits(per_month);
    b->setUnits(b_units);
    c->setUnits(per_month);
    d->setUnits(d_units);
    sharks->setUnits(number_of_sharks);
    fish->setUnits(thousands_of_fish);

    //  3.f 
    //      Call the validator to check the model.  We expect one error regarding the missing units in the MathML.
    validator->validateModel(model);
    printIssues(validator);

    //  3.g 
    //      Units for constants inside the MathML must be specified at the time.  This means we need to adjust
    //      equation1 to include the per_month units.  We have to wipe all the existing MathML and replace it.
    component->removeMath();
    component->setMath(mathHeader);
    equation1 =
        "  <apply><eq/>\n"
        "    <ci>c</ci>\n"
        "    <apply><plus/>\n"
        "      <ci>a</ci>\n"
        "      <cn cellml:units=\"per_month\">2.0</cn>\n"
        "    </apply>\n"
        "  </apply>\n";

    component->appendMath(equation1);
    component->appendMath(equation2);
    component->appendMath(equation3);
    component->appendMath(mathFooter);

    //  3.h 
    //      Revalidate your model and expect there to be no errors.
    validator->validateModel(model);
    printIssues(validator);
    assert(validator->errorCount() == 0);

    //  end 3

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "   Step 4: Analyse the model                                 " << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    //  4.a 
    //      Create an Analyser instance and pass it the model using the
    //      analyseModel function.  
    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(model);

    //  4.b 
    //      Check for errors found in the analyser. You should expect 6 errors,
    //      related to variables whose values are not computed or initialised.
    printIssues(analyser);

    //  4.c 
    //      Add initial conditions to all variables except the base variable, time
    //      and the constant c which will be computed. Reprocess the model.
    a->setInitialValue(-0.8);
    b->setInitialValue(0.3);
    d->setInitialValue(-0.6);
    sharks->setInitialValue(1.0);
    fish->setInitialValue(2.0);

    //  4.d 
    //      Reprocess the model and check that the generator is now free of errors.
    analyser->analyseModel(model);
    printIssues(analyser);

    //  end 4

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "   Step 5: Generate code and write to files                  " << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    //  5.a  
    //      Create a Generator instance.  Instead of giving it the Model item to process, 
    //      the generator takes the output from the analyser.  
    //      Retrieve the analysed model using the Analyser::model() function and pass it
    //      to the generator using the Generator::setModel function.
    auto generator = libcellml::Generator::create();
    generator->setModel(analyser->model());

    //  The generator takes the CellML model and turns it into procedural code in another 
    //  language.  The default is C, but Python is available too.  This language choice is
    //  called the "profile", and is stored in a GeneratorProfile item.

    //  If you're using the C profile then you have the option at this stage 
    //  to specify the file name of the interface file you'll create in the
    //  next step.  This means that the two files will be prepared to link to
    //  one another without manual editing later.  

    //  5.b
    //      You can do this by specifying the header file name in the GeneratorProfile item
    //      using the setInterfaceFileNameString("yourHeaderFileNameHere.h") function.
    //      This will need to be the same as the file which you write to in step 5.c below.
    auto profileC = generator->profile();
    profileC->setInterfaceFileNameString("PredatorPrey.h");

    //  5.c 
    //      First we'll use the default profile (C), so we need to output both the
    //      interfaceCode (the header file) and the implementationCode (source file)
    //      from the generator and write them to their respective files.
    std::ofstream outFile("PredatorPrey.h");
    outFile << generator->interfaceCode();
    outFile.close();

    outFile.open("PredatorPrey.c");
    outFile << generator->implementationCode();
    outFile.close();

    //  5.d
    //      Create a GeneratorProfile item using the libcellml::GeneratorProfile::Profile::PYTHON
    //      enum value in the constructor.  Pass this profile to the setProfile function in the
    //      generator.
    auto profilePython =
        libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::PYTHON);
    generator->setProfile(profilePython);

    //  5.e
    //      Retrieve the Python implementation code (there is no header file) and write to a *.py file.
    outFile.open("PredatorPrey.py");
    outFile << generator->implementationCode();
    outFile.close();

    //  end 5

    std::cout << "The generated files have been written to PredatorPrey.[c,h,py]." << std::endl;

    //  The next tutorial will take you through the process of running the simulation
    //  described by this model.

    //  Go and have a cuppa, you're done!
}
