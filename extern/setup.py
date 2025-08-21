from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

# Define the C++ extension module
ext_modules = [
    Pybind11Extension(
        # The name of the module as it will be imported in Python
        "histogram_module",
        # The source file for the bindings
        ["src/histogram.cpp"],
        # Specify C++11 standard
        extra_compile_args=["-std=c++11"],
        language='c++'
    ),
    # Second module: tpc_monitor_module
    Pybind11Extension(
        "tpc_monitor_module",
        ["src/tpc_monitor.cpp"], # Source file for this module
        extra_compile_args=["-std=c++11"],
        language='c++',
    ),
]

setup(
    name="codec_module",
    version="1.0",
    author="Your Name",
    description="Python bindings for pGRAMS Codec",
    ext_modules=ext_modules,
    # Use the custom build command from pybind11
    cmdclass={"build_ext": build_ext},
)