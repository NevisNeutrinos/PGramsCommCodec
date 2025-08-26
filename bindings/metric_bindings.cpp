//
// Created by Jon Sensenig on 8/23/25.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // Needed for automatic vector conversion
#include "../include/metric_base.h"
#include "../include/histogram.h"
#include "../include/tpc_monitor.h"
#include "../include/tpc_monitor_lbw.h"

namespace py = pybind11;

// A trampoline class is needed for pybind11 to handle virtual functions
// that might be overridden in Python.
class PyMetricBase : public MetricBase {
public:
    // Inherit constructors
    using MetricBase::MetricBase;

    std::vector<int32_t> serialize() const override {
        PYBIND11_OVERRIDE_PURE(
            std::vector<int32_t>, /* Return type */
            MetricBase,        /* Parent class */
            serialize             /* Function name */
        );
    }

    // Trampoline for the pure virtual deserialize
    std::vector<int32_t>::const_iterator deserialize(
        std::vector<int32_t>::const_iterator begin,
        std::vector<int32_t>::const_iterator end) override {
        PYBIND11_OVERRIDE_PURE(
            std::vector<int32_t>::const_iterator, /* Return type */
            MetricBase,                           /* Parent class */
            deserialize,                          /* Function name */
            begin, end                            /* Arguments */
        );
    }

    // Trampoline for the pure virtual getMetricDict
    py::dict getMetricDict() override {
        PYBIND11_OVERRIDE_PURE(
            py::dict,       /* Return type */
            MetricBase,     /* Parent class */
            getMetricDict  /* Function name */
        );
    }
};

// The PYBIND11_MODULE macro creates the entry point for the Python module.
// The first argument is the module name. It MUST match the filename of the final library.
PYBIND11_MODULE(datamon, m) {
    m.doc() = "Python bindings for the data monitoring library"; // Optional module docstring

    // We don't define a constructor (.def(py::init<>())) because it's an abstract interface.
    // This just makes pybind11 aware of the type so derived classes can use it.
    // py::class_<MetricBase>(m, "MetricBase");
    py::class_<MetricBase, PyMetricBase /* trampoline */>(m, "MetricBase")
        .def(py::init<>())
        .def("deserialize", static_cast<void (MetricBase::*)(const std::vector<int32_t>&)>(&MetricBase::deserialize), "Deserialize data from a list of integers.")
        .def("get_metric_dict", &MetricBase::getMetricDict, "Deserialize data and return a dictionary.");


    // Bind the Histogram class
    py::class_<Histogram, MetricBase>(m, "Histogram")
        .def(py::init<>()) // Bind the default constructor
        .def(py::init<int32_t, int32_t, int32_t>()) // Bind the parameterized constructor
        .def("fill", &Histogram::fill, "Fill the histogram with a value")
        .def("clear", &Histogram::clear, "Clear the histogram data")
        .def("serialize", &Histogram::serialize, "Serialize the histogram to a list of ints")
//        .def("deserialize", &Histogram::deserialize, py::arg("data"), "Deserialize from a list of ints");
        // .def("deserialize", [](Histogram &self, const std::vector<int32_t> &data) {
        //       self.MetricBase::deserialize(data);
        // }, "Deserialize from a list of ints", py::arg("data"))
        // .def("get_metric_dict", &Histogram::getMetricDict, "Get the metric dictionary")
        .def_property_readonly("min_value", &Histogram::getMinValue)
        .def_property_readonly("max_value", &Histogram::getMaxValue)
        .def_property_readonly("num_bins", &Histogram::getNumBins)
        .def_property_readonly("bins", &Histogram::getBins)
        .def_property_readonly("below_range_count", &Histogram::getBelowRangeCount)
        .def_property_readonly("above_range_count", &Histogram::getAboveRangeCount);

    // Bind the TpcMonitor class
    py::class_<TpcMonitor, MetricBase>(m, "TpcMonitor")
        .def(py::init<>())
        .def("clear", &TpcMonitor::clear)
        .def("serialize", &TpcMonitor::serialize)
        // .def("deserialize", [](TpcMonitor &self, const std::vector<int32_t> &data) {
        //     self.MetricBase::deserialize(data);
        //  }, "Deserialize from a list of ints", py::arg("data"))
        // .def("get_metric_dict", &TpcMonitor::getMetricDict, "Get the metric dictionary")

        // Expose the histograms (e.g., as read-only properties)
        .def_property_readonly("charge_histograms", &TpcMonitor::getChargeHistograms)
        .def_property_readonly("light_histograms", &TpcMonitor::getLightHistograms);

    // Bind the LowBwTpcMonitor class
    py::class_<LowBwTpcMonitor, MetricBase>(m, "LowBwTpcMonitor")
        .def(py::init<>())
        .def("clear", &LowBwTpcMonitor::clear)
        .def("serialize", &LowBwTpcMonitor::serialize)
        // .def("deserialize", [](LowBwTpcMonitor &self, const std::vector<int32_t> &data) {
        //     self.MetricBase::deserialize(data);
        // }, "Deserialize from a list of ints", py::arg("data"))
        // .def("get_metric_dict", &LowBwTpcMonitor::getMetricDict, "Get the metric dictionary")
        .def("set_num_fems", &LowBwTpcMonitor::setNumFems)
        .def("set_charge_channel_samples", &LowBwTpcMonitor::setChargeChannelSamples)
        .def_property_readonly("num_fems", &LowBwTpcMonitor::getNumFems)
        .def_property_readonly("num_charge_channels", &LowBwTpcMonitor::getNumChargeChannels)
        .def_property_readonly("num_light_channels", &LowBwTpcMonitor::getNumLightChannels)
        .def_property_readonly("charge_channel_num_samples", &LowBwTpcMonitor::getChargeChannelNumSamples);
}

