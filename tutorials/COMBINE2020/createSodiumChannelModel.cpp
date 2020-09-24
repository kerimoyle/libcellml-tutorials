
#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "tutorial_utilities.h"

int main()
{
    auto model = libcellml::Model::create("SodiumChannelModel");
    auto controller = libcellml::Component::create("controller");
    model->addComponent(controller);

    auto naChannel = libcellml::Component::create("sodiumChannel");
    model->addComponent(naChannel);

    auto naChannelEquations = libcellml::Component::create("sodiumChannelEquations");
    auto naChannelParameters = libcellml::Component::create("sodiumChannelParameters");
    naChannel->addComponent(naChannelEquations);
    naChannel->addComponent(naChannelParameters);

    auto mGate = libcellml::Component::create("mGate");
    naChannelEquations->addComponent(mGate);

    auto mGateParameters = libcellml::Component::create("mGateParameters");
    auto mGateEquations = libcellml::Component::create("mGateEquations");
    mGate->addComponent(mGateParameters);
    mGate->addComponent(mGateEquations);

    auto hGate = libcellml::Component::create("hGate");
    naChannelEquations->addComponent(hGate);

    auto hGateParameters = libcellml::Component::create("hGateParameters");
    auto hGateEquations = libcellml::Component::create("hGateEquations");
    hGate->addComponent(hGateParameters);
    hGate->addComponent(hGateEquations);

    auto importedGateH = libcellml::Component::create("importedGateH");
    hGateEquations->addComponent(importedGateH);

    auto importedGateM = libcellml::Component::create("importedGateM");
    mGateEquations->addComponent(importedGateM);

    printModelToTerminal(model);

    // Specify imports
    auto gateImportSource = libcellml::ImportSource::create();
    gateImportSource->setUrl("GateModel.cellml");

    importedGateH->setImportSource(gateImportSource);
    importedGateH->setImportReference("gateEquations");

    importedGateM->setImportSource(gateImportSource);
    importedGateM->setImportReference("gateEquations");

    auto controllerImportSource = libcellml::ImportSource::create();
    controllerImportSource->setUrl("SodiumChannelController.cellml");

    controller->setImportSource(controllerImportSource);
    controller->setImportReference("controller");

    // Specify maths: sodium channel current
    auto mathHeader = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\">\n";
    auto mathFooter = "</math>";

    auto equationNaConductance = 
        "  <apply><eq/>\n"
        "    <ci>Na_conductance</ci>\n"
        "    <apply><times/>\n"
        "     <ci>g_Na</ci>\n"
        "     <ci>h</ci>\n"
        "     <apply><power/>\n"
        "       <ci>m</ci>\n"
        "       <cn cellml:units=\"dimensionless\">3</cn>\n"
        "     </apply>\n"
        "   </apply>\n"
        "  </apply>\n";

    auto equationNaCurrent = 
        "  <apply><eq/>\n"
        "    <ci>i_Na</ci>\n"
        "    <apply><times/>\n"
        "      <ci>Na_conductance</ci>\n"
        "      <apply><minus/>\n"
        "        <ci>V</ci>\n"
        "        <ci>E_Na</ci>\n"
        "      </apply>\n"
        "    </apply>\n"
        "  </apply>\n";

    naChannelEquations->setMath(mathHeader);
    naChannelEquations->appendMath(equationNaConductance);
    naChannelEquations->appendMath(equationNaCurrent);
    naChannelEquations->appendMath(mathFooter);

    // mGate equations
    auto equationAlphaM = 
        "  <apply>\n"
        "    <eq/>\n"
        "    <ci>alpha_m</ci>\n"
        "    <apply>\n"
        "      <divide/>\n"
        "      <apply>\n"
        "        <times/>\n"
        "        <apply>\n"
        "          <minus/>\n"
        "          <cn cellml:units=\"per_mV_ms\">0.1</cn>\n"
        "        </apply>\n"
        "        <apply>\n"
        "          <plus/>\n"
        "          <ci>V</ci>\n"
        "          <cn cellml:units=\"mV\">50</cn>\n"
        "        </apply>\n"
        "      </apply>\n"
        "      <apply>\n"
        "        <minus/>\n"
        "        <apply>\n"
        "          <exp/>\n"
        "          <apply>\n"
        "            <divide/>\n"
        "            <apply>\n"
        "              <minus/>\n"
        "              <apply>\n"
        "                <plus/>\n"
        "                <ci>V</ci>\n"
        "                <cn cellml:units=\"mV\">50</cn>\n"
        "              </apply>\n"
        "            </apply>\n"
        "            <cn cellml:units=\"mV\">10</cn>\n"
        "          </apply>\n"
        "        </apply>\n"
        "        <cn cellml:units=\"dimensionless\">1</cn>\n"
        "      </apply>\n"
        "    </apply>\n"
        "  </apply>\n";

    auto equationBetaM = 
    "  <apply>\n"
    "    <eq/>\n"
    "    <ci>beta_m</ci>\n"
    "    <apply>\n"
    "      <times/>\n"
    "      <cn cellml:units=\"per_ms\">4</cn>\n"
    "      <apply>\n"
    "        <exp/>\n"
    "        <apply>\n"
    "          <divide/>\n"
    "          <apply>\n"
    "            <minus/>\n"
    "            <apply>\n"
    "              <plus/>\n"
    "              <ci>V</ci>\n"
    "              <cn cellml:units=\"mV\">75</cn>\n"
    "            </apply>\n"
    "          </apply>\n"
    "          <cn cellml:units=\"mV\">18</cn>\n"
    "        </apply>\n"
    "      </apply>\n"
    "    </apply>\n"
    "  </apply>\n";

    mGateEquations->setMath(mathHeader);
    mGateEquations->appendMath(equationAlphaM);
    mGateEquations->appendMath(equationBetaM);
    mGateEquations->appendMath(mathFooter);

    // hGate equations
    auto equationAlphaH = 
    "   <apply>\n"
    "    <eq/>\n"
    "    <ci>alpha_h</ci>\n"
    "    <apply>\n"
    "      <times/>\n"
    "      <cn cellml:units=\"per_ms\">0.07</cn>\n"
    "      <apply>\n"
    "        <exp/>\n"
    "        <apply>\n"
    "          <divide/>\n"
    "          <apply>\n"
    "            <minus/>\n"
    "            <apply>\n"
    "              <plus/>\n"
    "              <ci>V</ci>\n"
    "              <cn cellml:units=\"mV\">75</cn>\n"
    "            </apply>\n"
    "          </apply>\n"
    "          <cn cellml:units=\"mV\">20</cn>\n"
    "        </apply>\n"
    "      </apply>\n"
    "    </apply>\n"
    "  </apply>\n";

    auto equationBetaH = 
    "  <apply>\n"
    "    <eq/>\n"
    "    <ci>beta_h</ci>\n"
    "    <apply>\n"
    "      <divide/>\n"
    "      <cn cellml:units=\"per_ms\">1</cn>\n"
    "      <apply>\n"
    "        <plus/>\n"
    "        <apply>\n"
    "          <exp/>\n"
    "          <apply>\n"
    "            <divide/>\n"
    "            <apply>\n"
    "              <minus/>\n"
    "              <apply>\n"
    "                <plus/>\n"
    "                <ci>V</ci>\n"
    "                <cn cellml:units=\"mV\">45</cn>\n"
    "              </apply>\n"
    "            </apply>\n"
    "            <cn cellml:units=\"mV\">10</cn>\n"
    "          </apply>\n"
    "        </apply>\n"
    "        <cn cellml:units=\"dimensionless\">1</cn>\n"
    "      </apply>\n"
    "    </apply>\n"
    "  </apply>\n";

    hGateEquations->setMath(mathHeader);
    hGateEquations->appendMath(equationAlphaH);
    hGateEquations->appendMath(equationBetaH);
    hGateEquations->appendMath(mathFooter);

    auto validator = libcellml::Validator::create();
    validator->validateModel(model);
    printErrorsToTerminal(validator);

    // Create variables.
    mGateEquations->addVariable(libcellml::Variable::create("alpha_m"));
    mGateEquations->addVariable(libcellml::Variable::create("V"));
    mGateEquations->addVariable(libcellml::Variable::create("beta_m"));
    mGateEquations->addVariable(libcellml::Variable::create("m"));

    hGateEquations->addVariable(libcellml::Variable::create("alpha_h"));
    hGateEquations->addVariable(libcellml::Variable::create("beta_h"));
    hGateEquations->addVariable(libcellml::Variable::create("V"));
    hGateEquations->addVariable(libcellml::Variable::create("h"));

    naChannelEquations->addVariable(libcellml::Variable::create("Na_conductance"));
    naChannelEquations->addVariable(libcellml::Variable::create("g_Na"));
    naChannelEquations->addVariable(libcellml::Variable::create("h"));
    naChannelEquations->addVariable(libcellml::Variable::create("m"));
    naChannelEquations->addVariable(libcellml::Variable::create("i_Na"));
    naChannelEquations->addVariable(libcellml::Variable::create("V"));
    naChannelEquations->addVariable(libcellml::Variable::create("E_Na"));

    validator->validateModel(model);
    printErrorsToTerminal(validator);

    // Create units. Add to model. 
    auto mV = libcellml::Units::create("mV");
    mV->addUnit("volt", "milli");
    model->addUnits(mV);

    auto ms = libcellml::Units::create("ms");
    ms->addUnit("second", "milli");
    model->addUnits(ms);

    auto per_ms = libcellml::Units::create("per_ms");
    per_ms->addUnit("second", "milli", -1);
    model->addUnits(per_ms);

    auto per_mV_ms = libcellml::Units::create("per_mV_ms");
    per_mV_ms->addUnit("second", "milli", -1);
    per_mV_ms->addUnit("volt", "milli", -1);
    model->addUnits(per_mV_ms);

    auto microA_per_cm2 = libcellml::Units::create("microA_per_cm2");
    microA_per_cm2->addUnit("ampere", "micro");
    microA_per_cm2->addUnit("metre", "centi", -2);
    model->addUnits(microA_per_cm2);

    auto mS_per_cm2 = libcellml::Units::create("mS_per_cm2");
    mS_per_cm2->addUnit("siemens", "milli");
    mS_per_cm2->addUnit("metre", "centi", -2);
    model->addUnits(mS_per_cm2);

    validator->validateModel(model);
    printErrorsToTerminal(validator);

    // Associate units with variables.
    mGateEquations->variable("alpha_m")->setUnits("per_ms");
    mGateEquations->variable("beta_m")->setUnits("per_ms");
    mGateEquations->variable("V")->setUnits(mV);
    mGateEquations->variable("m")->setUnits("dimensionless");

    hGateEquations->variable("alpha_h")->setUnits("per_ms");
    hGateEquations->variable("beta_h")->setUnits("per_ms");
    hGateEquations->variable("V")->setUnits(mV);
    hGateEquations->variable("h")->setUnits("dimensionless");

    naChannelEquations->variable("V")->setUnits(mV);
    naChannelEquations->variable("m")->setUnits("dimensionless");
    naChannelEquations->variable("h")->setUnits("dimensionless");
    naChannelEquations->variable("i_Na")->setUnits(microA_per_cm2);
    naChannelEquations->variable("g_Na")->setUnits(mS_per_cm2);
    naChannelEquations->variable("E_Na")->setUnits(mV);
    naChannelEquations->variable("Na_conductance")->setUnits(mS_per_cm2);

    validator->validateModel(model);
    printErrorsToTerminal(validator);

    // Analyse.
    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(model);
    printErrorsToTerminal(analyser);

    // Connect variables: Constants.
    //  Create constants in parameters components
    naChannelParameters->addVariable(naChannelEquations->variable("g_Na")->clone());
    naChannelParameters->addVariable(naChannelEquations->variable("E_Na")->clone());
    naChannelParameters->variable("g_Na")->setInitialValue(120);
    naChannelParameters->variable("E_Na")->setInitialValue(35);

    //  Create initialising variables in parameters components
    mGateParameters->addVariable(mGateEquations->variable("m")->clone());
    mGateParameters->variable("m")->setInitialValue(0.05);
    hGateParameters->addVariable(hGateEquations->variable("h")->clone());
    hGateParameters->variable("h")->setInitialValue(0.6);

    // Resolve imports.
    auto importer = libcellml::Importer::create();
    importer->resolveImports(model, "");
    printErrorsToTerminal(importer);

    // Create dummy variables in imported components
    std::cout << "The importer has " << importer->libraryCount() << " models in the library." << std::endl;
    for(size_t i = 0; i < importer->libraryCount(); ++i) {
        std::cout << " library("<<i<<") = " << importer->key(i) << std::endl;
    }
    std::cout << std::endl;

    auto dummyGateH = importedGateH->importSource()->model()->component(importedGateH->importReference())->clone();
    auto dummyGateM = importedGateM->importSource()->model()->component(importedGateM->importReference())->clone();
    auto dummyController = importer->library("SodiumChannelController.cellml")->component(controller->importReference())->clone();

    while(dummyGateH->variableCount()) {
        importedGateH->addVariable(dummyGateH->variable(0));
    }
    while(dummyGateM->variableCount()) {
        importedGateM->addVariable(dummyGateM->variable(0));
    }
    while(dummyController->variableCount()) {
        controller->addVariable(dummyController->variable(0));
    }

    printModelToTerminal(model);

    // Create intermediate variables for t:
    naChannel->addVariable(controller->variable("t")->clone());
    naChannelEquations->addVariable(controller->variable("t")->clone());
    hGate->addVariable(controller->variable("t")->clone());
    mGate->addVariable(controller->variable("t")->clone());
    hGateEquations->addVariable(controller->variable("t")->clone());
    mGateEquations->addVariable(controller->variable("t")->clone());

    // Create intermediate variables for h and m:
    hGate->addVariable(hGateEquations->variable("h")->clone());
    mGate->addVariable(mGateEquations->variable("m")->clone());

    // Create intermediate variables for V:
    hGate->addVariable(controller->variable("V")->clone());
    mGate->addVariable(controller->variable("V")->clone());
    naChannel->addVariable(controller->variable("V")->clone());

    // Make sure that the output variable is specified in the top-level component, with a public facing interface.
    naChannel->addVariable(naChannelEquations->variable("i_Na")->clone());
    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("i_Na"), naChannel->variable("i_Na")));

    // Manually connect variables throughout the model.
    assert(libcellml::Variable::addEquivalence(importedGateH->variable("X"), hGateEquations->variable("h")));
    assert(libcellml::Variable::addEquivalence(importedGateH->variable("alpha_X"), hGateEquations->variable("alpha_h")));
    assert(libcellml::Variable::addEquivalence(importedGateH->variable("beta_X"), hGateEquations->variable("beta_h")));
    assert(libcellml::Variable::addEquivalence(importedGateH->variable("t"), hGateEquations->variable("t")));

    assert(libcellml::Variable::addEquivalence(importedGateM->variable("X"), mGateEquations->variable("m")));
    assert(libcellml::Variable::addEquivalence(importedGateM->variable("alpha_X"), mGateEquations->variable("alpha_m")));
    assert(libcellml::Variable::addEquivalence(importedGateM->variable("beta_X"), mGateEquations->variable("beta_m")));
    assert(libcellml::Variable::addEquivalence(importedGateM->variable("t"), mGateEquations->variable("t")));

    assert(libcellml::Variable::addEquivalence(hGateEquations->variable("h"), hGateParameters->variable("h")));
    assert(libcellml::Variable::addEquivalence(mGateEquations->variable("m"), mGateParameters->variable("m")));

    assert(libcellml::Variable::addEquivalence(hGateEquations->variable("h"), hGate->variable("h")));
    assert(libcellml::Variable::addEquivalence(hGateEquations->variable("V"), hGate->variable("V")));
    assert(libcellml::Variable::addEquivalence(hGateEquations->variable("t"), hGate->variable("t")));

    assert(libcellml::Variable::addEquivalence(mGateEquations->variable("m"), mGate->variable("m")));
    assert(libcellml::Variable::addEquivalence(mGateEquations->variable("V"), mGate->variable("V")));
    assert(libcellml::Variable::addEquivalence(mGateEquations->variable("t"), mGate->variable("t")));

    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("h"), hGate->variable("h")));
    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("V"), hGate->variable("V")));
    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("t"), hGate->variable("t")));
    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("m"), mGate->variable("m")));
    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("V"), mGate->variable("V")));
    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("t"), mGate->variable("t")));

    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("g_Na"), naChannelParameters->variable("g_Na")));
    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("E_Na"), naChannelParameters->variable("E_Na")));

    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("V"), naChannel->variable("V")));
    assert(libcellml::Variable::addEquivalence(naChannelEquations->variable("t"), naChannel->variable("t")));

    assert(libcellml::Variable::addEquivalence(controller->variable("V"), naChannel->variable("V")));
    assert(libcellml::Variable::addEquivalence(controller->variable("t"), naChannel->variable("t")));

    model->fixVariableInterfaces();

    // Manually make sure that the "output" variables have a public facing interface.
    naChannel->variable("i_Na")->setInterfaceType("public_and_private");

    auto flatModel = importer->flattenModel(model);
    validator->validateModel(flatModel);
    printErrorsToTerminal(validator);
    analyser->analyseModel(flatModel);
    printErrorsToTerminal(analyser);

    auto printer = libcellml::Printer::create();
    std::ofstream outFile("SodiumChannelModel.cellml");
    outFile << printer->printModel(model);
    outFile.close();

    printModelToTerminal(model);

}
