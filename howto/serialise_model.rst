.. _serialise_model:

=============================
Parse and print CellML syntax
=============================


.. _parse_model:

Parse from a CellML format file
===============================

The following code will read a file called :code:`sine_comparison.cellml` and deserialise its contents into a :code:`Model` instance.

Note that both the :code:`Parser` class and its opposite number, the :code:`Printer` class, deal with strings rather than files.
You'll need to read the file into a string, and then use the string as input to the :code:`Parser` item.

.. include:: /resources/code_snippets/snippet_parse_model.rst

.. _print_model:

Print a model to CellML format
==============================

The :code:`Printer` class has the opposite functionality to the :code:`Parser` class.
It accepts an existing :code:`ModelPtr` and creates a :code:`string` which can be written to a file.
As with the :code:`Parser` and the :code:`Validator`, the :code:`Printer` also records a collection of :code:`Issue` items which can be accessed as shown below.

.. tabs::

  .. code-tab:: c++

    // Create a Printer instance.
    auto printer = libcellml::Printer::create();

    // The output of the printModel function is a string representing the serialised input model.
    std::string serialisedModelString = printer->printModel(model);

    // Check the printer for issues.
    assert(printer->issueCount() == 0);

    // Write the serialised string to a file.
    std::string outFileName = "my_printed_file.cellml";
    std::ofstream outFile(outFileName);
    outFile << serialisedModelString;
    outFile.close();

  .. code-tab:: py

    from libcellml import Printer

    # Create a Printer instance.
    printer = Printer()

    # The output of the printModel function is a string representing the serialised input model.
    serialised_model = printer.printModel(model)

    # Check the printer for issues.
    assert(printer.issueCount() == 0)

    # Write the string to a file.
    write_file = open("my_printed_file.cellml", "w")
    write_file.write(serialised_model)
    write_file.close()

Details of any issues that the printer encounters and records can be retrieved as described on the :ref:`Retrieve Issue items<get_issues>` page.
