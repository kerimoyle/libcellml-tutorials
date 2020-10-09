/* The content of this file was generated using a modified C profile of libCellML 0.2.0. */

#include "HodgkinHuxleyModel.h"

#include <math.h>
#include <stdlib.h>

const char VERSION[] = "0.1.0.post0";
const char LIBCELLML_VERSION[] = "0.2.0";

const size_t STATE_COUNT = 4;
const size_t VARIABLE_COUNT = 19;

const VariableInfo VOI_INFO = {"t", "ms", "controller"};

const VariableInfo STATE_INFO[] = {
    {"X", "dimensionless", "importedGateM"},
    {"X", "dimensionless", "importedGateH"},
    {"X", "dimensionless", "importedGate"},
    {"V", "mV", "membraneEquations"}
};

const VariableInfoWithType VARIABLE_INFO[] = {
    {"E_L", "mV", "leakageParameters", CONSTANT},
    {"g_L", "mS_per_cm2", "leakageParameters", CONSTANT},
    {"Cm", "microF_per_cm2", "membraneParameters", CONSTANT},
    {"E_K", "mV", "potassiumChannelParameters", CONSTANT},
    {"g_K", "milliS_per_cm2", "potassiumChannelParameters", CONSTANT},
    {"E_Na", "mV", "sodiumChannelParameters", CONSTANT},
    {"g_Na", "mS_per_cm2", "sodiumChannelParameters", CONSTANT},
    {"i_stim", "microA_per_cm2", "controller", ALGEBRAIC},
    {"Na_conductance", "mS_per_cm2", "sodiumChannelEquations", ALGEBRAIC},
    {"i_Na", "microA_per_cm2", "sodiumChannelEquations", ALGEBRAIC},
    {"alpha_m", "per_ms", "mGateEquations", ALGEBRAIC},
    {"beta_m", "per_ms", "mGateEquations", ALGEBRAIC},
    {"alpha_h", "per_ms", "hGateEquations", ALGEBRAIC},
    {"beta_h", "per_ms", "hGateEquations", ALGEBRAIC},
    {"i_K", "microA_per_cm2", "potassiumChannelEquations", ALGEBRAIC},
    {"alpha_n", "per_ms", "nGateEquations", ALGEBRAIC},
    {"beta_n", "per_ms", "nGateEquations", ALGEBRAIC},
    {"i_L", "microA_per_cm2", "leakageEquations", ALGEBRAIC},
    {"i_tot", "microA_per_cm2", "membraneEquations", ALGEBRAIC}
};

double * createStatesArray()
{
    return (double *) malloc(STATE_COUNT*sizeof(double));
}

double * createVariablesArray()
{
    return (double *) malloc(VARIABLE_COUNT*sizeof(double));
}

void deleteArray(double *array)
{
    free(array);
}

void initialiseStatesAndConstants(double *states, double *variables)
{
    variables[0] = -64.387;
    variables[1] = 0.3;
    variables[2] = 1.0;
    variables[3] = -87.0;
    variables[4] = 36.0;
    variables[5] = 40.0;
    variables[6] = 120.0;
    states[0] = 0.05;
    states[1] = 0.6;
    states[2] = 0.325;
    states[3] = 1.0;
}

void computeComputedConstants(double *variables)
{
}

void computeRates(double voi, double *states, double *rates, double *variables)
{
    variables[10] = -0.1*(states[3]+50.0)/(exp(-(states[3]+50.0)/10.0)-1.0);
    variables[11] = 4.0*exp(-(states[3]+75.0)/18.0);
    rates[0] = variables[10]*(1.0-states[0])-variables[11]*states[0];
    variables[12] = 0.07*exp(-(states[3]+75.0)/20.0);
    variables[13] = 1.0/(exp(-(states[3]+45.0)/10.0)+1.0);
    rates[1] = variables[12]*(1.0-states[1])-variables[13]*states[1];
    variables[15] = 0.01*(states[3]+10.0)/(exp((states[3]+10.0)/10.0)-1.0);
    variables[16] = 0.125*exp(states[3]/80.0);
    rates[2] = variables[15]*(1.0-states[2])-variables[16]*states[2];
    variables[7] = (voi < 1.0)?0.0:(voi > 1.2)?0.0:100.0;
    variables[17] = variables[1]*(states[3]-variables[0]);
    variables[8] = variables[6]*states[1]*pow(states[0], 3.0);
    variables[9] = variables[8]*(states[3]-variables[5]);
    variables[14] = pow(states[2], 4.0)*variables[4]*(states[3]-variables[3]);
    variables[18] = variables[7]+variables[14]+variables[9]+variables[17];
    rates[3] = -1.0*variables[18]/variables[2];
}

void computeVariables(double voi, double *states, double *rates, double *variables)
{
}
