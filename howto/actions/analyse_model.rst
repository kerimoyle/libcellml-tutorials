.. _actions_analyse_model:

Analyse a model item
====================

The :code:`Analyser` class takes an existing, valid :code:`Model` item, and checks it for mathematical sense.
This includes checking things like that all variables requiring initial values have them, that equations do not conflict with one another, and that any variables used in equations are actually defined in the model.

The three basic steps to model analysis are:

#. Creating an :code:`Analyser` item and passing in a :code:`Model` for analysis;
#. Checking for any :code:`Issues` raised; and
#. (optional: for code generation only) Retrieving a :code:`AnalysedModel` item to pass to a :code:`Generator`, if required.

.. include:: ../snippets/snippet_analyser_analyse.rst

Any issues or messages raised are stored within the class's logger.
More information about accessing :code:`Issue` items can be found on the :ref:`Common actions > Retrieve Issue items<actions_get_issues>` page.

Use of the :code:`Analyser` class is a prerequisite for the :code:`Generator` class.
The generator makes use of the structures created during the analysis process, so takes a :code:`AnalyserModel` as an input.  

.. include:: ../snippets/snippet_analyser_output.rst

**TODO**
