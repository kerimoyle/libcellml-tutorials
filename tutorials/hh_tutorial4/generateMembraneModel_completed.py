"""
 libCellML TUTORIAL 4: Generate code for the Hodgkin-Huxley model.
 
  By the time you've worked through this tutorial you will be able to:
      - Use the Generator class to create C or Python code representing a CellML model
 
  This tutorial assumes you're already comfortable with:
      - Parsing an existing CellML file into a model instance
      - Using the diagnostic Validator class to check for syntactic issues 
      - Using the Importer class to resolve and flatten imports and
      - Using the Analyser class to check for mathematical issues in the model. 
      - Writing to files. 
"""
from libcellml import Analyser, Generator, GeneratorProfile, Importer, Model, Parser, Validator

from utilities import print_issues, print_model, get_cellml_element_type_from_enum, get_issue_level_from_enum, print_encapsulation

if __name__ == '__main__':

    print('----------------------------------------------------------')
    print('   STEP 1: Parse the existing membrane model              ')
    print('----------------------------------------------------------')

    # STEP 1: Parse an existing model from a CellML file.
    #         The Parser class is used to deserialise a CellML string into a Model instance.
    #         This means that you're responsible for finding, opening and reading the.cellml 
    #         file into a single string.  The Parser will then read that string and return a model.

    #  1.a 
    #       Read a CellML file into a std.string.
    read_file = open('MembraneModel.cellml')

    #  1.b 
    #       Create a Parser item. 
    parser = Parser()

    #  1.c 
    #       Use the parser to deserialise the contents of the string you've read and return the model.
    model = parser.parseModel(read_file.read())

    #  1.d 
    #       Print the parsed model to the terminal for viewing.
    print_model(model, False)

    #  end 1

    print('----------------------------------------------------------')
    print('   STEP 2: Resolve the imports and flatten                ')
    print('----------------------------------------------------------')
 
    #  2.a
    #      Create an Importer instance and use it to resolve the imports in your model.
    importer = Importer()
    importer.resolveImports(model, '')

    #  2.b
    #      Check that the importer has not raised any issues.
    print_issues(importer)

    #  2.c
    #      Use the importer to create a flattened version of the model.
    flatModel = importer.flattenModel(model)

    #  end 2

    print('----------------------------------------------------------')
    print('   STEP 3: Validate and analyse the flattened model       ')
    print('----------------------------------------------------------')

    #  3.a
    #      Create a Validator instance, pass in the flattened model, and check that
    #      there are no issues raised.
    validator = Validator()
    validator.validateModel(flatModel)
    print_issues(validator)

    #  3.b
    #      Create an Analyser instance, pass in the flattened model, and check that
    #      there are no issues raised.
    analyser = Analyser()
    analyser.analyseModel(flatModel) 
    print_issues(analyser)

    #  end 3

    print('----------------------------------------------------------')
    print('   STEP 4: Generate code and output                       ')
    print('----------------------------------------------------------')

    #  4.a 
    #       Create a Generator instance.  
    generator = Generator()

    #  4.b 
    #       The generator uses a GeneratorProfile item to set up a translation between the
    #       model stored as CellML and the language of your choice (currently C or Python).
    #       Create a GeneratorProfile object, and use the constructor argument of the
    #       GeneratorProfile.Profile enum for the language you want (C or PYTHON).
    profile = GeneratorProfile(GeneratorProfile.Profile.PYTHON)

    #  4.c 
    #       Use the generator's setProfile function to pass in the profile item you just created.
    generator.setProfile(profile)

    #  4.d 
    #       Instead of submitting a Model item (as we do for all other classes), 
    #       the Generator class will work from something which has already been processed 
    #       by the Analyser class: an AnalyserModel object.  
    #       Retrieve the analysed model using the Analyser.model() function, and submit 
    #       to the generator using the Generator.setModel(analysedModel) function.
    generator.setModel(analyser.model())

    #  4.e 
    #       (C profile only) If you're using the C profile then you have the option at this stage 
    #       to specify the file name of the interface file you'll create in the
    #       next step.  This means that the two files will be prepared to link to
    #       one another without manual editing later.  You can do this by specifying the
    #       header file name in the GeneratorProfile item using the 
    #       setInterfaceFileNameString('yourHeaderFileNameHere.h') function.  This will need
    #       to be the same as the file which you write to in step 4.g below.
    # profile.setInterfaceFileNameString('HodgkinHuxleyModel.h')

    #  4.f 
    #       Implementation code is the bulk of the model, and contains all the equations, 
    #       variables, units etc.  This is needed for both of the available profiles, and 
    #       would normally be stored in a.cpp or.py file.  
    #       Use the Generator.implementationCode() function to return the implementation 
    #       code as a string, and write it to a file with the appropriate extension.
    write_file = open('HodgkinHuxleyModel.py', 'w')
    write_file.write(generator.implementationCode())
    write_file.close()

    #  4.g 
    #       (C profile only) Interface code is the header needed by the C profile to define data types.
    #       Use the Generator.interfaceCode() function to return interface code as a string
    #       and write it to a.h header file.  This needs to be the same filename as you 
    #       specified in step 4.e above.
    # write_file = open('HodgkinHuxleyModel.h', 'w')
    # write_file.write(generator.interfaceCode())
    # write_file.close()

    #  end 4  

    print('The generated model code has been written to HodgkinHuxleyModel.py') 

    

