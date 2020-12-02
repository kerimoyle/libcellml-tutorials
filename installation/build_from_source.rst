..  _build_from_source:

=============================
Build and install from source
=============================

.. contents:: Contents
    :local:

0: Gather the prerequisites
===========================
If you're working on a Windows or Linux machine you'll need to have :libxml2:`libxml2<>` installed.
It can be downloaded from :libxml2_download:`this page<>`.
This is present by default on MacOS operating systems.

If you'd like to build the Python bindings, you'll need to have access to :swig:`SWIG<>`.
If you will only be working in C++ then you don't need this step.

1: Create the libCellML library
===============================

.. container:: dothis

  **1.a** Create an empty folder and navigate into it.

  .. code-block:: text

    mkdir MyLibCellMLDirectory
    cd MyLibCellMLDirectory

.. container:: dothis

  **1.b** Clone the branch from the libCellML library github repository.
  This will create and populate a folder called :code:`libcellml`.

  .. code-block:: text

    git clone -b develop --single-branch https://github.com/cellml/libcellml.git

Your folder structure is now:

.. code-block:: text

  ── MyLibCellMLDirectory
      └─ libcellml
          └─ ( ... contains the files you cloned )

.. container:: dothis

  **1.c** Rename the cloned :code:`libcellml` folder to be :code:`source`, and create sibling folders called :code:`build` and :code:`install`:

  .. code-block:: text

    mv libcellml source
    mkdir build
    mkdir install

Your folder structure should now look like this:

.. code-block:: text

  ── MyLibCellMLDirectory
      ├─ build
      │   └─ ( ... empty )
      ├─ install
      │   └─ ( ... empty )
      └─ source
          └─ ( ... contains the files you cloned )

2: Build and install the library
================================

.. container:: dothis

  **2.a** Navigate into the :code:`MyLibCellMLDirectory/build` directory to make the library:

  .. code-block:: text

    cd build
    cmake -DINSTALL_PREFIX=../install -S=../source -B=.
    make -j

.. container:: infospec

    CMake uses command line flags as well as the arguments' positions to determine what each of your inputs means.
    In the example above the :code:`-S` flag indicates the source directory, and the :code:`-B` flag indicates the build directory.
    By default, if these flags are not present, CMake will take the first unflagged item as the source directory (if there are none then it uses the current directory), and the second (if there is no second then it uses the current directory) as the build directory.
    Thus, if called from the build directory, then these are equivalent:

    .. code-block:: text

      cmake -DINSTALL_PREFIX=../install -S=../source -B=.  # Explicitly specifying directories.
      cmake -DINSTALL_PREFIX=../install ../source .        # First argument is the source, second argument is the build.
      cmake -DINSTALL_PREFIX=../install ../source          # First argument is the source, build is the current directory.

You should see output which runs through some checks (make sure they all pass), and finishes with a message like this:

.. code-block:: text

     ( ... lots of checks ... )
  -- Configuring done
  -- Generating done
  -- Build files have been written to: /Users/YourName/MyLibCellMLDirectory/build

.. container:: dothis

  **2.b** From the same location, install the library:

  .. code-block:: text

      make -j install

You should see output which finishes with a list of library and include files like this:

.. code-block:: text

  Install the project...
  -- Install configuration: "Debug"
  -- Installing: /Users/YourName/MyLibCellMLDirectory/build/../install/lib/libcellmld.0.2.0.dylib
  -- Installing: /Users/YourName/MyLibCellMLDirectory/build/../install/lib/libcellmld.dylib
      ...

Your directory structure should now look like:

.. code-block:: text

  ── MyLibCellMLDirectory
      ├─ build
      │   └─ ( ... lots of files from the build )
      ├─ install
      │   ├─ include
      │   └─ lib
      └─ source
          └─ ( ... the files you cloned in step 1.b )

.. container:: dothis

  **2.c (optional)** If you intend to use the Python bindings, you will need to install the Python version of the local library using pip3.
  Again, this expects that you are still in the :code:`build` directory.

  .. code-block:: text

    pip3 install -e src/bindings/python

.. container:: dothis

  **2.d (optional)** Run the tests on the library from the :code:`build` directory.
  Expect to see that all tests have passed.

  .. code-block:: text

    make test

3: Test the installation: C++
=============================

If you are intending to use the Python library only, please skip to :ref:`Test the installation: Python<test_python_installation>`.

.. container:: dothis

  **3.a** Download the following files to a folder, and navigate into it:

  - :download:`CMakeLists.txt<CMakeLists.txt>` 
  - :download:`test_install.cpp<test_install.cpp>` 

  .. code-block:: text

    cd wherever/you/downloaded/them

.. container:: dothis

  **3.b** Run CMake to create the Makefile.
  Note that you will need to specify the path to the :code:`install` directory you created in step 2.b using the :code:`-DINSTALL_PREFIX` flag.

  .. code-block:: text

    cmake -DINSTALL_PREFIX=path/to/your/install 

If this is correct you should see an output ending similar to this:

.. code-block:: text

    -- The C compiler identification is AppleClang 10.0.1.10010046
    -- The CXX compiler identification is AppleClang 10.0.1.10010046
    -- Check for working C compiler: /Library/Developer/CommandLineTools/usr/bin/cc
    -- Check for working C compiler: /Library/Developer/CommandLineTools/usr/bin/cc -- works
    -- Detecting C compiler ABI info
    -- Detecting C compiler ABI info - done
    -- Detecting C compile features
    -- Detecting C compile features - done
    -- Check for working CXX compiler: /Library/Developer/CommandLineTools/usr/bin/c++
    -- Check for working CXX compiler: /Library/Developer/CommandLineTools/usr/bin/c++ -- works
    -- Detecting CXX compiler ABI info
    -- Detecting CXX compiler ABI info - done
    -- Detecting CXX compile features
    -- Detecting CXX compile features - done
    -- Configuring done
    -- Generating done
    -- Build files have been written to: path/to/your/current/directory

.. container:: dothis

  **3.c** Now build the :code:`test_install` example

  .. code-block:: text

    make -j

You should see an output similar to this:

.. code-block:: text

    Scanning dependencies of target test_install
    [ 50%] Building CXX object CMakeFiles/test_install.dir/test_install.cpp.o
    [100%] Linking CXX executable test_install
    [100%] Built target test_install

.. container:: dothis

    **3.d** Finally, run the program to output the welcome and version number of the linked libCellML installation:

  .. code-block:: text

    ./test_install

You should see the output of:

.. code-block:: text

  -----------------------------------------------
      Welcome to libCellML!
      This version number is 0.2.0
  -----------------------------------------------

.. _test_python_installation:

4: Test the installation: Python
================================

.. container:: dothis

  **4.a** Download the file below and save it.
  Navigate to the directory where it is saved.
  
  - :download:`test_install.py<test_install.py>` 

  .. code-block:: text

    cd wherever/you/downloaded/it

.. container:: dothis

  **4.b** Run the test script from here.

  .. code-block:: text

    python3 test_install.py

You should see the output of:

.. code-block:: text

  -----------------------------------------------
      Welcome to libCellML!
      This version number is 0.2.0
  -----------------------------------------------

.. container:: dothis

  **4.c** Go and have a cuppa, you're done!