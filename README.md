
# `libclang-utils`


libclang-utils is a lightweight C++ wrapper for [libclang](https://clang.llvm.org/doxygen/group__CINDEX.html), 
the C interface to Clang.

Because libclang-utils defines its own header for libclang (see [cindex.h](include/libclang-utils/cindex.h))
and attempts to load the library dynamically, it can be compiled without having clang installed 
on the building machine.

## Building

See the definition of the [CMake Build workflow](https://github.com/strandfield/libclang-utils/actions) in 
[cmake-build.yml](.github/workflows/cmake-build.yml) for the steps.

libclang-utils uses the CMake build system so the above step should work both on Linux 
and Windows.
