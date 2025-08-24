cmake_minimum_required(VERSION 3.15)
project(DataMonProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the pybind11 subdirectory
add_subdirectory(extern/pybind11)

# Define our C++ library source files
set(LIB_SOURCES
        src/histogram.cpp
        src/tpc_monitor.cpp
        src/tpc_monitor_lbw.cpp
)

# Create a static library for our core C++ code.
# This avoids compiling the same code twice.
add_library(datamon_core SHARED ${LIB_SOURCES})
target_include_directories(datamon_core PUBLIC include)

# Create the Python module using pybind11
# The first argument 'datamon' is the name of the module you will import in Python.
pybind11_add_module(datamon bindings/metric_bindings.cpp)

# Link our core library to the Python module
target_link_libraries(datamon PRIVATE datamon_core)