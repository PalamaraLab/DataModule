[![Unit tests: Windows](https://github.com/PalamaraLab/DataModule/workflows/Unit%20tests:%20Windows/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)
[![Unit tests: Ubuntu](https://github.com/PalamaraLab/DataModule/workflows/Unit%20tests:%20Ubuntu/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)
[![Unit tests: macOS](https://github.com/PalamaraLab/DataModule/workflows/Unit%20tests:%20macOS/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)

[![Static analysis checks](https://github.com/PalamaraLab/DataModule/workflows/Static%20analysis%20checks/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)
[![Sanitiser checks](https://github.com/PalamaraLab/DataModule/workflows/Sanitiser%20checks/badge.svg)](https://github.com/PalamaraLab/DataModule/actions)
[![codecov](https://codecov.io/gh/PalamaraLab/DataModule/branch/main/graph/badge.svg)](https://codecov.io/gh/PalamaraLab/DataModule)
[![BCH compliance](https://bettercodehub.com/edge/badge/PalamaraLab/DataModule?branch=main)](https://bettercodehub.com/results/PalamaraLab/DataModule)

# Data Module

Methods that underpin interacting with genome data.

:warning: this is a work in progress, and is not yet fully featured :warning:

## Quickstart

### Obtaining the source code

Get the source, together with the [vcpkg](https://github.com/microsoft/vcpkg) submodule for installing dependencies:

```bash
git clone --recurse-submodules https://github.com/PalamaraLab/DataModule.git
cd DataModule
```

### Installing the Python module

Python module coming soon.

### Installing dependencies

The recommended way to install dependencies is to use the provided scripts, which use the [vcpkg](https://github.com/microsoft/vcpkg) submodule.

On **macOS** and **Linux**, run

```bash
scripts/install_dependencies.sh
```

On **Windows**, run

```bash
scripts\install_dependencies.bat
```

### Configuring and compiling the project

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
