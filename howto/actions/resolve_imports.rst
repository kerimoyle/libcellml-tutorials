.. _actions_resolve_imports:

:orphan:

Resolve the imported items in a model
=====================================

The import functionality for :code:`Units` and :code:`Component` items is key to enabling the reuse and sharing of models.
The import statements are basically a recipe for how these imported items can be combined to make the present model.
While models which contain import dependencies are perfectly valid, they cannot be used to generate runnable code.
The process of resolving the imports (telling libCellML where to look for these ingredients) and flattening the model (creating instances of the ingredients and removing the dependency) is necessary before code generation can happen.

The :code:`Importer` class supports all functionality to do with imports, and contains its own logger which can be used to report anything that might have gone wrong. 

.. include:: ../code_snippets/snippet_resolve_imports.rst
