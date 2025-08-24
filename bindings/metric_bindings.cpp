//
// Created by Jon Sensenig on 8/23/25.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Needed for automatic vector conversion
#include "../include/is_serializable.h"
#include "../include/histogram.h"
#include "../include/tpc_monitor.h"
#include "../include/tpc_monitor_lbw.h"

namespace py = pybind11;

// The PYBIND11_MODULE macro creates the entry point for the Python module.
// The first argument is the module name. It MUST match the filename of the final library.
PYBIND11_MODULE(datamon, m) {
    m.doc() = "Python bindings for the data monitoring library"; // Optional module docstring

    // We don't define a constructor (.def(py::init<>())) because it's an abstract interface.
    // This just makes pybind11 aware of the type so derived classes can use it.
    py::class_<ISerializable>(m, "ISerializable");

    // Bind the Histogram class
    py::class_<Histogram, ISerializable>(m, "Histogram")
        .def(py::init<>()) // Bind the default constructor
        .def(py::init<int32_t, int32_t, int32_t>()) // Bind the parameterized constructor
        .def("fill", &Histogram::fill, "Fill the histogram with a value")
        .def("clear", &Histogram::clear, "Clear the histogram data")
        .def("serialize", &Histogram::serialize, "Serialize the histogram to a list of ints")
//        .def("deserialize", &Histogram::deserialize, py::arg("data"), "Deserialize from a list of ints");
        .def("deserialize", [](Histogram &self, const std::vector<int32_t> &data) {
              self.ISerializable::deserialize(data);
        }, "Deserialize from a list of ints", py::arg("data"))
        .def_property_readonly("min_value", &Histogram::getMinValue)
        .def_property_readonly("max_value", &Histogram::getMaxValue)
        .def_property_readonly("num_bins", &Histogram::getNumBins)
        .def_property_readonly("bins", &Histogram::getBins)
        .def_property_readonly("below_range_count", &Histogram::getBelowRangeCount)
        .def_property_readonly("above_range_count", &Histogram::getAboveRangeCount);

    // Bind the TpcMonitor class
    py::class_<TpcMonitor, ISerializable>(m, "TpcMonitor")
        .def(py::init<>())
        .def("clear", &TpcMonitor::clear)
        .def("serialize", &TpcMonitor::serialize)
        .def("deserialize", [](TpcMonitor &self, const std::vector<int32_t> &data) {
            self.ISerializable::deserialize(data);
         }, "Deserialize from a list of ints", py::arg("data"))

        // Expose the histograms (e.g., as read-only properties)
        .def_property_readonly("charge_histograms", &TpcMonitor::getChargeHistograms)
        .def_property_readonly("light_histograms", &TpcMonitor::getLightHistograms);

    // Bind the LowBwTpcMonitor class
    py::class_<LowBwTpcMonitor, ISerializable>(m, "LowBwTpcMonitor")
        .def(py::init<>())
        .def("clear", &LowBwTpcMonitor::clear)
        .def("serialize", &LowBwTpcMonitor::serialize)
        .def("deserialize", [](LowBwTpcMonitor &self, const std::vector<int32_t> &data) {
            self.ISerializable::deserialize(data);
        }, "Deserialize from a list of ints", py::arg("data"))
        .def("set_num_fems", &LowBwTpcMonitor::setNumFems)
        .def("set_charge_channel_samples", &LowBwTpcMonitor::setChargeChannelSamples)
        .def_property_readonly("num_fems", &LowBwTpcMonitor::getNumFems)
        .def_property_readonly("num_charge_channels", &LowBwTpcMonitor::getNumChargeChannels)
        .def_property_readonly("num_light_channels", &LowBwTpcMonitor::getNumLightChannels)
        .def_property_readonly("charge_channel_num_samples", &LowBwTpcMonitor::getChargeChannelNumSamples);
}

