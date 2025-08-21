cmake_minimum_required(VERSION 3.15)
project(pgrams_codec)

set(CMAKE_CXX_STANDARD 17)

# This allows a parent CMake to overide the value define here
option(USE_PYTHON TRUE)
message(STATUS "SET PYTHON TO ${USE_PYTHON}")

file(GLOB CODEC_SRC src/*cpp)
if(USE_PYTHON)
    message(STATUS "Compiling codec with python bindings..")
    include_directories(src extern/pybind11)
    add_subdirectory(extern/pybind11)
#    add_executable(raw_decoder run_decode.cpp
#            ${DECODER_SRC})
    target_link_libraries(raw_decoder PRIVATE pybind11::module pybind11::embed)
else()
    message(STATUS "No Codec python bindings, use as C++ headers only..")
endif ()