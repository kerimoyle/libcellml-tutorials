/**
 *  TUTORIAL 2: Utilities
 *  This file contains helper functions for Tutorial 2
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <libcellml>

/**
 * @brief Prints the basic contents of a Model to the terminal
 */
void printModel(libcellml::ModelPtr &model);
void printModel(libcellml::ModelPtr &model, bool includeMaths);

/**
 * @brief Prints the validation errors to the terminal
 */
void printIssues(const libcellml::LoggerPtr &item);
void printErrorsToTerminal(libcellml::ValidatorPtr &item);
void printErrorsToTerminal(libcellml::AnalyserPtr &item);
void printErrorsToTerminal(libcellml::ImporterPtr &item);
void printErrorsToTerminal(libcellml::ParserPtr &item);
void printComponentToTerminal(const libcellml::ComponentPtr &component, size_t const c, std::string const spacer);
void printComponentToTerminal(const libcellml::ComponentPtr &component, size_t const c, std::string const spacer, bool includeMaths);
void printEncapsulation(libcellml::ModelPtr &model);
void printComponentOnlyToTerminal(libcellml::ComponentPtr &component, std::string spacer);

std::string fileContents(const std::string &fileName);

void switchUnitsInMaths(std::string &maths, std::string in, std::string out);
void insertIntoMathMLString(std::string &maths, std::string &addMe);

// std::string getModelTypeFromEnum(libcellml::Generator::ModelType myType);
// std::string getProfileFromEnum(libcellml::GeneratorProfile::Profile myType);

std::string getItemTypeAsString(libcellml::ItemType t);

void solveWithEuler(double stepSize, int stepCount, std::string outFileName);

void listEquivalentVariables(const libcellml::VariablePtr &variable, std::vector<libcellml::VariablePtr> &variableList);

void printEquivalentVariableSet(const libcellml::VariablePtr &variable);

void printImportDependencies(const libcellml::ModelPtr &model);

void connectAllVariablesWithSameName(const libcellml::ModelPtr &model, const libcellml::VariablePtr &v);
