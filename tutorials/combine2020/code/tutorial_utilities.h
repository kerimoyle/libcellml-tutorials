/**
 *  TUTORIAL 2: Utilities
 *  This file contains helper functions for Tutorial 2
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <libcellml>

void printModel(libcellml::ModelPtr &model);
void printModel(libcellml::ModelPtr &model, bool includeMaths);
void printIssues(const libcellml::LoggerPtr &item);
void printComponentToTerminal(const libcellml::ComponentPtr &component, size_t const c, std::string const spacer);
void printComponentToTerminal(const libcellml::ComponentPtr &component, size_t const c, std::string const spacer, bool includeMaths);

void printComponentOnlyToTerminal(libcellml::ComponentPtr &component, std::string spacer);
void printEncapsulation(libcellml::ModelPtr &model);

std::string fileContents(const std::string &fileName);

std::string getCellmlElementTypeFromEnum(libcellml::CellmlElementType t);
std::string getIssueLevelFromEnum(libcellml::Issue::Level myLevel);

void solveWithEuler(double stepSize, int stepCount, std::string outFileName);