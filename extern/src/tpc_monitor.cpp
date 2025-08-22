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
PYBIND11_MODULE(tpc_monitor_module, m) {
    m.doc() = "Python bindings for the C++ TPC Monitor struct";

    // Expose the Histogram struct to Python
    py::class_<TpcMonitor>(m, "TpcMonitor")
        // Bind the constructor
        .def(py::init<>())

        // Bind the 'serialize' method
        .def("serialize", &TpcMonitor::serialize, "Serializes the metrics to a list of integers")

        // No deserialize method since it returns a Histomgram object. Instead use the
        // get_<>_hists() methods which returns a list of serialized histograms which
        // can then be deserialized by the the Histogram class.

        // Get hisotgrams in serialized form
        .def("get_charge_hists", &TpcMonitor::get_charge_hists)
        .def("get_light_hists", &TpcMonitor::get_light_hists); // pybind11 converts std::vector to a Python list

}