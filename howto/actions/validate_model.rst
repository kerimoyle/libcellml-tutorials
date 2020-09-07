.. _examples_validate_model:

Validate a ``Model``
====================

The :code:`Validator` is the equivalent of a spelling checker: it can check that each item in a model has all of the information it needs, but it can't check whether it means what you intend it to.
Thus even if a model is valid, it could still be the equivalent of correctly-spelled nonsense.

.. include:: ../code_snippets/snippet_validate_model.rst

Once a model has been passed to a :code:`Validator` instance, the validator's internal logger will contain a list of any of the issues which have been encountered during the checking process. 
A model can be said to be valid - that is, conforming to the :cellml2:`CellML normative specification <>` - if the validator's logger contains no issues with a level of :code:`ERROR`.
