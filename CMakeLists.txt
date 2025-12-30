cmake_minimum_required(VERSION 3.15)
project(DataMonProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This allows a parent CMake to override the value define here
option(USE_PYTHON FALSE)
message(STATUS "SET PYTHON TO ${USE_PYTHON}")

if (USE_PYTHON)
    # Add the pybind11 subdirectory
    add_subdirectory(extern/pybind11)
endif ()

# Define our C++ library source files
file(GLOB LIB_SOURCES src/*.cpp)

# Create a static library for our core C++ code.
# This avoids compiling the same code twice.
add_library(datamon_core SHARED ${LIB_SOURCES})
target_include_directories(datamon_core PUBLIC include)

if (USE_PYTHON)
    add_compile_definitions(USE_PYTHON=1)
    target_include_directories(datamon_core PUBLIC pybind11::headers)

    # Create the Python module using pybind11
    # The first argument 'datamon' is the name of the module you will import in Python.
    pybind11_add_module(datamon bindings/metric_bindings.cpp)

    # Link our core library to the Python module
    target_link_libraries(datamon PRIVATE datamon_core)
endif ()
