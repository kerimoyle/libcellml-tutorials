.. _runtime_codes:

==============
Run-time codes
==============

Many of the classes in libCellML inherit a :code:`Logger` class, which keeps track of things that may have gone wrong, as well as giving hints when the behaviour may not be as expected.
This page keeps a list of the codes that are not related to CellML syntactic validation.


Issues related to validation are directed to the relevant section in the :cellml2:`CellML 2.0 Normative Specification document<>` instead, and are listed at the bottom of this page for completeness.

Analyser codes
==============

.. _ANALYSER_VARIABLE_INITIALISED_MORE_THAN_ONCE:

.. container:: issue-code
    
    ANALYSER_VARIABLE_INITIALISED_MORE_THAN_ONCE


.. _ANALYSER_VARIABLE_NON_CONSTANT_INITIALISATION:

.. container:: issue-code
    
    ANALYSER_VARIABLE_NON_CONSTANT_INITIALISATION


.. _ANALYSER_VOI_INITIALISED:

.. container:: issue-code
    
    ANALYSER_VOI_INITIALISED


.. _ANALYSER_VOI_SEVERAL:

.. container:: issue-code
    
    ANALYSER_VOI_SEVERAL


.. _ANALYSER_ODE_NOT_FIRST_ORDER:

.. container:: issue-code
    
    ANALYSER_ODE_NOT_FIRST_ORDER


.. _ANALYSER_VARIABLE_UNUSED:

.. container:: issue-code
    
    ANALYSER_VARIABLE_UNUSED


.. _ANALYSER_STATE_NOT_INITIALISED:

.. container:: issue-code
    
    ANALYSER_STATE_NOT_INITIALISED


.. _ANALYSER_VARIABLE_COMPUTED_MORE_THAN_ONCE:

.. container:: issue-code
    
    ANALYSER_VARIABLE_COMPUTED_MORE_THAN_ONCE


.. _ANALYSER_EXTERNAL_VARIABLE_DIFFERENT_MODEL:

.. container:: issue-code
    
    ANALYSER_EXTERNAL_VARIABLE_DIFFERENT_MODEL


.. _ANALYSER_EXTERNAL_VARIABLE_VOI:

.. container:: issue-code
    
    ANALYSER_EXTERNAL_VARIABLE_VOI


.. _ANALYSER_EXTERNAL_VARIABLE_USE_PRIMARY_VARIABLE:

.. container:: issue-code
    
    ANALYSER_EXTERNAL_VARIABLE_USE_PRIMARY_VARIABLE


Annotator codes
===============

.. _ANNOTATOR_NO_MODEL:

.. container:: issue-code

    ANNOTATOR_NO_MODEL

The :code:`Annotator` class needs a :code:`Model` item on which to operate.
If you're seeing an issue with this code, it's because you need to use the :code:`setModel` function to add it.

.. container:: shortlist

    **API** :api:`Annotator::setModel function<Annotator?fName=setModel>`

    **How-to** :ref:`Annotate a model<annotate_model>`

    **Common users** :ref:`Annotation tool developer<users_annotation_tool_dev>`
        

.. _ANNOTATOR_NULL_MODEL:

.. container:: issue-code

    ANNOTATOR_NULL_MODEL

This code is generated when the :code:`Annotator` is given a null pointer instead of a :code:`Model` item.

.. container:: shortlist

    **API** :api:`Annotator::setModel function<Annotator?fName=setModel>`

    **How-to** :ref:`Annotate a model<annotate_model>`

    **Tutorial** :ref:`Tutorial 5: Annotation<tutorial5>`

    **Common users** :ref:`Annotation tool developer<users_annotation_tool_dev>`


.. _ANNOTATOR_ID_NOT_FOUND:

.. container:: issue-code
    
    ANNOTATOR_ID_NOT_FOUND

When the :code:`Model` that was submitted to this :code:`Annotator` does not have any item with the requested id string, a warning with this code is created.

.. container:: shortlist

    **API** :api:`Annotator::item function<Annotator?fName=item>` (or any of the type-specific retrieval functions)

    **How-to** :ref:`Annotate a model<annotate_model>`

    **Tutorial** :ref:`Tutorial 5: Annotation<tutorial5>`

    **Common users** :ref:`Annotation tool developer<users_annotation_tool_dev>`

.. container:: nb

    Please note that currently the :code:`Annotator` class does not check for ids within MathML strings.
    

.. _ANNOTATOR_ID_NOT_UNIQUE:

.. container:: issue-code
    
    ANNOTATOR_ID_NOT_UNIQUE 

While the :code:`Annotator` is able to retrieve and manipulate collections of items with the same id, any XML document with duplicated ids is not valid.  
When non-unique id strings are encountered, a warning with this code is created.

.. container:: shortlist

    **How-to** :ref:`Handle duplicated ID attributes<handle_duplicated_id_attributes>`

    **How-to** :ref:`Annotate a model<annotate_model>`

    **Tutorial** :ref:`Tutorial 5: Annotation<tutorial5>`

    **Common users** :ref:`Annotation tool developer<users_annotation_tool_dev>`


.. _ANNOTATOR_INCONSISTENT_TYPE:

.. container:: issue-code

    ANNOTATOR_INCONSISTENT_TYPE 

In contrast to all other classes in libCellML, the :code:`Annotator` must be agnostic as to the type of item it deals with.
It uses :code:`AnyItem` type in order to be able to return items without specifying their type ahead of time.
An error is issued when the information stored in an :code:`AnyItem` item (the :code:`CellmlElementType` of the item, and the item itself) are not consistent.

This issue is raised when attempting to use the automatic id feature of the :code:`Annotator`, and can be rectified by checking that any manually created :code:`AnyItem` items have the correct pairing of :code:`CellmlElementType` and object.
The expected pairings are shown below.

.. table::

    ===================== ====================
    CellmlElementType     libCellML class
    ===================== ====================
    :code:`COMPONENT`     :code:`Component`
    :code:`COMPONENT_REF` :code:`Component`
    :code:`CONNECTION`    :code:`VariablePair`
    :code:`ENCAPSULATION` :code:`Model`
    :code:`IMPORT`        :code:`ImportSource`
    :code:`MAP_VARIABLES` :code:`VariablePair`
    :code:`MODEL`         :code:`Model`
    :code:`RESET`         :code:`Reset`
    :code:`RESET_VALUE`   :code:`Reset`
    :code:`TEST_VALUE`    :code:`Reset`
    :code:`UNIT`          :code:`Unit`
    :code:`UNITS`         :code:`Units`
    :code:`VARIABLE`      :code:`Variable`
    ===================== ====================


Importer codes
==============

For complete descriptions of all aspects of the :code:`Importer`, please see the tutorials and examples available at the :ref:`Import debugger<users_import_debugger>` page.

.. _IMPORTER_NULL_MODEL:

.. container:: issue-code

    IMPORTER_NULL_MODEL

When the :code:`Importer` attempts to resolve the imports in a model for flattening, but encounters an :code:`ImportSource` whose associated :code:`Model` is missing, this code is returned.
This situation can be caused by changing any of the import sources or their references after a model has been added to the importer.

.. container:: shortlist

    **API** :api:`Importer::flattenModel<Importer?fName=flattenModel>`

    **How-to** :ref:`Resolve imports<actions_resolve_imports>`

    **Common users** :ref:`Import debugger<users_import_debugger>`

.. _IMPORTER_MISSING_FILE:

.. container:: issue-code

    IMPORTER_MISSING_FILE

The :code:`Importer` will return this code if it cannot locate or open an imported file.
You can check the specified locations using the :api:`ImportSource::url() function <ImportSource?fName=url>` for the imported items in your model.

.. container:: shortlist

    **API** :api:`ImportSource::url()<ImportSource?fName=url>`, :api:`ImportSource::setUrl()<ImportSource?fName=setUrl>`

    **Common users** :ref:`Import debugger<users_import_debugger>`

    **Tutorial** :ref:`Hodgkin-Huxley Tutorial 2<createPotassiumChannelModel>`

.. container:: nb

    Be aware that since imports can have many generations, the error may not be in your first level model.
    The description returned with this error will identify the model of interest.

.. _IMPORTER_MISSING_COMPONENT:

.. container:: issue-code

    IMPORTER_MISSING_COMPONENT

The :code:`Importer` will return this code if an import reference for an imported component is not found in the specified model.
The issue's description will note the URL of the model in question.  

.. container:: shortlist 

    To debug:
    
    - check the component names available in the imported model using :api:`Model::units(index)<Model?fName=units>` on the model at the URL;
    - use the :api:`ImportSource::importReference()<ImportSource?fName=importReference>` function to check the currently specified reference;
    - use the :api:`ImportSource::setImportReference()<ImportSource?fName=setImportReference>` function to amend the reference. 

.. container:: shortlist

    **API** :api:`ImportSource::importReference()<ImportSource?fName=importReference>`, :api:`ImportSource::setImportReference()<ImportSource?fName=setImportReference>`

    **Common users** :ref:`Import debugger<users_import_debugger>`

.. _IMPORTER_MISSING_UNITS:

.. container:: issue-code

    IMPORTER_MISSING_UNITS

The :code:`Importer` will return this code if an import reference for an imported units item is not found in the specified model.
The issue's description will note the URL of the model in question.

.. container:: shortlist 

    To debug:

    - check the units names available in the imported model using :api:`Model::units(index)<Model?fName=units>` on the model at the URL;
    - use the :api:`ImportSource::importReference()<ImportSource?fName=importReference>` function to check the currently specified reference;
    - use the :api:`ImportSource::setImportReference()<ImportSource?fName=setImportReference>` function to amend the reference. 

.. container:: shortlist

    **API** :api:`ImportSource::importReference()<ImportSource?fName=importReference>`, :api:`ImportSource::setImportReference()<ImportSource?fName=setImportReference>`

    **Common users** :ref:`Import debugger<users_import_debugger>`

Parser codes
============
All codes returned by the :code:`Parser` are related to syntax, and covered in the :ref:`section below<syntax_validation>`.

Printer codes
=============
At present, the :code:`Printer` class does not return any issue codes.

Miscellaneous codes
===================

.. _INVALID_ARGUMENT:

.. container:: issue-code

    INVALID_ARGUMENT

This is a general code used by the :code:`Importer` and :code:`Validator` classes when the argument given is null.

.. container:: shortlist

    **API** :api:`Validator::validateModel<Validator?fName=validateModel>`, :api:`Importer::flattenModel<Importer?fName=flattenModel>`.

.. _syntax_validation:

Codes related to syntactic validation
=====================================
The codes below are returned from the :code:`Validator` as well as the :code:`Parser` classes. 
The documentation is located on the :cellml2:`CellML 2.0 specification pages<>`, with examples and extra explanations under the "See more" blocks there.

.. container:: issue-code

    XML
    
See :cellml2:`Section 1.2.1<reference/formal_and_informative/specA02.html?issue=XML>`

.. container:: issue-code

    DATA_REPR_IDENTIFIER_UNICODE
    
See :cellml2:`Section 1.3.1.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_UNICODE>`

.. container:: issue-code

    DATA_REPR_IDENTIFIER_LATIN_ALPHANUM
    
See :cellml2:`Section 1.3.1.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_LATIN_ALPHANUM>`

.. container:: issue-code

    DATA_REPR_IDENTIFIER_AT_LEAST_ONE_ALPHANUM
    
See :cellml2:`Section 1.3.1.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_AT_LEAST_ONE_ALPHANUM>`

.. container:: issue-code

    DATA_REPR_IDENTIFIER_BEGIN_EURO_NUM
    
See :cellml2:`Section 1.3.1.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_BEGIN_EURO_NUM>`

.. container:: issue-code

    DATA_REPR_IDENTIFIER_IDENTICAL
    
See :cellml2:`Section 1.3.1.2<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_IDENTICAL>`

.. container:: issue-code

    DATA_REPR_INT_BASE10
    
See :cellml2:`Section 1.3.2.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_INT_BASE10>`

.. container:: issue-code

    DATA_REPR_INT_SIGN
    
See :cellml2:`Section 1.3.2.2<reference/formal_and_informative/specA03.html?issue=DATA_REPR_INT_SIGN>`

.. container:: issue-code

    DATA_REPR_INT_DIGIT
    
See :cellml2:`Section 1.3.2.3<reference/formal_and_informative/specA03.html?issue=DATA_REPR_INT_DIGIT>`

.. container:: issue-code

    DATA_REPR_BASIC_REAL_BASE10
    
See :cellml2:`Section 1.3.3.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_BASIC_REAL_BASE10>`

.. container:: issue-code

    DATA_REPR_BASIC_REAL_SIGN
    
See :cellml2:`Section 1.3.3.2<reference/formal_and_informative/specA03.html?issue=DATA_REPR_BASIC_REAL_SIGN>`

.. container:: issue-code

    DATA_REPR_BASIC_REAL_DECIMAL
    
See :cellml2:`Section 1.3.3.3<reference/formal_and_informative/specA03.html?issue=DATA_REPR_BASIC_REAL_DECIMAL>`

.. container:: issue-code

    DATA_REPR_BASIC_REAL_DIGIT
    
See :cellml2:`Section 1.3.3.4<reference/formal_and_informative/specA03.html?issue=DATA_REPR_BASIC_REAL_DIGIT>`

.. container:: issue-code

    DATA_REPR_REAL_BASE10
    
See :cellml2:`Section 1.3.4.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_REAL_BASE10>`

.. container:: issue-code

    DATA_REPR_REAL_SIGNIFICAND
    
See :cellml2:`Section 1.3.4.3<reference/formal_and_informative/specA03.html?issue=DATA_REPR_REAL_SIGNIFICAND>`

.. container:: issue-code

    DATA_REPR_REAL_EXPONENT
    
See :cellml2:`Section 1.3.4.4<reference/formal_and_informative/specA03.html?issue=DATA_REPR_REAL_EXPONENT>`

.. container:: issue-code

    MODEL_ELEMENT
    
See :cellml2:`Section 2.1<reference/formal_and_informative/specB01.html?issue=MODEL_ELEMENT>`

.. container:: issue-code

    MODEL_NAME
    
See :cellml2:`Section 2.1.1<reference/formal_and_informative/specB01.html?issue=MODEL_NAME>`

.. container:: issue-code

    MODEL_CHILD
    
See :cellml2:`Section 2.1.2<reference/formal_and_informative/specB01.html?issue=MODEL_CHILD>`

.. container:: issue-code

    MODEL_MORE_THAN_ONE_ENCAPSULATION
    
See :cellml2:`Section 2.1.3<reference/formal_and_informative/specB01.html?issue=MODEL_MORE_THAN_ONE_ENCAPSULATION>`

.. container:: issue-code

    IMPORT_ATTRIBUTE
    
See :cellml2:`Section 2.2<reference/formal_and_informative/specB02.html?issue=IMPORT_ATTRIBUTE>`

.. container:: issue-code

    IMPORT_HREF
    
See :cellml2:`Section 2.2.1<reference/formal_and_informative/specB02.html?issue=IMPORT_HREF>`

.. container:: issue-code

    IMPORT_CHILD
    
See :cellml2:`Section 2.2.2<reference/formal_and_informative/specB02.html?issue=IMPORT_CHILD>`

.. container:: issue-code

    IMPORT_EQUIVALENT
    
See :cellml2:`Section 2.2.3<reference/formal_and_informative/specB02.html?issue=IMPORT_EQUIVALENT>`

.. container:: issue-code

    IMPORT_UNITS_NAME
    
See :cellml2:`Section 2.3.1<reference/formal_and_informative/specB03.html?issue=IMPORT_UNITS_NAME>`

.. container:: issue-code

    IMPORT_UNITS_NAME_UNIQUE
    
See :cellml2:`Section 2.3.1<reference/formal_and_informative/specB03.html?issue=IMPORT_UNITS_NAME_UNIQUE>`

.. container:: issue-code

    IMPORT_UNITS_REF
    
See :cellml2:`Section 2.3.2<reference/formal_and_informative/specB03.html?issue=IMPORT_UNITS_REF>`

.. container:: issue-code

    IMPORT_COMPONENT_NAME

    See :cellml2:`Section 2.4.1<reference/formal_and_informative/specB04.html?issue=IMPORT_COMPONENT_NAME>`

.. container:: issue-code

    IMPORT_COMPONENT_NAME_UNIQUE

See :cellml2:`Section 2.4.1<reference/formal_and_informative/specB04.html?issue=IMPORT_COMPONENT_NAME_UNIQUE>`

.. container:: issue-code

    IMPORT_COMPONENT_COMPONENT_REF

See :cellml2:`Section 2.4.2<reference/formal_and_informative/specB04.html?issue=IMPORT_COMPONENT_COMPONENT_REF>`

.. container:: issue-code

    UNITS_ATTRIBUTE

See :cellml2:`Section 2.5<reference/formal_and_informative/specB05.html?issue=UNITS_ATTRIBUTE>`

.. container:: issue-code

    UNITS_NAME

See :cellml2:`Section 2.5.1<reference/formal_and_informative/specB05.html?issue=UNITS_NAME>`

.. container:: issue-code

    UNITS_NAME_UNIQUE

See :cellml2:`Section 2.5.1<reference/formal_and_informative/specB05.html?issue=UNITS_NAME_UNIQUE>`

.. container:: issue-code

    UNITS_STANDARD

See :cellml2:`Section 2.5.2<reference/formal_and_informative/specB05.html?issue=UNITS_STANDARD>`

.. container:: issue-code

    UNITS_CHILD

See :cellml2:`Section 2.5.3<reference/formal_and_informative/specB05.html?issue=UNITS_CHILD>`

.. container:: issue-code

    UNIT_ATTRIBUTE

See :cellml2:`Section 2.6<reference/formal_and_informative/specB06.html?issue=UNIT_ATTRIBUTE>`

.. container:: issue-code

    UNIT_UNITS_REF

See :cellml2:`Section 2.6.1<reference/formal_and_informative/specB06.html?issue=UNIT_UNITS_REF>`

.. container:: issue-code

    UNIT_CIRCULAR_REF

See :cellml2:`Section 2.6.1.2<reference/formal_and_informative/specB06.html?issue=UNIT_CIRCULAR_REF>`

.. container:: issue-code

    UNIT_OPTIONAL_ATTRIBUTE

See :cellml2:`Section 2.6.2<reference/formal_and_informative/specB06.html?issue=UNIT_OPTIONAL_ATTRIBUTE>`

.. container:: issue-code

    UNIT_PREFIX

See :cellml2:`Section 2.6.2.1<reference/formal_and_informative/specB06.html?issue=UNIT_PREFIX>`

.. container:: issue-code

    UNIT_MULTIPLIER

See :cellml2:`Section 2.6.2.2<reference/formal_and_informative/specB06.html?issue=UNIT_MULTIPLIER>`

.. container:: issue-code

    UNIT_EXPONENT

See :cellml2:`Section 2.6.2.3<reference/formal_and_informative/specB06.html?issue=UNIT_EXPONENT>`

.. container:: issue-code

    COMPONENT_ATTRIBUTE

See :cellml2:`Section 2.7<reference/formal_and_informative/specB07.html?issue=COMPONENT_ATTRIBUTE>`

.. container:: issue-code

    COMPONENT_NAME

See :cellml2:`Section 2.7.1<reference/formal_and_informative/specB07.html?issue=COMPONENT_NAME>`

.. container:: issue-code

    COMPONENT_NAME_UNIQUE

See :cellml2:`Section 2.7.1<reference/formal_and_informative/specB07.html?issue=COMPONENT_NAME_UNIQUE>`

.. container:: issue-code

    COMPONENT_CHILD

See :cellml2:`Section 2.7.2<reference/formal_and_informative/specB07.html?issue=COMPONENT_CHILD>`

.. container:: issue-code

    VARIABLE_ATTRIBUTE

See :cellml2:`Section 2.8<reference/formal_and_informative/specB08.html?issue=VARIABLE_ATTRIBUTE>`

.. container:: issue-code

    VARIABLE_CHILD

See :cellml2:`Section 2.8<reference/formal_and_informative/specB08.html?issue=VARIABLE_CHILD>`

.. container:: issue-code

    VARIABLE_NAME

See :cellml2:`Section 2.8.1.1<reference/formal_and_informative/specB08.html?issue=VARIABLE_NAME>`

.. container:: issue-code

    VARIABLE_NAME_UNIQUE

See :cellml2:`Section 2.8.1.1<reference/formal_and_informative/specB08.html?issue=VARIABLE_NAME_UNIQUE>`

.. container:: issue-code

    VARIABLE_UNITS

See :cellml2:`Section 2.8.1.2<reference/formal_and_informative/specB08.html?issue=VARIABLE_UNITS>`

.. container:: issue-code

    VARIABLE_INTERFACE

See :cellml2:`Section 2.8.2.1<reference/formal_and_informative/specB08.html?issue=VARIABLE_INTERFACE>`

.. container:: issue-code

    VARIABLE_INITIAL_VALUE

See :cellml2:`Section 2.8.2.2<reference/formal_and_informative/specB08.html?issue=VARIABLE_INITIAL_VALUE>`

.. container:: issue-code

    RESET_ATTRIBUTE

See :cellml2:`Section 2.9<reference/formal_and_informative/specB09.html?issue=RESET_ATTRIBUTE>`

.. container:: issue-code

    RESET_VARIABLE_REF

See :cellml2:`Section 2.9.1.1<reference/formal_and_informative/specB09.html?issue=RESET_VARIABLE_REF>`

.. container:: issue-code

    RESET_TEST_VARIABLE_REF

See :cellml2:`Section 2.9.1.2<reference/formal_and_informative/specB09.html?issue=RESET_TEST_VARIABLE_REF>`

.. container:: issue-code

    RESET_ORDER

See :cellml2:`Section 2.9.1.3<reference/formal_and_informative/specB09.html?issue=RESET_ORDER>`

.. container:: issue-code

    RESET_CHILD

See :cellml2:`Section 2.9.2<reference/formal_and_informative/specB09.html?issue=RESET_CHILD>`

.. container:: issue-code

    RESET_TEST_VALUE

See :cellml2:`Section 2.1<reference/formal_and_informative/specB10.html?issue=RESET_TEST_VALUE>`

.. container:: issue-code

    RESET_RESET_VALUE

See :cellml2:`Section 2.11<reference/formal_and_informative/specB11.html?issue=RESET_RESET_VALUE>`

.. container:: issue-code

    MATH_MATHML

See :cellml2:`Section 2.12.1<reference/formal_and_informative/specB12.html?issue=MATH_MATHML>`

.. container:: issue-code

    MATH_CHILD

See :cellml2:`Section 2.12.2<reference/formal_and_informative/specB12.html?issue=MATH_CHILD>`

.. container:: issue-code

    MATH_CI_VARIABLE_REF

See :cellml2:`Section 2.12.3<reference/formal_and_informative/specB12.html?issue=MATH_CI_VARIABLE_REF>`

.. container:: issue-code

    MATH_CN_UNITS

See :cellml2:`Section 2.13.4<reference/formal_and_informative/specB13.html?issue=MATH_CN_UNITS>`

.. container:: issue-code

    MATH_CN_BASE10

See :cellml2:`Section 2.13.5<reference/formal_and_informative/specB13.html?issue=MATH_CN_BASE10>`

.. container:: issue-code

    MATH_CN_FORMAT

See :cellml2:`Section 2.13.5<reference/formal_and_informative/specB13.html?issue=MATH_CN_FORMAT>`

.. container:: issue-code

    ENCAPSULATION_ATTRIBUTE

See :cellml2:`Section 2.13<reference/formal_and_informative/specB13.html?issue=ENCAPSULATION_ATTRIBUTE>`

.. container:: issue-code

    ENCAPSULATION_CHILD

See :cellml2:`Section 2.13.1<reference/formal_and_informative/specB13.html?issue=ENCAPSULATION_CHILD>`

.. container:: issue-code

    COMPONENT_REF_COMPONENT

See :cellml2:`Section 2.14.1<reference/formal_and_informative/specB14.html?issue=COMPONENT_REF_COMPONENT>`

.. container:: issue-code

    COMPONENT_REF_CHILD

See :cellml2:`Section 2.14.2<reference/formal_and_informative/specB14.html?issue=COMPONENT_REF_CHILD>`

.. container:: issue-code

    CONNECTION_ATTRIBUTE

See :cellml2:`Section 2.15<reference/formal_and_informative/specB14.html?issue=CONNECTION_ATTRIBUTE>`

.. container:: issue-code

    CONNECTION_COMPONENT1

See :cellml2:`Section 2.15.1<reference/formal_and_informative/specB15.html?issue=CONNECTION_COMPONENT1>`

.. container:: issue-code

    CONNECTION_COMPONENT2

See :cellml2:`Section 2.15.2<reference/formal_and_informative/specB15.html?issue=CONNECTION_COMPONENT2>`

.. container:: issue-code

    CONNECTION_EXCLUDE_SELF

See :cellml2:`Section 2.15.3<reference/formal_and_informative/specB15.html?issue=CONNECTION_EXCLUDE_SELF>`

.. container:: issue-code

    CONNECTION_UNIQUE

See :cellml2:`Section 2.15.4<reference/formal_and_informative/specB15.html?issue=CONNECTION_UNIQUE>`

.. container:: issue-code

    CONNECTION_CHILD

See :cellml2:`Section 2.15.5<reference/formal_and_informative/specB15.html?issue=CONNECTION_CHILD>`

.. container:: issue-code

    MAP_VARIABLES_ATTRIBUTE

See :cellml2:`Section 2.16<reference/formal_and_informative/specB16.html?issue=MAP_VARIABLES_ATTRIBUTE>`

.. container:: issue-code

    MAP_VARIABLES_VARIABLE1

See :cellml2:`Section 2.16.1<reference/formal_and_informative/specB16.html?issue=MAP_VARIABLES_VARIABLE1>`

.. container:: issue-code

    MAP_VARIABLES_VARIABLE2

See :cellml2:`Section 2.16.2<reference/formal_and_informative/specB16.html?issue=MAP_VARIABLES_VARIABLE2>`

.. container:: issue-code

    MAP_VARIABLES_UNIQUE

See :cellml2:`Section 2.16.3<reference/formal_and_informative/specB16.html?issue=MAP_VARIABLES_UNIQUE>`

.. container:: issue-code

    MAP_VARIABLES_AVAILABLE_INTERFACE

See :cellml2:`Section 3.10.8<reference/formal_and_informative/specC10.html?issue=MAP_VARIABLES_AVAILABLE_INTERFACE>`

.. container:: issue-code

    MAP_VARIABLES_IDENTICAL_UNIT_REDUCTION

See :cellml2:`Section 3.10.9<reference/formal_and_informative/specC10.html?issue=MAP_VARIABLES_IDENTICAL_UNIT_REDUCTION>`