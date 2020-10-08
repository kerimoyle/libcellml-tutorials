/**
 * COMBINE2020 libCellML TUTORIAL 4: Generate code for the Hodgkin-Huxley model.
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

#include "tutorial_utilities.h"

int main()
{

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 1: Parse the existing membrane model              " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    // STEP 1: Parse an existing model from a CellML file.
    //         The Parser class is used to deserialise a CellML string into a Model instance.
    //         This means that you're responsible for finding, opening and reading the *.cellml 
    //         file into a single string.  The Parser will then read that string and return a model.

    //  1.a Read a CellML file into a std::string.

    //  1.b Create a Parser item. 

    //  1.c Use the parser to deserialise the contents of the string you've read and return the model.
 
    //  1.d Print the parsed model to the terminal for viewing.
  
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 2: Resolve the imports and flatten                " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
 
    auto importer = libcellml::Importer::create();
    importer->resolveImports(model, "");
    printIssues(importer);

    auto flatModel = importer->flattenModel(model);

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 3: Validate and analyse the flattened model       " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    auto validator = libcellml::Validator::create();
    validator->validateModel(flatModel);
    printIssues(validator);

    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(flatModel); 
    printIssues(analyser);

    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "   STEP 4: Generate code and output                       " << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    //  4.a Create a Generator instance.  
    auto generator = libcellml::Generator::create();

    //  4.b The generator uses a GeneratorProfile item to set up a translation between the
    //      model stored as CellML and the language of your choice (currently C or Python).
    //      Create a GeneratorProfile object, and use the constructor argument of the
    //      libcellml::GeneratorProfile::Profile enum for the language you want (C or PYTHON).
    auto profile = libcellml::GeneratorProfile::create(libcellml::GeneratorProfile::Profile::C);

    //  4.c Use the generator's setProfile function to pass in the profile item you just created.
    generator->setProfile(profile);

    //  4.d Instead of submitting a Model item (as we do for all other classes), 
    //      the Generator class will work from something which has already been processed 
    //      by the Analyser class: an AnalyserModel object.  
    //      Retrieve the analysed model using the Analyser::model() function, and submit 
    //      to the generator using the Generator::setModel(analysedModel) function.
    generator->setModel(analyser->model());

    //  4.e (C only) If you're using the C profile then you have the option at this stage 
    //      to specify the file name of the interface file you'll create in the
    //      next step.  This means that the two files will be prepared to link to
    //      one another without manual editing later.  You can do this by specifying the
    //      header file name in the GeneratorProfile item using the 
    //      setInterfaceFileNameString("yourHeaderFileNameHere.h") function.  This will need
    //      to be the same as the file which you write to in step 4.g below.
    profile->setInterfaceFileNameString("HodgkinHuxleyModel.h");

    //  4.f Implementation code is the bulk of the model, and contains all the equations, 
    //      variables, units etc.  This is needed for both of the available profiles, and 
    //      would normally be stored in a *.cpp or *.py file.  
    //      Use the Generator::implementationCode() function to return the implementation 
    //      code as a string, and write it to a file with the appropriate extension.
    std::ofstream outFile("HodgkinHuxleyModel.c");
    outFile << generator->implementationCode();
    outFile.close();

    //  4.g (C only) Interface code is the header needed by the C profile to define data types.
    //      Use the Generator::interfaceCode() function to return interface code as a string
    //      and write it to a *.h header file.  This needs to be the same filename as you 
    //      specified in step 4.e above.
    outFile.open("HodgkinHuxleyModel.h");
    outFile << generator->interfaceCode();
    outFile.close();

    std::cout << "The generated model code has been written to HodgkinHuxleyModel.[c,h|py]"
              << std::endl;   
}
