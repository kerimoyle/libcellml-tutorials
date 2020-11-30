
/**
 *  TUTORIAL UTILITIES
 *  This file contains helper functions for the tutorials.
 */

#include <algorithm>
#include <map>

// Includes for creating a new directory
#include <string> // required for std::string
#include <sys/types.h> // required for stat.h
#include <sys/stat.h> // no clue why required -- man pages say so

#include <libcellml>

#include "libcellml/types.h"
#include "utilities.h"


void printModel(libcellml::ModelPtr &model)
{
    printModel(model, false);
}

void printModel(libcellml::ModelPtr &model, bool includeMaths)
{
    if(model == nullptr) {
        std::cout << "Model requested is a nullptr." << std::endl;
        return;
    }
    std::string spacer = "    ";

    std::cout << " MODEL: '" << model->name() << "'";
    if (model->id() != "") {
        std::cout << ", id: '" << model->id() << "'";
    }
    std::cout << std::endl;

    std::cout << spacer << "UNITS: " << model->unitsCount() << " custom units" << std::endl;
    for (size_t u = 0; u < model->unitsCount(); ++u) {
        std::cout << spacer << spacer << "[" << u << "]: " << model->units(u)->name();
        if(model->units(u)->isImport()) {
            std::cout << ", imported from: '";
            std::cout << model->units(u)->importReference();
            std::cout << "' in '"<<model->units(u)->importSource()->url() << "'";
        }
        std::cout << std::endl;
    }

    std::cout << spacer << "COMPONENTS: " << model->componentCount() << " components" << std::endl;
    for (size_t c = 0; c < model->componentCount(); ++c) {
        auto component = model->component(c);
        printComponentToTerminal(component, c, spacer + spacer, includeMaths);
    }
}

void printComponentToTerminal(const libcellml::ComponentPtr &component, size_t const c, std::string const spacer)
{
    printComponentToTerminal(component, c, spacer, true);
}

void printComponentToTerminal(const libcellml::ComponentPtr &component, size_t const c, std::string const spacer, bool includeMaths)
{
    std::string local = "    ";

    std::cout << spacer << "[" << c << "]: " << component->name();
    if (component->id() != "") {
        std::cout << " id: " << component->id();
    }
    if(component->isImport()) {
            std::cout << " <--- imported from: '";
            std::cout << component->importReference();
            std::cout << "' in '" << component->importSource()->url() << "'";
    }
    std::cout << std::endl;

    std::cout << spacer << local << "VARIABLES: " << component->variableCount() << " variables" << std::endl;

    // Printing the variables within the component.
    for (size_t v = 0; v < component->variableCount(); v++) {
        std::cout << spacer << local << local;
        std::cout << "[" << v << "]: " << component->variable(v)->name();
        if (component->variable(v)->units() != nullptr) {
            std::cout << " [" << component->variable(v)->units()->name() << "]";
        }
        if (component->variable(v)->initialValue() != "") {
            std::cout << ", initial = " << component->variable(v)->initialValue();
        }
        std::cout << std::endl;
        if (component->variable(v)->equivalentVariableCount() > 0) {
            std::cout << spacer << local << local << local;
            std::string con = "  └──> ";
            for (size_t e = 0; e < component->variable(v)->equivalentVariableCount(); ++e) {
                auto ev = component->variable(v)->equivalentVariable(e);
                if (ev == nullptr) {
                    std::cout << "WHOOPS! Null equivalent variable!";
                    continue;
                }
                libcellml::ComponentPtr ev_parent = std::dynamic_pointer_cast<libcellml::Component>(ev->parent());
                if (ev_parent == nullptr) {
                    std::cout << "WHOOPS! Null parent component for equivalent variable!";
                    continue;
                }
                std::cout << con << ev_parent->name() << ":" << ev->name();
                if (ev->units() != nullptr) {
                    std::cout << " [" << ev->units()->name() << "]";
                }
                con = ", ";
            }
            std::cout << std::endl;
        }
    }

    // Print the maths within the component.
    if (includeMaths) {
        if (component->math() != "") {
            std::cout << spacer << "  Maths in the component is:" << std::endl;
            std::cout << component->math() << std::endl;
        }
    }

    // Print the encapsulated components
    if (component->componentCount() > 0) {
        std::cout << spacer << local << "COMPONENT " << component->name() << " has "
                  << component->componentCount()
                  << " child components:" << std::endl;

        for (size_t c2 = 0; c2 < component->componentCount(); c2++) {
            auto child = component->component(c2);
            std::string oneMoreSpacer = spacer + local + local;
            printComponentToTerminal(child, c2, oneMoreSpacer, includeMaths);
        }
    }
}

void printIssues(const libcellml::LoggerPtr &item) {
    int num = item->issueCount();
    std::cout << "Recorded " << num << " issues";
    if (num != 0) {
        std::cout << ":" << std::endl;
        for (size_t i = 0; i < num; ++i) {
            libcellml::IssuePtr issue = item->issue(i);
            std::string errorReference = issue->referenceHeading();

            std::cout << "Issue " << i << " is " << getIssueLevelFromEnum(issue->level()) << ":" << std::endl;
            std::cout << "    description: " << issue->description() << std::endl;
            if (errorReference != "") {
                std::cout << "    see section " << errorReference
                          << " in the CellML specification." << std::endl;
            }
            if(!issue->url().empty()){
                std::cout << "    more information at: " <<issue->url() << std::endl;
            }
            std::cout << "    stored item type: " << getCellmlElementTypeFromEnum(issue->cellmlElementType()) << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
    else {
        std::cout << "!" << std::endl << std::endl;
    }
}

std::string fileContents(const std::string &fileName)
{
    std::ifstream file(fileName);
    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

std::map<libcellml::CellmlElementType, std::string> itemTypeToString = 
    {{libcellml::CellmlElementType::COMPONENT, "COMPONENT"},
    {libcellml::CellmlElementType::COMPONENT_REF, "COMPONENT_REF"},
    {libcellml::CellmlElementType::CONNECTION, "CONNECTION"},
    {libcellml::CellmlElementType::ENCAPSULATION, "ENCAPSULATION" },
    {libcellml::CellmlElementType::IMPORT, "IMPORT"},
    {libcellml::CellmlElementType::MAP_VARIABLES, "MAP_VARIABLES"},
    {libcellml::CellmlElementType::MATH, "MATH"},
    {libcellml::CellmlElementType::MODEL, "MODEL"},
    {libcellml::CellmlElementType::RESET, "RESET"},
    {libcellml::CellmlElementType::RESET_VALUE, "RESET_VALUE"},
    {libcellml::CellmlElementType::TEST_VALUE, "TEST_VALUE"},
    {libcellml::CellmlElementType::UNDEFINED, "UNDEFINED"},
    {libcellml::CellmlElementType::UNIT, "UNIT"},
    {libcellml::CellmlElementType::UNITS, "UNITS"}, 
    {libcellml::CellmlElementType::VARIABLE, "VARIABLE"}};

std::string getCellmlElementTypeFromEnum(libcellml::CellmlElementType t) {
    return itemTypeToString.at(t);
}

std::string getIssueLevelFromEnum(libcellml::Issue::Level myLevel)
{
    std::string myTypeAsString = "dunno";

    switch (myLevel) {
    case libcellml::Issue::Level::ERROR:
        myTypeAsString = "an ERROR";
        break;
    case libcellml::Issue::Level::WARNING:
        myTypeAsString = "a WARNING";
        break;
    case libcellml::Issue::Level::HINT:
        myTypeAsString = "a HINT";
        break;
    case libcellml::Issue::Level::MESSAGE:
        myTypeAsString = "a MESSAGE";
        break;
    }
    return myTypeAsString;
}

void printComponentOnlyToTerminal(libcellml::ComponentPtr &component, std::string spacer)
{
    std::cout << spacer << "Component '" << component->name() << "' has " << component->componentCount() << " child components" << std::endl;
    for (size_t c = 0; c < component->componentCount(); c++) {
        std::string anotherSpacer = "    " + spacer;
        auto child = component->component(c);
        printComponentOnlyToTerminal(child, anotherSpacer);
    }
}

void printEncapsulation(libcellml::ModelPtr &model)
{
    // Prints the encapsulation structure of the model to the terminal
    std::string spacer = "  - ";

    std::cout << "Model '" << model->name() << "' has " << model->componentCount()
              << " components" << std::endl;

    for (size_t c = 0; c < model->componentCount(); ++c) {
        auto child = model->component(c);
        printComponentOnlyToTerminal(child, spacer);
    }
}

void listEquivalentVariables(const libcellml::VariablePtr &variable, std::vector<libcellml::VariablePtr> &variableList)
{
    if (variable == nullptr) {
        return;
    }

    for (size_t i = 0; i < variable->equivalentVariableCount(); ++i) {
        auto equivalentVariable = variable->equivalentVariable(i);
        if (std::find(variableList.begin(), variableList.end(), equivalentVariable) == variableList.end()) {
            variableList.push_back(equivalentVariable);
            listEquivalentVariables(equivalentVariable, variableList);
        }
    }
}

void printEquivalentVariableSet(const libcellml::VariablePtr &variable)
{
    if (variable == nullptr) {
        std::cout << "NULL variable submitted to printEquivalentVariableSet." << std::endl;
        return;
    }
    std::vector<libcellml::VariablePtr> variableList;
    variableList.push_back(variable);
    listEquivalentVariables(variable, variableList);

    libcellml::ComponentPtr component = std::dynamic_pointer_cast<libcellml::Component>(variable->parent());

    if (component != nullptr) {
        std::cout << "Tracing: " << component->name() << " -> "
                  << variable->name();
        if (variable->units() != nullptr) {
            std::cout << " [" << variable->units()->name() << "]";
        }
        if (variable->initialValue() != "") {
            std::cout << ", initial = " << variable->initialValue();
        }
        std::cout << std::endl;
    }

    if (variableList.size() > 1) {
        for (auto &e : variableList) {
            component = std::dynamic_pointer_cast<libcellml::Component>(e->parent());
            if (component != nullptr) {
                std::cout << "    - " << component->name() << " -> " << e->name();
                if (e->units() != nullptr) {
                    std::cout << " [" << e->units()->name() << "]";
                }
                if (e->initialValue() != "") {
                    std::cout << ", initial = " << e->initialValue();
                }
                std::cout << std::endl;
            } else {
                std::cout << "Variable " << e->name() << " does not have a parent component." << std::endl;
            }
        }
    } else {
        std::cout << "    - Not connected to any equivalent variables." << std::endl;
    }
}

void doPrintImportDependencies(const libcellml::ModelPtr &model, std::string &spacer) {
    // Function to recursively iterate through the import dependencies in this model, and 
    // print their URL and what they require to the terminal.
    if(model->hasUnresolvedImports() || (model->importSourceCount() == 0)) {
        return;
    }
    std::cout << spacer << "Model '" << model->name() << "' imports:" << std::endl;
    for(size_t i = 0; i < model->importSourceCount(); ++i) {
        // Each import source should have its own model pointer attached now.
        auto importSource = model->importSource(i);
        std::cout << spacer << "   From "<< importSource->url() << ":" << std::endl;
        for(size_t u = 0; u < importSource->unitsCount(); ++u){
            std::cout << spacer << "    - units "<<importSource->units(u)->name() << " <- "<<importSource->units(u)->importReference()<<std::endl;
        }
        for(size_t c = 0; c < importSource->componentCount(); ++c){
            std::cout << spacer << "    - component "<<importSource->component(c)->name() << " <- "<<importSource->component(c)->importReference()<<std::endl;
        }
        std::string bigSpacer = spacer + "    ";
        doPrintImportDependencies(importSource->model(), bigSpacer);
    }
}

void printImportDependencies(const libcellml::ModelPtr &model){
    std::string spacer = " ";
    doPrintImportDependencies(model, spacer);
}

bool makeDirectory(const std::string &sPath) {
    // std::string sPath = "/tmp/test";
    mode_t nMode = 0733; // UNIX style permissions
    int nError = 0;
    #if defined(_WIN32)
    nError = _mkdir(sPath.c_str()); // can be used on Windows
    #else 
    nError = mkdir(sPath.c_str(), nMode); // can be used on non-Windows
    #endif
    if (nError != 0) {
        // std::cout << "OOPS! Something went wrong trying to create directory: " << sPath << std::endl;
        return false;
    }
    return true;
}