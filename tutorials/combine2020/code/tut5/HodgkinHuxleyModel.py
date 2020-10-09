# The content of this file was generated using the Python profile of libCellML 0.2.0.

from enum import Enum
from math import *


__version__ = "0.1.0"
LIBCELLML_VERSION = "0.2.0"

STATE_COUNT = 4
VARIABLE_COUNT = 19


class VariableType(Enum):
    CONSTANT = 1
    COMPUTED_CONSTANT = 2
    ALGEBRAIC = 3


VOI_INFO = {"name": "t", "units": "ms", "component": "controller"}

STATE_INFO = [
    {"name": "X", "units": "dimensionless", "component": "importedGateM"},
    {"name": "X", "units": "dimensionless", "component": "importedGateH"},
    {"name": "X", "units": "dimensionless", "component": "importedGate"},
    {"name": "V", "units": "mV", "component": "membraneEquations"}
]

VARIABLE_INFO = [
    {"name": "E_L", "units": "mV", "component": "leakageParameters", "type": VariableType.CONSTANT},
    {"name": "g_L", "units": "mS_per_cm2", "component": "leakageParameters", "type": VariableType.CONSTANT},
    {"name": "Cm", "units": "microF_per_cm2", "component": "membraneParameters", "type": VariableType.CONSTANT},
    {"name": "E_K", "units": "mV", "component": "potassiumChannelParameters", "type": VariableType.CONSTANT},
    {"name": "g_K", "units": "milliS_per_cm2", "component": "potassiumChannelParameters", "type": VariableType.CONSTANT},
    {"name": "E_Na", "units": "mV", "component": "sodiumChannelParameters", "type": VariableType.CONSTANT},
    {"name": "g_Na", "units": "mS_per_cm2", "component": "sodiumChannelParameters", "type": VariableType.CONSTANT},
    {"name": "i_stim", "units": "microA_per_cm2", "component": "controller", "type": VariableType.ALGEBRAIC},
    {"name": "Na_conductance", "units": "mS_per_cm2", "component": "sodiumChannelEquations", "type": VariableType.ALGEBRAIC},
    {"name": "i_Na", "units": "microA_per_cm2", "component": "sodiumChannelEquations", "type": VariableType.ALGEBRAIC},
    {"name": "alpha_m", "units": "per_ms", "component": "mGateEquations", "type": VariableType.ALGEBRAIC},
    {"name": "beta_m", "units": "per_ms", "component": "mGateEquations", "type": VariableType.ALGEBRAIC},
    {"name": "alpha_h", "units": "per_ms", "component": "hGateEquations", "type": VariableType.ALGEBRAIC},
    {"name": "beta_h", "units": "per_ms", "component": "hGateEquations", "type": VariableType.ALGEBRAIC},
    {"name": "i_K", "units": "microA_per_cm2", "component": "potassiumChannelEquations", "type": VariableType.ALGEBRAIC},
    {"name": "alpha_n", "units": "per_ms", "component": "nGateEquations", "type": VariableType.ALGEBRAIC},
    {"name": "beta_n", "units": "per_ms", "component": "nGateEquations", "type": VariableType.ALGEBRAIC},
    {"name": "i_L", "units": "microA_per_cm2", "component": "leakageEquations", "type": VariableType.ALGEBRAIC},
    {"name": "i_tot", "units": "microA_per_cm2", "component": "membraneEquations", "type": VariableType.ALGEBRAIC}
]


def leq_func(x, y):
    return 1.0 if x <= y else 0.0


def geq_func(x, y):
    return 1.0 if x >= y else 0.0


def and_func(x, y):
    return 1.0 if bool(x) & bool(y) else 0.0


def create_states_array():
    return [nan]*STATE_COUNT


def create_variables_array():
    return [nan]*VARIABLE_COUNT


def initialise_states_and_constants(states, variables):
    variables[0] = -64.387
    variables[1] = 0.3
    variables[2] = 1.0
    variables[3] = -87.0
    variables[4] = 36.0
    variables[5] = 40.0
    variables[6] = 120.0
    states[0] = 0.05
    states[1] = 0.6
    states[2] = 0.325
    states[3] = -75.0


def compute_computed_constants(variables):
    pass


def compute_rates(voi, states, rates, variables):
    variables[10] = -0.1*(states[3]+50.0)/(exp(-(states[3]+50.0)/10.0)-1.0)
    variables[11] = 4.0*exp(-(states[3]+75.0)/18.0)
    rates[0] = variables[10]*(1.0-states[0])-variables[11]*states[0]
    variables[12] = 0.07*exp(-(states[3]+75.0)/20.0)
    variables[13] = 1.0/(exp(-(states[3]+45.0)/10.0)+1.0)
    rates[1] = variables[12]*(1.0-states[1])-variables[13]*states[1]
    variables[15] = -0.01*(states[3]+65.0)/(exp((states[3]+65.0)/-10.0)-1.0)
    variables[16] = 0.125*exp((states[3]+75.0)/-80.0)
    rates[2] = variables[15]*(1.0-states[2])-variables[16]*states[2]
    variables[7] = 20.0 if and_func(geq_func(voi, 10.0), leq_func(voi, 10.5)) else 0.0
    variables[17] = variables[1]*(states[3]-variables[0])
    variables[8] = variables[6]*states[1]*pow(states[0], 3.0)
    variables[9] = variables[8]*(states[3]-variables[5])
    variables[14] = pow(states[2], 4.0)*variables[4]*(states[3]-variables[3])
    variables[18] = -variables[7]+variables[14]+variables[9]+variables[17]
    rates[3] = -1.0*variables[18]/variables[2]


def compute_variables(voi, states, rates, variables):
    pass
