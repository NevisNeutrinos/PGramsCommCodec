cmake_minimum_required(VERSION 3.15)
project(codec_bindings)

set(CMAKE_CXX_STANDARD 17)
#add_compile_definitions(USE_PYBIND11=1)

find_package(pybind11 CONFIG REQUIRED)
#add_subdirectory(extern/pybind11)
include_directories(../include)
pybind11_add_module(codec_bindings
                    src/tpc_monitor.cpp)

install(TARGETS codec_bindings DESTINATION .)