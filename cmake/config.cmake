# Only a default: an unconditional set() here would shadow the CMAKE_BUILD_TYPE cache variable and
# silently override -DCMAKE_BUILD_TYPE=... (e.g. the build type scikit-build-core passes for wheel
# builds).
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()
# Keep defaults overridable by an embedding project or a -D cache argument.
# An unconditional set() shadows the cache and can unexpectedly enable Python
# bindings when a C++ consumer has explicitly disabled them.
option(XGRAMMAR_BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
option(XGRAMMAR_ENABLE_COVERAGE "Enable code coverage with gcov" OFF)
option(XGRAMMAR_BUILD_CXX_TESTS "Build C++ tests" OFF)
option(XGRAMMAR_ENABLE_CPPTRACE "Enable C++ trace" OFF)
option(XGRAMMAR_ENABLE_INTERNAL_CHECK "Enable internal checks" OFF)
