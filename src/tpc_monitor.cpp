//
// Created by Jon Sensenig on 8/23/25.
//

#include "../include/tpc_monitor.h"
#include <iostream>

TpcMonitor::TpcMonitor() {
    // Initialize histograms with their specific configurations
    charge_histograms.assign(NUM_CHARGE_CHANNELS, Histogram(1024, 4096, CHARGE_BINS));
    light_histograms.assign(NUM_LIGHT_CHANNELS, Histogram(1596, 4096, LIGHT_BINS));
}

void TpcMonitor::clear() {
    for (auto& hist : charge_histograms) hist.clear();
    for (auto& hist : light_histograms) hist.clear();
}

std::vector<int32_t> TpcMonitor::serialize() const {
    std::vector<int32_t> serialized_data;
    // Serialize each member object and append its data
    for (const auto& hist : charge_histograms) {
        auto hist_data = hist.serialize();
        serialized_data.insert(serialized_data.end(), hist_data.begin(), hist_data.end());
    }
    for (const auto& hist : light_histograms) {
        auto hist_data = hist.serialize();
        serialized_data.insert(serialized_data.end(), hist_data.begin(), hist_data.end());
    }
    serialized_data.insert(serialized_data.end(), channel_mean.begin(), channel_mean.end());
    serialized_data.insert(serialized_data.end(), channel_stddev.begin(), channel_stddev.end());
    return serialized_data;
}

std::vector<int32_t>::const_iterator TpcMonitor::deserialize(std::vector<int32_t>::const_iterator begin,
                                                             std::vector<int32_t>::const_iterator end) {
    auto it = begin;
    // Let each histogram deserialize its own part of the data stream
    for (auto& hist : charge_histograms) {
        it = hist.deserialize(it, end);
    }
    for (auto& hist : light_histograms) {
        it = hist.deserialize(it, end);
    }

    // Copy charge channel mean/stddev data
    std::copy(it, it + NUM_CHARGE_CHANNELS, channel_mean.begin());
    it += NUM_CHARGE_CHANNELS;
    std::copy(it, it + NUM_CHARGE_CHANNELS, channel_stddev.begin());
    it += NUM_CHARGE_CHANNELS;

    return it;
}

#ifdef USE_PYTHON
py::dict TpcMonitor::getMetricDict() {

    py::dict metric_dict;
    py::list charge_hist_list;
    py::list light_hist_list;

    for (auto& hist : charge_histograms) {
        charge_hist_list.append(hist.getMetricDict());
    }
    for (auto& hist : light_histograms) {
        light_hist_list.append(hist.getMetricDict());
    }

    metric_dict["charge_hists"] = charge_hist_list;
    metric_dict["light_hists"] = light_hist_list;

    metric_dict["charge_channel_mean"] = vector_to_numpy_array_1d(channel_mean);
    metric_dict["charge_channel_stddev"] = vector_to_numpy_array_1d(channel_stddev);

    return metric_dict;
}
#endif

void TpcMonitor::print() const {
    std::cout << "++++++++++++++++ TpcMonitor ++++++++++++++++" << std::endl;
    std::cout << "Showing first charge histogram:" << std::endl;
    if (!charge_histograms.empty()) {
        charge_histograms.at(0).print();
    }
    std::cout << "Showing first light histogram:" << std::endl;
    if (!light_histograms.empty()) {
        light_histograms.at(0).print();
    }
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}