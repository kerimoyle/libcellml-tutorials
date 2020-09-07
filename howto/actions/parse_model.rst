.. _actions_parse_model:

Parse an existing ``Model`` from a file
=======================================

The following code will read a file called :code:`sine_comparison.cellml` and deserialise its contents into a :code:`Model` instance.

Note that both the :code:`Parser` class and its opposite number, the :code:`Printer` class, deal with strings rather than files.
You'll need to read the file into a string, and then use the string as input to the :code:`Parser` item.

.. include:: ../code_snippets/snippet_parse_model.rst
