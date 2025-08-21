//
// Created by Jon Sensenig on 8/21/25.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Needed for automatic vector conversions
#include "../../src/metrics.h"

namespace py = pybind11;

// PYBIND11_MODULE defines a function that will be called when the Python module is imported.
// The first argument is the module name (must match the name in setup.py)
// The second argument (m) is a py::module_ object that is the main interface
PYBIND11_MODULE(tpc_monitor_module, m) {
    m.doc() = "Python bindings for the C++ TPC Monitor struct";

    // Expose the Histogram struct to Python
    py::class_<TPCMonitor>(m, "TPCMonitor")
        // Bind the constructor
        .def(py::init<>())

        // Bind the 'serialize' method
        .def("serialize", &TPCMonitor::serialize, "Serializes the metrics to a list of integers")

        // Bind the static 'deserialize' method and declare it as a static method in Python
        .def_static("deserialize", &TPCMonitor::deserialize, "Deserializes a list of integers into a TPCMonitor object", py::arg("data"))

        // Get hisotgrams in serialized form
        .def("get_charge_hists", &TPCMonitor::get_charge_hists)
        .def("get_light_hists", &TPCMonitor::get_light_hists) // pybind11 converts std::vector to a Python list

        // Expose public member variables as read-only properties in Python
        .def_readonly("num_fems", &TPCMonitor::num_fems)
        .def_readonly("num_charge_channels", &TPCMonitor::num_charge_channels)
        .def_readonly("num_light_channels", &TPCMonitor::num_light_channels)
        .def_readonly("charge_channel_num_samples", &TPCMonitor::charge_channel_num_samples);
}