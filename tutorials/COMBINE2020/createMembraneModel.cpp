
#include <fstream>
#include <iostream>
#include <sstream>

#include <libcellml>

#include "tutorial_utilities.h"

int main()
{
    auto model = libcellml::Model::create("MembraneModel");
    auto controller = libcellml::Component::create("controller");
    model->addComponent(controller);

    auto membrane = libcellml::Component::create("membrane");
    model->addComponent(membrane);

    auto membraneEquations = libcellml::Component::create("membraneEquations");
    membrane->addComponent(membraneEquations);

    auto membraneParameters = libcellml::Component::create("membraneParameters");
    membrane->addComponent(membraneParameters);

    auto naChannel = libcellml::Component::create("sodiumChannel");
    membraneEquations->addComponent(naChannel);

    auto kChannel = libcellml::Component::create("potassiumChannel");
    membraneEquations->addComponent(kChannel);

    auto leakage = libcellml::Component::create("leakage");
    membraneEquations->addComponent(leakage);

    // Specify imports.
    auto naChannelImportSource = libcellml::ImportSource::create();
    naChannelImportSource->setUrl("SodiumChannelModel.cellml");
    naChannel->setImportSource(naChannelImportSource);
    naChannel->setImportReference("sodiumChannel");

    auto kChannelImportSource = libcellml::ImportSource::create();
    kChannelImportSource->setUrl("PotassiumChannelModel.cellml");
    kChannel->setImportSource(kChannelImportSource);
    kChannel->setImportReference("potassiumChannel");

    auto controllerImportSource = libcellml::ImportSource::create();
    controllerImportSource->setUrl("MembraneController.cellml");
    controller->setImportSource(controllerImportSource);
    controller->setImportReference("controller");

    auto leakageImportSource = libcellml::ImportSource::create();
    leakageImportSource->setUrl("LeakageModel.cellml");
    leakage->setImportSource(leakageImportSource);
    leakage->setImportReference("leakage");

    // Also import source for units. 
    auto unitsImportSource = libcellml::ImportSource::create();
    unitsImportSource->setUrl("Units.cellml");
    model->addImportSource(unitsImportSource);

    // Specify maths: sodium channel current
    auto mathHeader = "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" xmlns:cellml=\"http://www.cellml.org/cellml/2.0#\">\n";
    auto mathFooter = "</math>";

    auto equationITot = 
    "  <apply>\n"
    "    <eq/>\n"
    "    <ci>i_tot</ci>\n"
    "    <apply>\n"
    "      <plus/>\n"
    "      <ci>i_stim</ci>\n"
    "      <ci>i_K</ci>\n"
    "      <ci>i_Na</ci>\n"
    "      <ci>i_L</ci>\n"
    "    </apply>\n"
    "  </apply>\n";

    auto equationV = 
        "  <apply>\n"
        "    <eq/>\n"
        "    <apply>\n"
        "      <diff/>\n"
        "      <bvar>\n"
        "        <ci>t</ci>\n"
        "      </bvar>\n"
        "      <ci>V</ci>\n"
        "    </apply>\n"
        "    <apply>\n"
        "      <divide/>\n"
        "      <apply>\n"
        "       <times/>\n"
        "        <cn cellml:units=\"dimensionless\">-1</cn>\n"
        "        <ci>i_tot</ci>\n"
        "      </apply>\n"
        "      <ci>Cm</ci>\n"
        "    </apply>\n"
        "  </apply>\n";

    membraneEquations->setMath(mathHeader);
    membraneEquations->appendMath(equationV);
    membraneEquations->appendMath(equationITot);
    membraneEquations->appendMath(mathFooter);

    // Add variables.
    membraneEquations->addVariable(libcellml::Variable::create("V"));
    membraneEquations->addVariable(libcellml::Variable::create("t"));
    membraneEquations->addVariable(libcellml::Variable::create("i_tot"));
    membraneEquations->addVariable(libcellml::Variable::create("i_stim"));
    membraneEquations->addVariable(libcellml::Variable::create("i_Na"));
    membraneEquations->addVariable(libcellml::Variable::create("i_K"));
    membraneEquations->addVariable(libcellml::Variable::create("i_L"));
    membraneEquations->addVariable(libcellml::Variable::create("Cm"));

    // Import units
    auto mV = libcellml::Units::create("mV");
    mV->setImportReference("mV");
    mV->setImportSource(unitsImportSource);
    model->addUnits(mV);

    auto ms = libcellml::Units::create("ms");
    ms->setImportReference("ms");
    ms->setImportSource(unitsImportSource);
    model->addUnits(ms);

    auto per_ms = libcellml::Units::create("per_ms");
    per_ms->setImportReference("per_ms");
    per_ms->setImportSource(unitsImportSource);
    model->addUnits(per_ms);

    auto per_mV_ms = libcellml::Units::create("per_mV_ms");
    per_mV_ms->setImportReference("per_mV_ms");
    per_mV_ms->setImportSource(unitsImportSource);
    model->addUnits(per_mV_ms);

    auto microA_per_cm2 = libcellml::Units::create("microA_per_cm2");
    microA_per_cm2->setImportReference("microA_per_cm2");
    microA_per_cm2->setImportSource(unitsImportSource);
    model->addUnits(microA_per_cm2);

    auto mS_per_cm2 = libcellml::Units::create("mS_per_cm2");
    mS_per_cm2->setImportReference("mS_per_cm2");
    mS_per_cm2->setImportSource(unitsImportSource);
    model->addUnits(mS_per_cm2);

    auto microF_per_cm2 = libcellml::Units::create("microF_per_cm2");
    microF_per_cm2->setImportReference("microF_per_cm2");
    microF_per_cm2->setImportSource(unitsImportSource);
    model->addUnits(microF_per_cm2);

    membraneEquations->variable("V")->setUnits(mV);
    membraneEquations->variable("t")->setUnits(ms);
    membraneEquations->variable("i_stim")->setUnits(microA_per_cm2);
    membraneEquations->variable("i_tot")->setUnits(microA_per_cm2);
    membraneEquations->variable("i_L")->setUnits(microA_per_cm2);
    membraneEquations->variable("i_Na")->setUnits(microA_per_cm2);
    membraneEquations->variable("i_K")->setUnits(microA_per_cm2);
    membraneEquations->variable("Cm")->setUnits(microF_per_cm2);

    membrane->addVariable(membraneEquations->variable("t")->clone());
    membrane->addVariable(membraneEquations->variable("i_stim")->clone());
    membrane->addVariable(membraneEquations->variable("V")->clone());

    membraneParameters->addVariable(membraneEquations->variable("Cm")->clone());
    membraneParameters->variable("Cm")->setInitialValue(1);
    membraneParameters->addVariable(membraneEquations->variable("V")->clone());
    membraneParameters->variable("V")->setInitialValue(-85);

    auto validator = libcellml::Validator::create();
    validator->validateModel(model);
    printErrorsToTerminal(validator);

    auto importer = libcellml::Importer::create();
    importer->resolveImports(model, "");

    for(size_t i = 0; i < importer->libraryCount(); ++i) {
        std::cout << "Importer key "<<i<<" is "<<importer->key(i)<<std::endl;
    }

    // Dummy variables from imported components.
    for(size_t i = 0; i < model->importSourceCount(); ++i) {
        auto importSource = model->importSource(i);
        for(size_t c = 0; c < importSource->componentCount(); ++c) {
            auto sourceModel = importer->library(importSource->url());
            if(sourceModel == nullptr){
                continue;
            }
            auto dummyComponent = sourceModel->component(importSource->component(c)->importReference())->clone();
            while(dummyComponent->variableCount()) {
                importSource->component(c)->addVariable(dummyComponent->variable(0));
            }
        }
    }

    printModelToTerminal(model);

    // Connections everywhere
    assert(libcellml::Variable::addEquivalence(membraneParameters->variable("V"), membraneEquations->variable("V")));
    assert(libcellml::Variable::addEquivalence(membraneParameters->variable("Cm"), membraneEquations->variable("Cm")));
    assert(libcellml::Variable::addEquivalence(membrane->variable("t"), membraneEquations->variable("t")));
    assert(libcellml::Variable::addEquivalence(membrane->variable("t"), controller->variable("t")));
    assert(libcellml::Variable::addEquivalence(membrane->variable("V"), membraneEquations->variable("V")));
    assert(libcellml::Variable::addEquivalence(membrane->variable("i_stim"), membraneEquations->variable("i_stim")));
    assert(libcellml::Variable::addEquivalence(membrane->variable("i_stim"), controller->variable("i_stim")));

    assert(libcellml::Variable::addEquivalence(naChannel->variable("i_Na"), membraneEquations->variable("i_Na")));
    assert(libcellml::Variable::addEquivalence(naChannel->variable("t"), membraneEquations->variable("t")));
    assert(libcellml::Variable::addEquivalence(naChannel->variable("V"), membraneEquations->variable("V")));
    assert(libcellml::Variable::addEquivalence(kChannel->variable("i_K"), membraneEquations->variable("i_K")));
    assert(libcellml::Variable::addEquivalence(kChannel->variable("t"), membraneEquations->variable("t")));
    assert(libcellml::Variable::addEquivalence(kChannel->variable("V"), membraneEquations->variable("V")));

    assert(libcellml::Variable::addEquivalence(leakage->variable("i_L"), membraneEquations->variable("i_L")));
    assert(libcellml::Variable::addEquivalence(leakage->variable("V"), membraneEquations->variable("V")));

    model->fixVariableInterfaces();

    // Manually specify external interfaces after fixing.
    membrane->variable("V")->setInterfaceType("public_and_private");

    printModelToTerminal(model);
    validator->validateModel(model);
    printErrorsToTerminal(validator);

    auto flatModel = importer->flattenModel(model);
    printErrorsToTerminal(importer);

    printModelToTerminal(flatModel);

    auto analyser = libcellml::Analyser::create();
    analyser->analyseModel(flatModel);
    printErrorsToTerminal(analyser);
}
