.. _examples_parse_model:

=======================================
Parse an existing ``Model`` from a file
=======================================

The following code will read a file called :code:`sine_comparison.cellml` and deserialise its contents into a :code:`Model` instance.

Note that the :code:`Parser` class will make a note of any issues it finds, which can be accessed as described in the :ref:`Retrieve Issue items<examples_get_issues>` page.

.. include:: ../code_snippets/snippet_parse_model.rst

.. This example can be seen in more context in:
.. - :ref:`Tutorial 1: Reading and writing CellML files<tutorial1>` and
.. - :ref:`Tutorial 2: Debugging, error checking and validating<tutorial2>`.
