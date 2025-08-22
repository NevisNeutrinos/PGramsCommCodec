//
// Created by Jon Sensenig on 8/21/25.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Needed for automatic vector conversions
#include "../../include/metrics.h"

namespace py = pybind11;

// PYBIND11_MODULE defines a function that will be called when the Python module is imported.
// The first argument is the module name (must match the name in setup.py)
// The second argument (m) is a py::module_ object that is the main interface
PYBIND11_MODULE(histogram_module, m) {
    m.doc() = "Python bindings for a C++ Histogram struct"; // Optional module docstring

    // Expose the Histogram struct to Python
    py::class_<Histogram>(m, "Histogram")
        // Bind the constructor
        .def(py::init<int32_t, int32_t, int32_t>(),
             py::arg("min_val"), py::arg("max_val"), py::arg("num_bins"))

        // Bind the 'fill' method
        .def("fill", &Histogram::fill, "Adds a data point to the histogram", py::arg("value"))

        // Bind the 'serialize' method
        .def("serialize", &Histogram::serialize, "Serializes the histogram to a list of integers")

        // Bind the static 'deserialize' method and declare it as a static method in Python
        .def_static("deserialize", &Histogram::deserialize, "Deserializes a list of integers into a Histogram object", py::arg("data"))

        // Expose public member variables as read-only properties in Python
        .def_readonly("min_value", &Histogram::min_value)
        .def_readonly("max_value", &Histogram::max_value)
        .def_readonly("num_bins", &Histogram::num_bins)
        .def_readonly("below_range_count", &Histogram::below_range_count)
        .def_readonly("above_range_count", &Histogram::above_range_count)
        .def_readonly("bins", &Histogram::bins); // pybind11 converts std::vector to a Python list
}
