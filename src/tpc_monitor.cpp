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
    return it;
}

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