/**
 * libCellML TUTORIAL 4: Generate code for the Hodgkin-Huxley model.
 * 
 *  By the time you've worked through this tutorial you will be able to:
 *      - Use the Generator class to create C or Python code representing a CellML model;
 * 
 *  This tutorial assumes you're already comfortable with:
 *      - Parsing an existing CellML file into a model instance ();
 *      - Using the diagnostic Validator class to check for syntactic issues; 
 *      - Using the Importer class to resolve and flatten imports; and
 *      - Using the Analyser class to check for mathematical issues in the model. 
 *      - Writing to files. 
 */

#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "utilities.h"

int main()
{
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 1: Parse the existing membrane model              " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 1: Parse an existing model from a CellML file.
    //         The Parser class is used to deserialise a CellML string into a Model instance.
    //         This means that you're responsible for finding, opening and reading the *.cellml 
    //         file into a single string.  The Parser will then read that string and return a model.

    //  1.a 
    //      Read a CellML file into a std::string.
    std::ifstream inFile("MembraneModel.cellml");
    std::stringstream inFileContents;
    inFileContents << inFile.rdbuf();

    //  1.b 
    //      Create a Parser item. 
    auto parser = libcellml::Parser::create();

    //  1.c 
    //      Use the parser to deserialise the contents of the string you've read and return the model.
    auto model = parser->parseModel(inFileContents.str());

    //  1.d 
    //      Print the parsed model to the terminal for viewing.
    printModel(model, false);

    //  end 1

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Resolve the imports and flatten                " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
 
    //  2.a
    //      Create an Importer instance and use it to resolve the imports in your model.
    auto importer = libcellml::Importer::create();
    importer->resolveImports(model, "");

    //  2.b
    //      Check that the importer has not raised any issues.
    printIssues(importer);

    //  2.c
    //      Use the importer to create a flattened version of the model.
    auto flatModel = importer->flattenModel(model);

    //  end 2

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Validate and analyse the flattened model       " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  3.a
    //      Create a Validator instance, pass in the flattened model, and check that
    //      there are no issues raised.
    auto validator = libcellml::Validator::create();
    validator->validateModel(flatModel);
    printIssues(validator);

    //  3.b
    //      Create an Analyser instance,pass in the flattened model, and check that
    //      there are no issues raised.
    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(flatModel); 
    printIssues(analyser);

    //  end 3

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 4: Generate code and output                       " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  4.a 
    //      Create a Generator instance.  
    auto generator = libcellml::Generator::create();

    //  end 4.a
    //      The generator uses a GeneratorProfile item to set up a translation between the
    //      model stored as CellML and the language of your choice (currently C or Python).
    //  4.b
    //      Create a GeneratorProfile object, and use the constructor argument of the
    //      libcellml::GeneratorProfile::Profile enum for the language you want (C or PYTHON).
    auto profile = libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::C);

    //  4.c 
    //      Use the generator's setProfile function to pass in the profile item you just created.
    generator->setProfile(profile);

    //  end 4.c 
    //      Instead of submitting a Model item (as we do for all other classes), 
    //      the Generator class will work from something which has already been processed 
    //      by the Analyser class: an AnalyserModel object.

    //  4.d
    //      Retrieve the analysed model using the Analyser::model() function, and submit 
    //      to the generator using the Generator::setModel(analysedModel) function.
    generator->setModel(analyser->model());

    //  end 4.d 
    //      (C only) If you're using the C profile then you have the option at this stage 
    //      to specify the file name of the interface file you'll create in the
    //      next step.  This means that the two files will be prepared to link to
    //      one another without manual editing later.  
    //  4.e
    //      You can do this by specifying the header file name in the GeneratorProfile item
    //      using the setInterfaceFileNameString("yourHeaderFileNameHere.h") function.
    //      This will need to be the same as the file which you write to in step 4.g below.
    profile->setInterfaceFileNameString("HodgkinHuxleyModel.h");

    //  end 4.e 
    //      Implementation code is the bulk of the model, and contains all the equations, 
    //      variables, units etc.  This is needed for both of the available profiles, and 
    //      would normally be stored in a *.cpp or *.py file.
    //  4.f
    //      Use the Generator::implementationCode() function to return the implementation 
    //      code as a string, and write it to a file with the appropriate extension.
    std::ofstream outFile("HodgkinHuxleyModel.cpp");
    outFile << generator->implementationCode();
    outFile.close();

    //  4.g 
    //      (C only) Interface code is the header needed by the C profile to define data types.
    //      Use the Generator::interfaceCode() function to return interface code as a string
    //      and write it to a *.h header file.  This needs to be the same filename as you 
    //      specified in step 4.e above.
    outFile.open("HodgkinHuxleyModel.h");
    outFile << generator->interfaceCode();
    outFile.close();

    //  end 4

    std::cout << "The generated model code has been written to HodgkinHuxleyModel.[cpp,h]"
              << std::endl;   
}
