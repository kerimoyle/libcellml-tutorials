.. _snippet_build_solver:

.. container:: toggle

  .. container:: header-left

    Build a solver using generated code

  .. tabs::

    .. tab:: C++ snippet

      This file should be saved as a CMakeLists.txt file for generation with CMake.

      .. literalinclude:: /resources/code/solver/CMakeLists.txt
        :language: cmake

    .. tab:: Python snippet

      .. literalinclude:: /resources/code/solver/solveGeneratedModel.py
        :language: python
        :linenos:
        :lineno-start: 67
        :start-after: # MODULE FROM FILE
        :end-before: # END MODULE FROM FILE
      
      Full context: :download:`solveGeneratedModel.py</resources/code/solver/solveGeneratedModel.py>`
