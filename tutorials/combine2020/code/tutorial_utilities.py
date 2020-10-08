import string

from libcellml import Issue, CellmlElementType

def print_model(model, include_maths=False):
    print("The model name is: '{}'".format(model.name()))
    if model.id() != "":
        print("The model id is: '{}'".format(model.id()))

    print("The model defines {} custom units:".format(model.unitsCount()))
    for u in range(0, model.unitsCount()):
        print("  Units[{u}] is '{n}'".format(u=u, n=model.units(u).name()))

    print("The model has {n} components:".format(n=model.componentCount()))
    for c in range(0, model.componentCount()):
        component = model.component(c)
        spacer = "  "
        print_component_to_terminal(component, c, spacer, include_maths)

    return


def print_component_to_terminal(component, c, spacer, include_maths=False):
    local = '    '
    # Print this component
    print("{s}Component[{c}] has name: '{n}'".format(
        s=spacer,
        c=c,
        n=component.name()))
    if component.id() != "":
        print("{s}Component[{c}] has id: '{n}'".format(
            s=spacer,
            c=c,
            n=component.id()))

    # Print variables in this component
    for v in range(0, component.variableCount()):
        print(spacer + local + local, end='')
        print("[{}]: {}".format(v, component.variable(v).name()), end='')
        if component.variable(v).units() is not None:
            print(" [{}]".format(component.variable(v).units().name()), end='')
        if (component.variable(v).initialValue() != ""):
            print(", initial = {}".format(component.variable(v).initialValue()), end='')
        print()
        if component.variable(v).equivalentVariableCount() > 0:
            print(spacer + local + local + local, end='')
            con = "  └──> "
            for e in range(0,component.variable(v).equivalentVariableCount()):
                ev = component.variable(v).equivalentVariable(e)
                if ev is None:
                    print("WHOOPS! Null equivalent variable!")
                    continue
                
                ev_parent = ev.parent()
                if ev_parent is None:
                    print("WHOOPS! Null parent component for equivalent variable!")
                    continue
                
                print("{} {}:{}".format(con,ev_parent.name(),ev.name()), end='')
                if ev.units() is not None:
                    print(" [{}]".format(ev.units().name()), end='')
                
                con = ", "
            print()
        if include_maths:
            print(component.math())

    # Print the encapsulated components inside this one
    if component.componentCount() > 0:
        print("{s}Component[{c}] has {n} child components".format(
            s=spacer,
            c=c,
            n=component.componentCount()))

        for c2 in range(0, component.componentCount()):
            child = component.component(c2)
            one_more_spacer = spacer + "    "
            print_component_to_terminal(child, c2, one_more_spacer)


levelAsString = {
    Issue.Level.ERROR: "Error",
    Issue.Level.WARNING: "Warning",
    Issue.Level.HINT: "Hint",
    Issue.Level.MESSAGE: "Message"
}


def print_issues(item):
    number_of_issues = item.issueCount()

    if number_of_issues != 0:
        print("\nThe {t} has found {n} issues:".format(
            t=type(item).__name__,
            n=number_of_issues)
        )
        for e in range(0, number_of_issues):
            # Retrieve the issue item.
            i = item.issue(e)

            # Within the issue are a level, a URL, a reference heading (if appropriate), and
            # the item to which the issue applies.
            specification = i.referenceHeading()
            url = i.url()
            level = i.level()

            print("  {l}[{e}]:".format(
                l=levelAsString[level],
                e=e))
            print("    Description: {d}".format(
                d=i.description()))
            if specification != "":
                print("    See section {s} in the CellML specification.".format(
                    s=specification))
            if url != "":
                print("    More information is available at {url}".format(
                    url=url))
            print("    Stored item type: {}".format(get_item_type_from_enum(i.cellmlElementType())))
    else:
        print("\nThe {t} has not found any issues!".format(
            t=type(item).__name__)
        )


def print_component_only_to_terminal(component, spacer):
    
    print("{s}Component '{c}' has {n} child components".format(
        s=spacer,
        c=component.name(),
        n=component.componentCount()))

    for c in range(0, component.componentCount()):
        another_spacer = "    " + spacer
        child_component = component.component(c)
        print_component_only_to_terminal(child_component, another_spacer)


def print_encapsulation(model):
    # Prints the encapsulation structure of the model to the terminal
    spacer = "  - "
    print("Model '{m}' has {c} components".format(
        m=model.name(), c=model.componentCount()))

    for c in range(0, model.componentCount()):
        child_component = model.component(c)
        print_component_only_to_terminal(child_component, spacer)


def get_model_type_from_enum(my_type):

    my_type_as_string = "dunno"

    if my_type == Generator.ModelType.UNKNOWN:
        my_type_as_string = "UNKNOWN"
    elif my_type == Generator.ModelType.ALGEBRAIC:
        my_type_as_string = "ALGEBRAIC"
    elif my_type == Generator.ModelType.ODE:
        my_type_as_string = "ODE"
    elif my_type == Generator.ModelType.INVALID:
        my_type_as_string = "INVALID"
    elif my_type == Generator.ModelType.UNDERCONSTRAINED:
        my_type_as_string = "UNDERCONSTRAINED"
    elif my_type == Generator.ModelType.OVERCONSTRAINED:
        my_type_as_string = "OVERCONSTRAINED"
    elif my_type == Generator.ModelType.UNSUITABLY_CONSTRAINED:
        my_type_as_string = "UNSUITABLY_CONSTRAINED"

    return my_type_as_string


def get_profile_from_enum(my_type):

    my_type_as_string = "dunno"

    if my_type == GeneratorProfile.Profile.C:
        my_type_as_string = "C"
    elif my_type == GeneratorProfile.Profile.PYTHON:
        my_type_as_string = "PYTHON"

    return my_type_as_string


def get_issue_level_from_enum(my_level):

    my_type_as_string = "dunno"

    if my_level == Issue.Level.ERROR:
        my_type_as_string = "ERROR"

    elif my_level == Issue.Level.WARNING:
        my_type_as_string = "WARNING"
        
    elif my_level == Issue.Level.HINT:
        my_type_as_string = "HINT"

    elif my_level == Issue.Level.MESSAGE:
        my_type_as_string = "MESSAGE"

    return my_type_as_string


def get_item_type_from_enum(my_cause):

    my_type_as_string = "dunno"

    if my_cause == CellmlElementType.COMPONENT:
        my_type_as_string = "COMPONENT"

    elif my_cause == CellmlElementType.COMPONENT_REF:
        my_type_as_string = "COMPONENT_REf"

    elif my_cause == CellmlElementType.CONNECTION:
        my_type_as_string = "CONNECTION"

    elif my_cause == CellmlElementType.ENCAPSULATION:
        my_type_as_string = "ENCAPSULATION"

    elif my_cause == CellmlElementType.IMPORT:
        my_type_as_string = "IMPORT"

    elif my_cause == CellmlElementType.MATH:
        my_type_as_string = "MATH"

    elif my_cause == CellmlElementType.MAP_VARIABLES:
        my_type_as_string = "MAP_VARIABLES"

    elif my_cause == CellmlElementType.MODEL:
        my_type_as_string = "MODEL"

    elif my_cause == CellmlElementType.RESET:
        my_type_as_string = "RESET"

    elif my_cause == CellmlElementType.RESET_VALUE:
        my_type_as_string = "RESET_VALUE"

    elif my_cause == CellmlElementType.TEST_VALUE:
        my_type_as_string = "TEST_VALUE"

    elif my_cause == CellmlElementType.UNDEFINED:
        my_type_as_string = "UNDEFINED"

    elif my_cause == CellmlElementType.UNIT:
        my_type_as_string = "UNIT"

    elif my_cause == CellmlElementType.UNITS:
        my_type_as_string = "UNITS"

    elif my_cause == CellmlElementType.VARIABLE:
        my_type_as_string = "VARIABLE"

    return my_type_as_string


def list_equivalent_variables(variable, variable_set):
    if variable is None:
        return
    for i in range(0, variable.equivalentVariableCount()):
        equivalent_variable = variable.equivalentVariable(i)
        if equivalent_variable not in variable_set:
            variable_set.push_back(equivalent_variable)
            list_equivalent_variables(equivalent_variable, variable_set)


def print_equivalent_variable_set(variable):

    if variable is None:
        print("None variable submitted to print_equivalent_variable_set.")
        return

    variable_set = set()
    variable_set.add(variable)
    list_equivalent_variables(variable, variable_set)

    component = variable.parent()
    print_me = ''
    if component != None:
        print_me += "Tracing: {c}.{v}".format(
            c=component.name(), v=variable.name())
        if variable.units() is not None:
            print_me += " [{u}]".format(u=variable.units().name())

        if variable.initialValue() != "":
            print_me += ", initial = {i}".format(i=variable.initialValue())

        print(print_me)

    if len(variable_set) > 1:
        for e in variable_set:
            print_me = ''
            component = e.parent()
            if component is not None:
                print_me += "    - {c}.{v}".format(
                    c=component.name(), v=e.name())
                if e.units() is not None:
                    print_me += " [{u}]".format(u=e.units().name())
                if e.initialValue() != "":
                    print_me += ", initial = {i}".format(i=e.initialValue())
                print(print_me)
            else:
                print(
                    "Variable {v} does not have a parent component.".format(v=e.name()))
        else:
            print("    - Not connected to any equivalent variables.")
