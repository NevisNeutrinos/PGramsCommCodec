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
PYBIND11_MODULE(low_bw_tpc_monitor_module, m) {
    m.doc() = "Python bindings for the C++ Low Bandwidth TPC Monitor struct";

    // Expose the Histogram struct to Python
    py::class_<LowBwTpcMonitor>(m, "LowBwTpcMonitor")
        // Bind the constructor
        .def(py::init<>())

        // Bind the 'serialize' method
        .def("serialize", &LowBwTpcMonitor::serialize, "Serializes the metrics to a list of integers")

        // Bind the static 'deserialize' method and declare it as a static method in Python
        .def_static("deserialize", &LowBwTpcMonitor::deserialize, "Deserializes a list of integers into a LowBwTpcMonitor object", py::arg("data"))

        // Expose public member variables as read-only properties in Python
        .def_readonly("num_fems", &LowBwTpcMonitor::num_fems)
        .def_readonly("num_charge_channels", &LowBwTpcMonitor::num_charge_channels)
        .def_readonly("num_light_channels", &LowBwTpcMonitor::num_light_channels)
        .def_readonly("charge_channel_num_samples", &LowBwTpcMonitor::charge_channel_num_samples);
}