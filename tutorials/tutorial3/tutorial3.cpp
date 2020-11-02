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
    
    //  1.b   
    //      Create a Component instance to use as an integrator, set its attributes and
    //      add it to the model.

    //  1.c, d, e
    //      Create the MathML2 strings representing the governing equations.

    //  1.f
    //      Create the header and footer strings.

    //  1.g 
    //      Add the maths strings in to the component.

    //  1.h 
    //      Create a Validator instance and use it to check for issues so far.
    //      We expect there to be 18 errors found, related to missing variables
    //      in the component.  You can use the utility printIssues function 
    //      to print them to the terminal.

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "   Step 2: Create the variables                              " << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    //  2.a 
    //      Create the variables listed by the validator: d, a, b, c, time, y_s, y_f.

    //  2.b 
    //      Add the variables into the component.

    //  2.c 
    //      Call the validator again to check.

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "   Step 3: Create the units                                  " << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    //  3.a 
    //      Create units representing a month, or 2592000 seconds.

    //  3.b 
    //      Create the per_month unit based on the month defined in 3.a.

    //  3.c 
    //      Create the sharks and fishes base units, "number_of_sharks" and "thousands_of_fish".

    //  3.d 
    //      Create the combined units for the constants, "per_shark_month" and "per_fish_month".

    //  3.e 
    //      Add the units to their variables using the setUnits function.

    //  3.f 
    //      Call the validator to check the model.  We expect one error regarding the missing units in the MathML.

    //  3.g 
    //      Units for constants inside the MathML must be specified at the time.  This means we need to adjust
    //      equation1 to include the per_month units.  We have to wipe all the existing MathML and replace it.

    //  3.h 
    //      Revalidate your model and expect there to be no errors.

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "   Step 4: Analyse the model                                 " << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    //  4.a 
    //      Create an Analyser instance and pass it the model using the
    //      analyseModel function.  

    //  4.b 
    //      Check for errors found in the analyser. You should expect 6 errors,
    //      related to variables whose values are not computed or initialised.

    //  4.c 
    //      Add initial conditions to all variables except the base variable, time
    //      and the constant c which will be computed. Reprocess the model.

    //  4.d 
    //      Reprocess the model and check that the generator is now free of errors.

    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "   Step 5: Generate code and write to files                  " << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    //  5.a  
    //      Create a Generator instance.  Instead of giving it the Model item to process, 
    //      the generator takes the output from the analyser.  
    //      Retrieve the analysed model using the Analyser::model() function and pass it
    //      to the generator using the Generator::setModel function.

    //  5.b 
    //      First we'll use the default profile (C), so we need to output both the
    //      interfaceCode (the header file) and the implementationCode (source file)
    //      from the generator and write them to their respective files.

    //  5.c 
    //      Create a GeneratorProfile item using the libcellml::GeneratorProfile::Profile::PYTHON
    //      enum value in the constructor.  Pass this profile to the setProfile function in the
    //      generator.

    //  5.d
    //      Retrieve the Python implementation code (there is no header file) and write to a *.py file.

}
