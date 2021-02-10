# Based on https://github.com/pybind/cmake_example

import os
import platform
import sys
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}


# A CMakeExtension needs a sourcedir instead of a file list.
# The name must be the _single_ output extension from the CMake build.
# If you need multiple extensions, see scikit-build.
class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):

    def deps_installed(self, ext):
        deps = open(os.path.join(ext.sourcedir, 'scripts', 'vcpkg_dependencies'), 'r').read().split(' ')

        if platform.system().lower() == "windows":
            vcpkg_exe = os.path.join(ext.sourcedir, 'vcpkg', 'vcpkg.exe')
        else:
            vcpkg_exe = os.path.join(ext.sourcedir, 'vcpkg', 'vcpkg')

        if os.path.isfile(vcpkg_exe):
            out = subprocess.check_output([vcpkg_exe, 'list'], encoding='UTF-8')

            return all([f'{dep.strip()}:' in out for dep in deps])

        return False

    def install_deps(self, ext):
        install_script_dir = os.path.join(ext.sourcedir, 'scripts')
        if platform.system().lower() == "windows":
            subprocess.check_call(
                [os.path.join(install_script_dir, 'install_dependencies.bat')]
            )
        else:
            subprocess.check_call(
                ['bash', os.path.join(install_script_dir, 'install_dependencies.sh')]
            )

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        if self.deps_installed(ext):
            print(f'All dependencies already installed...')
        else:
            print(f'Need to install dependencies...')
            self.install_deps(ext)

        # required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        cfg = "Debug" if self.debug else "Release"

        # CMake lets you override the generator - we need to check this.
        # Can be set with Conda-Build, for example.
        cmake_generator = os.environ.get("CMAKE_GENERATOR", "")

        # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
        # EXAMPLE_VERSION_INFO shows you how to pass a value into the C++ code
        # from Python.
        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",
            f"-DWARNINGS_AS_ERRORS=OFF",
            f"-DENABLE_TESTING=OFF",
            f"-DMAKE_DOCS=OFF",
        ]
        build_args = []

        if self.compiler.compiler_type != "msvc":
            # Using Ninja-build since it a) is available as a wheel and b)
            # multithreads automatically. MSVC would require all variables be
            # exported for Ninja to pick it up, which is a little tricky to do.
            # Users can override the generator with CMAKE_GENERATOR in CMake
            # 3.15+.
            if not cmake_generator:
                cmake_args += ["-GNinja"]

        else:

            # Single config generators are handled "normally"
            single_config = any(x in cmake_generator for x in {"NMake", "Ninja"})

            # CMake allows an arch-in-generator style for backward compatibility
            contains_arch = any(x in cmake_generator for x in {"ARM", "Win64"})

            # Specify the arch if using MSVC generator, but only if it doesn't
            # contain a backward-compatibility arch spec already in the
            # generator name.
            if not single_config and not contains_arch:
                cmake_args += ["-A", PLAT_TO_CMAKE[self.plat_name]]

            # Multi-config generators have a different way to specify configs
            if not single_config:
                cmake_args += [
                    "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(cfg.upper(), extdir)
                ]
                build_args += ["--config", cfg]

        # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
        # across all generators.
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            # self.parallel is a Python 3 only way to set parallel jobs by hand
            # using -j in the build_ext call, not supported by pip or PyPA-build.
            if hasattr(self, "parallel") and self.parallel:
                # CMake 3.12+ only.
                build_args += ["-j{}".format(self.parallel)]

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
        )
        subprocess.check_call(
            ["cmake", "--build", ".", "--target", f"{ext.name}"] + build_args, cwd=self.build_temp
        )


# The information here can also be placed in setup.cfg - better separation of
# logic and declaration, and simpler if you include description/version in a file.
setup(
    name="asmc_data_module",
    version="0.0.1",
    author="Palamara Lab",
    description="Something",
    long_description="",
    ext_modules=[CMakeExtension("asmc_data_module")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    install_requires=[
        'cmake',
        'ninja',
        'jupyter',
    ],
    extras_require={
        'dev': [
            'flake8',
            'nbconvert',
            'pytest',
        ],
    },
)
