from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

# Define the C++ source files for the extension
sources = [
    'bindings/metric_bindings.cpp',
    'src/histogram.cpp',
    'src/tpc_monitor.cpp',
    'src/tpc_monitor_lbw.cpp',
]

ext_modules = [
    Pybind11Extension(
        "datamon",  # Name of the module in Python
        sources,
        # Define the C++ standard to use
        cxx_std=17,
    ),
]

setup(
    name='datamon',
    version='0.1.0',
    author='Your Name',
    description='Python bindings for the data monitoring library',
    ext_modules=ext_modules,
    # The build_ext command from pybind11.setup_helpers handles compiler flags
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)