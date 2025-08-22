cmake_minimum_required(VERSION 3.15)
project(pgrams_codec)

set(CMAKE_CXX_STANDARD 17)

# This allows a parent CMake to overide the value defined here
option(USE_PYTHON TRUE)
message(STATUS "SET PYTHON TO ${USE_PYTHON}")

if(USE_PYTHON)
    message(STATUS "Compiling codec with python bindings..")
else()
    message(STATUS "No Codec python bindings, use as C++ headers only..")
endif ()