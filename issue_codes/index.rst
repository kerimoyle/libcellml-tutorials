.. _issue_codes:

===========================
Run-time return issue codes
===========================

Many of the classes in libCellML inherit a :code:`Logger` class, which keeps track of things that may have gone wrong, as well as giving hints when the behaviour may not be as expected.
This page keeps a list of the codes that are not related to CellML syntactic validation.
Validation issues are pointed to the relevant section in the :cellml2:`CellML 2.0 Normative Specification document<>` instead.

Analyser codes
==============

.. _ANALYSER_VARIABLE_INITIALISED_MORE_THAN_ONCE:

ANALYSER_VARIABLE_INITIALISED_MORE_THAN_ONCE
--------------------------------------------

.. _ANALYSER_VARIABLE_NON_CONSTANT_INITIALISATION:

ANALYSER_VARIABLE_NON_CONSTANT_INITIALISATION
---------------------------------------------

.. _ANALYSER_VOI_INITIALISED:

ANALYSER_VOI_INITIALISED
------------------------

.. _ANALYSER_VOI_SEVERAL:

ANALYSER_VOI_SEVERAL
--------------------

.. _ANALYSER_ODE_NOT_FIRST_ORDER:

ANALYSER_ODE_NOT_FIRST_ORDER
----------------------------

.. _ANALYSER_VARIABLE_UNUSED:

ANALYSER_VARIABLE_UNUSED
------------------------


.. _ANALYSER_STATE_NOT_INITIALISED:

ANALYSER_STATE_NOT_INITIALISED
------------------------------


.. _ANALYSER_VARIABLE_COMPUTED_MORE_THAN_ONCE:

ANALYSER_VARIABLE_COMPUTED_MORE_THAN_ONCE
-----------------------------------------


.. _ANALYSER_EXTERNAL_VARIABLE_DIFFERENT_MODEL:

ANALYSER_EXTERNAL_VARIABLE_DIFFERENT_MODEL
------------------------------------------


.. _ANALYSER_EXTERNAL_VARIABLE_VOI:

ANALYSER_EXTERNAL_VARIABLE_VOI
------------------------------


.. _ANALYSER_EXTERNAL_VARIABLE_USE_PRIMARY_VARIABLE:

ANALYSER_EXTERNAL_VARIABLE_USE_PRIMARY_VARIABLE
-----------------------------------------------


Importer codes
==============

.. _IMPORTER_NULL_MODEL:

IMPORTER_NULL_MODEL
-------------------

.. _IMPORTER_MISSING_COMPONENT:

IMPORTER_MISSING_COMPONENT
--------------------------

.. _IMPORTER_MISSING_UNITS:

IMPORTER_MISSING_UNITS
----------------------

Parser codes
============

Printer codes
=============

Miscellaneous codes
===================

.. _MAP_VARIABLES_IDENTICAL_UNIT_REDUCTION:

MAP_VARIABLES_IDENTICAL_UNIT_REDUCTION
--------------------------------------


.. _INVALID_ARGUMENT:

INVALID_ARGUMENT
----------------


