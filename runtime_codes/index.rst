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

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_VARIABLE_INITIALISED_MORE_THAN_ONCE

    Lorem ipsum dolor sit amet, consectetur adipiscing elit. 
    Proin mauris sapien, scelerisque eu ornare et, luctus feugiat odio. 
    Mauris vestibulum augue vel ligula tincidunt euismod. Duis eros mauris, bibendum et nulla in, pulvinar malesuada lacus. 
    Mauris nulla justo, faucibus vulputate elementum a, auctor ac mi.
    Nunc vestibulum, dui porta lobortis tempor, odio neque semper tellus, eu efficitur erat magna non nisi.
    Integer finibus feugiat ultrices. Vivamus ut nunc elementum, tristique nisi sed, euismod sapien.


.. _ANALYSER_VARIABLE_NON_CONSTANT_INITIALISATION:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_VARIABLE_NON_CONSTANT_INITIALISATION


.. _ANALYSER_VOI_INITIALISED:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_VOI_INITIALISED


.. _ANALYSER_VOI_SEVERAL:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_VOI_SEVERAL


.. _ANALYSER_ODE_NOT_FIRST_ORDER:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_ODE_NOT_FIRST_ORDER


.. _ANALYSER_VARIABLE_UNUSED:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_VARIABLE_UNUSED


.. _ANALYSER_STATE_NOT_INITIALISED:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_STATE_NOT_INITIALISED


.. _ANALYSER_VARIABLE_COMPUTED_MORE_THAN_ONCE:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_VARIABLE_COMPUTED_MORE_THAN_ONCE


.. _ANALYSER_EXTERNAL_VARIABLE_DIFFERENT_MODEL:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_EXTERNAL_VARIABLE_DIFFERENT_MODEL


.. _ANALYSER_EXTERNAL_VARIABLE_VOI:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_EXTERNAL_VARIABLE_VOI


.. _ANALYSER_EXTERNAL_VARIABLE_USE_PRIMARY_VARIABLE:

.. container:: issue

    .. container:: issue-code
    
        ANALYSER_EXTERNAL_VARIABLE_USE_PRIMARY_VARIABLE


Annotator codes
===============

.. _ANNOTATOR_NO_MODEL:

.. container:: issue

    .. container:: issue-code
    
        ANNOTATOR_NO_MODEL
    
    The :code:`Annotator` class needs a :code:`Model` item on which to operate.
    If you're seeing an issue with this code, it's because you need to use the :code:`setModel` function to add it.

    **API:** :api:`Annotator::setModel function<Annotator?fName=setModel>`
    **How-to:** :ref:`Howto: Annotate a model<annotate_model>`
    **Common users:** :ref:`Annotation tool developer<users_annotation_tool_dev>`
    

.. _ANNOTATOR_NULL_MODEL:

.. container:: issue

    .. container:: issue-code
    
        ANNOTATOR_NULL_MODEL
    
    This code is generated when the :code:`Annotator` is given a null pointer instead of a :code:`Model` item.

    **API:** :api:`Annotator::setModel function<Annotator?fName=setModel>`
    **How-to:** :ref:`Howto: Annotate a model<annotate_model>`
    **Common users:** :ref:`Annotation tool developer<users_annotation_tool_dev>`

.. _ANNOTATOR_ID_NOT_FOUND:

.. container:: issue

    .. container:: issue-code
    
        ANNOTATOR_ID_NOT_FOUND
    
    When the :code:`Model` that was submitted to this :code:`Annotator` does not have any item with the requested id string, a warning with this code is created.

    .. container:: nb
    
        Please note that currently the :code:`Annotator` class does not check for ids within MathML strings.
    
    **API:** :api:`Annotator::item function<Annotator?fName=item>` (or any of the type-specific retrieval functions)
    **How-to:** :ref:`Howto: Annotate a model<annotate_model>`
    **Common users:** :ref:`Annotation tool developer<users_annotation_tool_dev>`
    

.. _ANNOTATOR_ID_NOT_UNIQUE:

.. container:: issue

    .. container:: issue-code
    
        ANNOTATOR_ID_NOT_UNIQUE 

    While the :code:`Annotator` is able to retrieve and manipulate collections of items with the same id, any XML document with duplicated ids is not valid.  
    When non-unique id strings are encountered, a warning with this code is created.

    For more information please see the :ref:`Annotator: Handle duplicated ID attributes<handle_duplicated_id_attributes>` section.


.. _ANNOTATOR_INCONSISTENT_TYPE:

.. container:: issue

    .. container:: issue-code
    
        ANNOTATOR_INCONSISTENT_TYPE 
    
    In contrast to all other classes in libCellML, the :code:`Annotator` must be agnostic as to the type of item it deals with.
    It uses :code:`AnyItem` type in order to be able to return items without specifying their type ahead of time.
    An error is issued when the information stored in an :code:`AnyItem` item (the :code:`CellmlElementType` of the item, and the item itself) are not consistent.
    This issue is raised when attempting to use the automatic id feature of the :code:`Annotator`, and can be rectified by checking that any manually created :code:`AnyItem` items have the correct pairing of :code:`CellmlElementType` and object.
    The expected pairings are:

    .. table:: Expected pairings in :code:`AnyItem`

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

.. container:: issue

    .. container:: issue-code
    
        IMPORTER_NULL_MODEL
    
    When the :code:`Importer` attempts to resolve the imports in a model, but encounters an :code:`ImportSource` whose associated :code:`Model` is missing, then an issue is created with this code.  

    For more examples, please see the :ref:`Import debugger<users_import_debugger>` page.

.. _IMPORTER_MISSING_FILE:

    .. container:: issue

        .. container:: issue-code
        
            IMPORTER_MISSING_FILE
    
    The :code:`Importer` will return this code if it cannot locate or open an imported file.
    You can check the specified locations using the :api:`ImportSource::url() function <ImportSource?fName=url>` for the imported items in your model.

    .. container:: nb

        Be aware that since imports can have many generations, the error may not be in your first level model.
        The description returned with this error will identify the model of interest.
    
    For more examples, please see the :ref:`Import debugger<users_import_debugger>` page.

.. _IMPORTER_MISSING_COMPONENT:

.. container:: issue

    .. container:: issue-code
    
        IMPORTER_MISSING_COMPONENT

    The :code:`Importer` will return this code if an import reference for an imported component is not found in the specified model.
    The issue's description will note the URL of the model in question.  

    .. container:: shortlist 

        To debug:
        - check the component names available in the imported model using :api:`Model::units(index)<Model?fName=units>` on the model at the URL;
        - use the :api:`ImportSource::importReference()<ImportSource?fName=importReference>` function to check the currently specified reference;
        - use the :api:`ImportSource::setImportReference()<ImportSource?fName=setImportReference>` function to amend the reference. 

    For more examples, please see the :ref:`Import debugger<users_import_debugger>` page.

.. _IMPORTER_MISSING_UNITS:

.. container:: issue

    .. container:: issue-code
    
        IMPORTER_MISSING_UNITS

    The :code:`Importer` will return this code if an import reference for an imported units item is not found in the specified model.
    The issue's description will note the URL of the model in question.

    .. container:: shortlist 

        To debug:
        - check the units names available in the imported model using :api:`Model::units(index)<Model?fName=units>` on the model at the URL;
        - use the :api:`ImportSource::importReference()<ImportSource?fName=importReference>` function to check the currently specified reference;
        - use the :api:`ImportSource::setImportReference()<ImportSource?fName=setImportReference>` function to amend the reference. 

    For more examples, please see the :ref:`Import debugger<users_import_debugger>` page.


Parser codes
============

.. _XML:

.. container:: issue

    .. container:: issue-code
    
        XML
    
    This is a catch-all code for anything encountered when parsing an XML string that means it can't be understood.
    The description will provide more specific information on what is wrong.

    **Read more:** There is a lot more information about well-formed or otherwise XML in the :cellml2:`CellML 2.0 specification<reference/formal_and_informative/specA02.html>` under then "See more" informative blocks.  
    

Printer codes
=============

At present, the :code:`Printer` class does not return any issue codes.


Miscellaneous codes
===================

.. _INVALID_ARGUMENT:

.. container:: issue

    .. container:: issue-code
    
        INVALID_ARGUMENT

    This is a general code used by the :code:`Importer` and :code:`Validator` classes when the argument given is null.

    **API:** :api:`Validator::validateModel<Validator?fName=validateModel>` and :api:`Importer::flattenModel<Importer?fName=flattenModel>`.

Codes related to syntactic validation
=====================================
The codes below are returned from the :code:`Validator` as well as the :code:`Parser` classes. 
The documentation is located on the :cellml2:`CellML 2.0 specification pages<>`, with examples and extra explanations under the "See more" blocks there.

.. container:: shortlist

    - :code:`XML` see :cellml2:`Section 1.2.1<reference/formal_and_informative/specA02.html?issue=XML>`
    - :code:`DATA_REPR_IDENTIFIER_UNICODE` see :cellml2:`Section 1.3.1.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_UNICODE>`
    - :code:`DATA_REPR_IDENTIFIER_LATIN_ALPHANUM` see :cellml2:`Section 1.3.1.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_LATIN_ALPHANUM>`
    - :code:`DATA_REPR_IDENTIFIER_AT_LEAST_ONE_ALPHANUM` see :cellml2:`Section 1.3.1.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_AT_LEAST_ONE_ALPHANUM>`
    - :code:`DATA_REPR_IDENTIFIER_BEGIN_EURO_NUM` see :cellml2:`Section 1.3.1.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_BEGIN_EURO_NUM>`
    - :code:`DATA_REPR_IDENTIFIER_IDENTICAL` see :cellml2:`Section 1.3.1.2<reference/formal_and_informative/specA03.html?issue=DATA_REPR_IDENTIFIER_IDENTICAL>`
    - :code:`DATA_REPR_INT_BASE10` see :cellml2:`Section 1.3.2.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_INT_BASE10>`
    - :code:`DATA_REPR_INT_SIGN` see :cellml2:`Section 1.3.2.2<reference/formal_and_informative/specA03.html?issue=DATA_REPR_INT_SIGN>`
    - :code:`DATA_REPR_INT_DIGIT` see :cellml2:`Section 1.3.2.3<reference/formal_and_informative/specA03.html?issue=DATA_REPR_INT_DIGIT>`
    - :code:`DATA_REPR_BASIC_REAL_BASE10` see :cellml2:`Section 1.3.3.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_BASIC_REAL_BASE10>`
    - :code:`DATA_REPR_BASIC_REAL_SIGN` see :cellml2:`Section 1.3.3.2<reference/formal_and_informative/specA03.html?issue=DATA_REPR_BASIC_REAL_SIGN>`
    - :code:`DATA_REPR_BASIC_REAL_DECIMAL` see :cellml2:`Section 1.3.3.3<reference/formal_and_informative/specA03.html?issue=DATA_REPR_BASIC_REAL_DECIMAL>`
    - :code:`DATA_REPR_BASIC_REAL_DIGIT` see :cellml2:`Section 1.3.3.4<reference/formal_and_informative/specA03.html?issue=DATA_REPR_BASIC_REAL_DIGIT>`
    - :code:`DATA_REPR_REAL_BASE10` see :cellml2:`Section 1.3.4.1<reference/formal_and_informative/specA03.html?issue=DATA_REPR_REAL_BASE10>`
    - :code:`DATA_REPR_REAL_SIGNIFICAND` see :cellml2:`Section 1.3.4.3<reference/formal_and_informative/specA03.html?issue=DATA_REPR_REAL_SIGNIFICAND>`
    - :code:`DATA_REPR_REAL_EXPONENT` see :cellml2:`Section 1.3.4.4<reference/formal_and_informative/specA03.html?issue=DATA_REPR_REAL_EXPONENT>`
    - :code:`MODEL_ELEMENT` see :cellml2:`Section 2.1<reference/formal_and_informative/specB01.html?issue=MODEL_ELEMENT>`
    - :code:`MODEL_NAME` see :cellml2:`Section 2.1.1<reference/formal_and_informative/specB01.html?issue=MODEL_NAME>`
    - :code:`MODEL_CHILD` see :cellml2:`Section 2.1.2<reference/formal_and_informative/specB01.html?issue=MODEL_CHILD>`
    - :code:`MODEL_MORE_THAN_ONE_ENCAPSULATION` see :cellml2:`Section 2.1.3<reference/formal_and_informative/specB01.html?issue=MODEL_MORE_THAN_ONE_ENCAPSULATION>`
    - :code:`IMPORT_ATTRIBUTE` see :cellml2:`Section 2.2<reference/formal_and_informative/specB02.html?issue=IMPORT_ATTRIBUTE>`
    - :code:`IMPORT_HREF` see :cellml2:`Section 2.2.1<reference/formal_and_informative/specB02.html?issue=IMPORT_HREF>`
    - :code:`IMPORT_CHILD` see :cellml2:`Section 2.2.2<reference/formal_and_informative/specB02.html?issue=IMPORT_CHILD>`
    - :code:`IMPORT_EQUIVALENT` see :cellml2:`Section 2.2.3<reference/formal_and_informative/specB02.html?issue=IMPORT_EQUIVALENT>`
    - :code:`IMPORT_UNITS_NAME` see :cellml2:`Section 2.3.1<reference/formal_and_informative/specB03.html?issue=IMPORT_UNITS_NAME>`
    - :code:`IMPORT_UNITS_NAME_UNIQUE` see :cellml2:`Section 2.3.1<reference/formal_and_informative/specB03.html?issue=IMPORT_UNITS_NAME_UNIQUE>`
    - :code:`IMPORT_UNITS_REF` see :cellml2:`Section 2.3.2<reference/formal_and_informative/specB03.html?issue=IMPORT_UNITS_REF>`
    - :code:`IMPORT_COMPONENT_NAME` see :cellml2:`Section 2.4.1<reference/formal_and_informative/specB04.html?issue=IMPORT_COMPONENT_NAME>`
    - :code:`IMPORT_COMPONENT_NAME_UNIQUE` see :cellml2:`Section 2.4.1<reference/formal_and_informative/specB04.html?issue=IMPORT_COMPONENT_NAME_UNIQUE>`
    - :code:`IMPORT_COMPONENT_COMPONENT_REF` see :cellml2:`Section 2.4.2<reference/formal_and_informative/specB04.html?issue=IMPORT_COMPONENT_COMPONENT_REF>`
    - :code:`UNITS_ATTRIBUTE` see :cellml2:`Section 2.5<reference/formal_and_informative/specB05.html?issue=UNITS_ATTRIBUTE>`
    - :code:`UNITS_NAME` see :cellml2:`Section 2.5.1<reference/formal_and_informative/specB05.html?issue=UNITS_NAME>`
    - :code:`UNITS_NAME_UNIQUE` see :cellml2:`Section 2.5.1<reference/formal_and_informative/specB05.html?issue=UNITS_NAME_UNIQUE>`
    - :code:`UNITS_STANDARD` see :cellml2:`Section 2.5.2<reference/formal_and_informative/specB05.html?issue=UNITS_STANDARD>`
    - :code:`UNITS_CHILD` see :cellml2:`Section 2.5.3<reference/formal_and_informative/specB05.html?issue=UNITS_CHILD>`
    - :code:`UNIT_ATTRIBUTE` see :cellml2:`Section 2.6<reference/formal_and_informative/specB06.html?issue=UNIT_ATTRIBUTE>`
    - :code:`UNIT_UNITS_REF` see :cellml2:`Section 2.6.1<reference/formal_and_informative/specB06.html?issue=UNIT_UNITS_REF>`
    - :code:`UNIT_CIRCULAR_REF` see :cellml2:`Section 2.6.1.2<reference/formal_and_informative/specB06.html?issue=UNIT_CIRCULAR_REF>`
    - :code:`UNIT_OPTIONAL_ATTRIBUTE` see :cellml2:`Section 2.6.2<reference/formal_and_informative/specB06.html?issue=UNIT_OPTIONAL_ATTRIBUTE>`
    - :code:`UNIT_PREFIX` see :cellml2:`Section 2.6.2.1<reference/formal_and_informative/specB06.html?issue=UNIT_PREFIX>`
    - :code:`UNIT_MULTIPLIER` see :cellml2:`Section 2.6.2.2<reference/formal_and_informative/specB06.html?issue=UNIT_MULTIPLIER>`
    - :code:`UNIT_EXPONENT` see :cellml2:`Section 2.6.2.3<reference/formal_and_informative/specB06.html?issue=UNIT_EXPONENT>`
    - :code:`COMPONENT_ATTRIBUTE` see :cellml2:`Section 2.7<reference/formal_and_informative/specB07.html?issue=COMPONENT_ATTRIBUTE>`
    - :code:`COMPONENT_NAME` see :cellml2:`Section 2.7.1<reference/formal_and_informative/specB07.html?issue=COMPONENT_NAME>`
    - :code:`COMPONENT_NAME_UNIQUE` see :cellml2:`Section 2.7.1<reference/formal_and_informative/specB07.html?issue=COMPONENT_NAME_UNIQUE>`
    - :code:`COMPONENT_CHILD` see :cellml2:`Section 2.7.2<reference/formal_and_informative/specB07.html?issue=COMPONENT_CHILD>`
    - :code:`VARIABLE_ATTRIBUTE` see :cellml2:`Section 2.8<reference/formal_and_informative/specB08.html?issue=VARIABLE_ATTRIBUTE>`
    - :code:`VARIABLE_CHILD` see :cellml2:`Section 2.8<reference/formal_and_informative/specB08.html?issue=VARIABLE_CHILD>`
    - :code:`VARIABLE_NAME` see :cellml2:`Section 2.8.1.1<reference/formal_and_informative/specB08.html?issue=VARIABLE_NAME>`
    - :code:`VARIABLE_NAME_UNIQUE` see :cellml2:`Section 2.8.1.1<reference/formal_and_informative/specB08.html?issue=VARIABLE_NAME_UNIQUE>`
    - :code:`VARIABLE_UNITS` see :cellml2:`Section 2.8.1.2<reference/formal_and_informative/specB08.html?issue=VARIABLE_UNITS>`
    - :code:`VARIABLE_INTERFACE` see :cellml2:`Section 2.8.2.1<reference/formal_and_informative/specB08.html?issue=VARIABLE_INTERFACE>`
    - :code:`VARIABLE_INITIAL_VALUE` see :cellml2:`Section 2.8.2.2<reference/formal_and_informative/specB08.html?issue=VARIABLE_INITIAL_VALUE>`
    - :code:`RESET_ATTRIBUTE` see :cellml2:`Section 2.9<reference/formal_and_informative/specB09.html?issue=RESET_ATTRIBUTE>`
    - :code:`RESET_VARIABLE_REF` see :cellml2:`Section 2.9.1.1<reference/formal_and_informative/specB09.html?issue=RESET_VARIABLE_REF>`
    - :code:`RESET_TEST_VARIABLE_REF` see :cellml2:`Section 2.9.1.2<reference/formal_and_informative/specB09.html?issue=RESET_TEST_VARIABLE_REF>`
    - :code:`RESET_ORDER` see :cellml2:`Section 2.9.1.3<reference/formal_and_informative/specB09.html?issue=RESET_ORDER>`
    - :code:`RESET_CHILD` see :cellml2:`Section 2.9.2<reference/formal_and_informative/specB09.html?issue=RESET_CHILD>`
    - :code:`RESET_TEST_VALUE` see :cellml2:`Section 2.1<reference/formal_and_informative/specB10.html?issue=RESET_TEST_VALUE>`
    - :code:`RESET_RESET_VALUE` see :cellml2:`Section 2.11<reference/formal_and_informative/specB11.html?issue=RESET_RESET_VALUE>`
    - :code:`MATH_MATHML` see :cellml2:`Section 2.12.1<reference/formal_and_informative/specB12.html?issue=MATH_MATHML>`
    - :code:`MATH_CHILD` see :cellml2:`Section 2.12.2<reference/formal_and_informative/specB12.html?issue=MATH_CHILD>`
    - :code:`MATH_CI_VARIABLE_REF` see :cellml2:`Section 2.12.3<reference/formal_and_informative/specB12.html?issue=MATH_CI_VARIABLE_REF>`
    - :code:`MATH_CN_UNITS` see :cellml2:`Section 2.13.4<reference/formal_and_informative/specB13.html?issue=MATH_CN_UNITS>`
    - :code:`MATH_CN_BASE10` see :cellml2:`Section 2.13.5<reference/formal_and_informative/specB13.html?issue=MATH_CN_BASE10>`
    - :code:`MATH_CN_FORMAT` see :cellml2:`Section 2.13.5<reference/formal_and_informative/specB13.html?issue=MATH_CN_FORMAT>`
    - :code:`ENCAPSULATION_ATTRIBUTE` see :cellml2:`Section 2.13<reference/formal_and_informative/specB13.html?issue=ENCAPSULATION_ATTRIBUTE>`
    - :code:`ENCAPSULATION_CHILD` see :cellml2:`Section 2.13.1<reference/formal_and_informative/specB13.html?issue=ENCAPSULATION_CHILD>`
    - :code:`COMPONENT_REF_COMPONENT` see :cellml2:`Section 2.14.1<reference/formal_and_informative/specB14.html?issue=COMPONENT_REF_COMPONENT>`
    - :code:`COMPONENT_REF_CHILD` see :cellml2:`Section 2.14.2<reference/formal_and_informative/specB14.html?issue=COMPONENT_REF_CHILD>`
    - :code:`CONNECTION_ATTRIBUTE` see :cellml2:`Section 2.15<reference/formal_and_informative/specB14.html?issue=CONNECTION_ATTRIBUTE>`
    - :code:`CONNECTION_COMPONENT1` see :cellml2:`Section 2.15.1<reference/formal_and_informative/specB15.html?issue=CONNECTION_COMPONENT1>`
    - :code:`CONNECTION_COMPONENT2` see :cellml2:`Section 2.15.2<reference/formal_and_informative/specB15.html?issue=CONNECTION_COMPONENT2>`
    - :code:`CONNECTION_EXCLUDE_SELF` see :cellml2:`Section 2.15.3<reference/formal_and_informative/specB15.html?issue=CONNECTION_EXCLUDE_SELF>`
    - :code:`CONNECTION_UNIQUE` see :cellml2:`Section 2.15.4<reference/formal_and_informative/specB15.html?issue=CONNECTION_UNIQUE>`
    - :code:`CONNECTION_CHILD` see :cellml2:`Section 2.15.5<reference/formal_and_informative/specB15.html?issue=CONNECTION_CHILD>`
    - :code:`MAP_VARIABLES_ATTRIBUTE` see :cellml2:`Section 2.16<reference/formal_and_informative/specB16.html?issue=MAP_VARIABLES_ATTRIBUTE>`
    - :code:`MAP_VARIABLES_VARIABLE1` see :cellml2:`Section 2.16.1<reference/formal_and_informative/specB16.html?issue=MAP_VARIABLES_VARIABLE1>`
    - :code:`MAP_VARIABLES_VARIABLE2` see :cellml2:`Section 2.16.2<reference/formal_and_informative/specB16.html?issue=MAP_VARIABLES_VARIABLE2>`
    - :code:`MAP_VARIABLES_UNIQUE` see :cellml2:`Section 2.16.3<reference/formal_and_informative/specB16.html?issue=MAP_VARIABLES_UNIQUE>`
    - :code:`MAP_VARIABLES_AVAILABLE_INTERFACE` see :cellml2:`Section 3.10.8<reference/formal_and_informative/specC10.html?issue=MAP_VARIABLES_AVAILABLE_INTERFACE>`
    - :code:`MAP_VARIABLES_IDENTICAL_UNIT_REDUCTION` see :cellml2:`Section 3.10.9<reference/formal_and_informative/specC10.html?issue=MAP_VARIABLES_IDENTICAL_UNIT_REDUCTION>`