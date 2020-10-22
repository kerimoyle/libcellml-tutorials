"""
    TUTORIAL 3: MODEL CREATION THROUGH THE API

    By the time you have worked through Tutorial 3 you will be able to:
      - Create a new model and its child entities from scratch using the API
      - Define custom combinations of built-in units
      - Define your own custom units independent from the built-in units and
      - Use the Generator to create C or Python code representing the model.

    This tutorial assumes that you are comfortable with:
      - Accessing and adjusting names of items inside a model hierarchy (T2)
      - Creating a validator and using it to check a model for errors (T2)
      - Accessing the errors produced by a validator and using them to correct
        the model (T2) and
      - Serialising and printing a model to a CellML file (T1).
"""

from libcellml import Analyser, Component, Generator, GeneratorProfile, Model, Units, Validator, Variable

from tutorial_utilities import print_issues, print_model

if __name__ == "__main__":
    print("-------------------------------------------------------------")
    print(" TUTORIAL 3: MODEL CREATION AND CODE GENERATION WITH THE API ")
    print("-------------------------------------------------------------")

    print("-------------------------------------------------------------")
    print("   Step 1: Create model instance and maths                   ")
    print("-------------------------------------------------------------")

    #   1.a   Create a Model and name it.

    #   1.b   Create a component to use as an integrator, set its attributes and
    #         add it to the model.

    #   1.c, d, e
    #       Create the MathML2 string representing the governing equations.  

    #  1.f
    #     Create the header and footer strings.

    #  1.g
    #     Include the MathML strings in the component.

    #  1.h   
    #     Create a validator and use it to check the model so far.

    print("-------------------------------------------------------------")
    print("   Step 2: Create the variables                              ")
    print("-------------------------------------------------------------")

    #  2.a  
    #       Create the variables listed by the validator: d, a, b, c, time, y_s, y_f.

    #  2.b
    #       Add the variables into the component.

    #  2.c  
    #       Call the validator again to check the model.

    print("-------------------------------------------------------------")
    print("   Step 3: Create user-defined units                         ")
    print("-------------------------------------------------------------")

    #  3.a  
    #       Define the relationship between our custom units and the built-in
    #       units. There is a list of built-in units and their definitions
    #       available in section 19.2 of the CellML2 specification.
    #       First we create the "month" units, which will be equivalent to
    #       60*60*24*30 = 2,592,000 seconds.

    #  3.b  
    #       Create units which represent "per_month", which
    #       is simply the inverse of the "month" unit above.

    #  3.c      
    #       Create the sharks and fishes base units.

    #  3.d  
    #       Create the combined units for the constants.  Note that each item included
    #       with the addUnit command is multiplied to create the final Units definition.

    #  3.e  
    #       Set the units to their respective variables.

    #  3.f  
    #       Call the validator again to check the model.
    #       Expect one error regarding a missing unit in the MathML.

    #  3.g  
    #       Units for constants inside the MathML must be specified at the time.
    #       This means we need to adjust equation1 to include the per_month units.
    #       We have to wipe all the existing MathML and replace it.

    #  3.h  
    #       Validate once more, and expect there to be no errors this time.

    print("-------------------------------------------------------------")
    print("   Step 4: Analyse the mathematics                           ")
    print("-------------------------------------------------------------")

    #  4.a 
    #      Create an Analyser instance and pass it the model using the
    #      analyseModel function.  

    #  4.b 
    #      Check for errors found in the analyser. You should expect 6 errors,
    #      related to variables whose values are not computed or initialised.

    #  4.c 
    #      Add initial conditions to all variables except the base variable, time
    #      and the constant c which will be computed. 

    #  4.d 
    #      Reprocess the model and check that the analyser is now free of errors.

    print("-------------------------------------------------------------")
    print("   Step 5: Generate code and output                          ")
    print("-------------------------------------------------------------")

    #  5.a  
    #      Create a Generator instance.  Instead of giving it the Model item to process, 
    #      the generator takes the output from the analyser.  
    #      Retrieve the analysed model using the Analyser.model() function and pass it
    #      to the generator using the Generator.setModel function.

    #  5.b 
    #      First we'll use the default profile (C), so we need to output both the
    #      interfaceCode (the header file) and the implementationCode (source file)
    #      from the generator and write them to their respective files.

    #  5.c 
    #      Create a GeneratorProfile item using the libcellml.GeneratorProfile.Profile.PYTHON
    #      enum value in the constructor.  Pass this profile to the setProfile function in the
    #      generator.

    #  5.d
    #      Retrieve the Python implementation code (there is no header file) and write to a *.py file.
