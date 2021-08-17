[![Unit tests: Windows](https://github.com/PalamaraLab/DataModule/workflows/Unit%20tests:%20Windows/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)
[![Unit tests: Ubuntu](https://github.com/PalamaraLab/DataModule/workflows/Unit%20tests:%20Ubuntu/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)
[![Unit tests: macOS](https://github.com/PalamaraLab/DataModule/workflows/Unit%20tests:%20macOS/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)

[![Python 3.6-3.9 on Linux](https://github.com/PalamaraLab/DataModule/workflows/Python%203.6-3.9%20on%20Linux/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)

[![Static analysis checks](https://github.com/PalamaraLab/DataModule/workflows/Static%20analysis%20checks/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)
[![Sanitiser checks](https://github.com/PalamaraLab/DataModule/workflows/Sanitiser%20checks/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)

[![Documentation](https://readthedocs.org/projects/data-module/badge/?version=latest)](https://data-module.readthedocs.io/en/latest/?badge=latest)
[![codecov](https://codecov.io/gh/PalamaraLab/DataModule/branch/main/graph/badge.svg)](https://codecov.io/gh/PalamaraLab/DataModule)

# Data Module

Methods that underpin interacting with genome data.

:warning: this is a work in progress, and is not yet fully featured :warning:

## Quickstart

This project is a C++ library with optional Python bindings.
The Python bindings provide a Python module called `asmc_data_module`.

### Obtaining the source code

Get the source, together with the [vcpkg](https://github.com/microsoft/vcpkg) submodule (for installing dependencies) and the [pybind11](https://github.com/pybind/pybind11) submodule (for Python bindings):

```bash
git clone --recurse-submodules https://github.com/PalamaraLab/DataModule.git
cd DataModule
```

### Installing dependencies

The C++ code requires the following dependencies:

- [eigen3](https://eigen.tuxfamily.org/index.php) (linear algebra)
- [{fmt}](https://github.com/fmtlib/fmt) (text formatting) 
- [range-v3](https://github.com/ericniebler/range-v3) (pre C++20 ranges support)
- [zlib](https://zlib.net/) (compression)

The recommended way to install dependencies is via the built-in [vcpkg](https://github.com/microsoft/vcpkg) CMake integration.
Assuming you have checked out the vcpkg submodule, the dependencies will be automatically installed when you run the CMake configure step below.

### Installing the Python module

If you want to use this project as a Python module, you only need to run:

```bash
pip install .
```

This will work even if you have not installed the dependencies, but if you are a developer who may be installing the Python bindings multiple times, you should install the dependencies once, first, for efficiency.

### Configuring and compiling the C++ project

This project uses [CMake](https://cmake.org/).

Create a build directory:

```bash
mkdir build
cd build
```

Configure using the vcpkg toolchain file:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
```

Build the DataModule library:

```bash
cmake --build . --parallel 4 --target data_module_lib
```

Build the and run the unit tests:

```bash
cmake --build . --parallel 4 --target unit_tests
ctest --output-on-failure
```

## Extra tools

### Coverage

Configure with coverage enabled, using `g++` or `clang++`:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build . --parallel 2 --target unit_tests
ctest -j2 --output-on-failure
lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/vcpkg/*' '*/test/*' --output-file coverage.info
lcov --list coverage.info
```

### Static analysis

This project is configured to work with [clang tidy](https://clang.llvm.org/extra/clang-tidy/) and [cppcheck](http://cppcheck.sourceforge.net/).
Enable the relevant option and compile with `clang++`.

For clang tidy:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_CLANG_TIDY=ON
cmake --build . --parallel 2 --target data_module_lib
```

For cppcheck:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_CPPCHECK=ON
cmake --build . --parallel 2 --target data_module_lib
```

### Sanitisers

This project is configured to work with various LLVM sanitisers.
Enable the relevant option, compile with `clang++`, and run the unit tests.

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITISER_{{{SANITISER}}}=ON
cmake --build . --parallel 2 --target unit_tests
./test/unit_tests
```

where `{{{SANITISER}}}` is one of:

- [ADDRESS](https://clang.llvm.org/docs/AddressSanitizer.html)
- [LEAK](https://clang.llvm.org/docs/LeakSanitizer.html)
- [UNDEFINED_BEHAVIOUR](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html)
- [THREAD](https://clang.llvm.org/docs/ThreadSanitizer.html)

## License

This project is currently released under the GNU General Public License Version 3.
