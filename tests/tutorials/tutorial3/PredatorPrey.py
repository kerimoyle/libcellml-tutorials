# The content of this file was generated using the Python profile of libCellML 0.2.0.

from enum import Enum
from math import *


__version__ = "0.1.0"
LIBCELLML_VERSION = "0.2.0"

STATE_COUNT = 2
VARIABLE_COUNT = 4


class VariableType(Enum):
    CONSTANT = 1
    COMPUTED_CONSTANT = 2
    ALGEBRAIC = 3


VOI_INFO = {"name": "time", "units": "month", "component": "predator_prey_component"}

STATE_INFO = [
    {"name": "y_s", "units": "number_of_sharks", "component": "predator_prey_component"},
    {"name": "y_f", "units": "thousands_of_fish", "component": "predator_prey_component"}
]

VARIABLE_INFO = [
    {"name": "a", "units": "per_month", "component": "predator_prey_component", "type": VariableType.CONSTANT},
    {"name": "b", "units": "per_shark_month", "component": "predator_prey_component", "type": VariableType.CONSTANT},
    {"name": "d", "units": "per_1000fish_month", "component": "predator_prey_component", "type": VariableType.CONSTANT},
    {"name": "c", "units": "per_month", "component": "predator_prey_component", "type": VariableType.COMPUTED_CONSTANT}
]


def create_states_array():
    return [nan]*STATE_COUNT


def create_variables_array():
    return [nan]*VARIABLE_COUNT


def initialise_states_and_constants(states, variables):
    variables[0] = -0.8
    variables[1] = 0.3
    variables[2] = -0.6
    states[0] = 1.0
    states[1] = 2.0


def compute_computed_constants(variables):
    variables[3] = variables[0]+2.0


def compute_rates(voi, states, rates, variables):
    rates[0] = variables[0]*states[0]+variables[1]*states[0]*states[1]
    rates[1] = variables[3]*states[1]+variables[2]*states[0]*states[1]


def compute_variables(voi, states, rates, variables):
    pass
