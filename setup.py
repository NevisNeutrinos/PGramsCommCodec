import os
import shutil
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools.command.build_ext import build_ext as build_ext_orig

# Define the C++ source files for the extension
sources = [
    'bindings/metric_bindings.cpp',
    'src/histogram.cpp',
    'src/tpc_monitor.cpp',
    'src/tpc_monitor_lbw.cpp',
    'src/tpc_configs.cpp',
    'src/daq_comp_monitor.cpp',
    'src/tpc_readout_monitor.cpp'
]

ext_modules = [
    Pybind11Extension(
        "datamon",  # Name of the module in Python
        sources,
        include_dirs=['include'],
        # Define the C++ standard to use
        cxx_std=17,
        define_macros=[('USE_PYTHON', '1')]
    ),
]

class CustomBuildExt(build_ext_orig):
    """A custom build_ext command to clean up previous builds."""
    def run(self):
        # --- Your Custom Cleaning Logic ---
        print("🧹 Running custom clean step")

        # List of directories to remove
        directories_to_remove = ['./build', './dist']

        for directory in directories_to_remove:
            if os.path.exists(directory):
                print(f"   - Removing directory: {directory}")
                shutil.rmtree(directory)

        # We also need to find and remove the .egg-info directory
        # It's usually in the project root
        for item in os.listdir('.'):
            if item.endswith('.egg-info'):
                egg_info_dir = os.path.join('.', item)
                print(f"   - Removing directory: {egg_info_dir}")
                shutil.rmtree(egg_info_dir)

        # --- Call the original build_ext command ---
        print("✨ Proceeding with the original build...")
        super().run()

setup(
    name='datamon',
    version='0.1.0',
    author='Your Name',
    description='Python bindings for the data monitoring library',
    ext_modules=ext_modules,
    # The build_ext command from pybind11.setup_helpers handles compiler flags
    cmdclass={"build_ext": CustomBuildExt},
    zip_safe=False,
    python_requires=">=3.7",
)
