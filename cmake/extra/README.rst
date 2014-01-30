Extra CMake scripts used in my projects.
========================================
.. image:: http://stillmaintained.com/berenm/cmake-extra.png
    :alt: Still Maintained?
    :target: http://stillmaintained.com/berenm/cmake-extra

USAGE
````````````````````````````
1. Download the CMakeExtraBootstrap.cmake file in a cmake/ subfolder of your project:

.. code:: bash

  mkdir cmake
  wget http://git.io/CMakeExtraBootstrap.cmake -P cmake/


2. Add the following lines at the beginning of your CMakeLists.txt:

.. code:: cmake

  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
  include(CMakeExtraBootstrap)


``cmake-extra`` will automatically try to keep itself up-to-date.


COPYING INFORMATION
````````````````````````````

 Distributed under the Boost Software License, Version 1.0.

 See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
