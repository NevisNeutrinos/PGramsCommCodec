//
// Created by Jon Sensenig on 8/23/25.
//

#include "../include/tpc_monitor_lbw.h"
#include <stdexcept>
#include <iostream>

LowBwTpcMonitor::LowBwTpcMonitor() : num_fems(0), num_charge_channels(0), num_light_channels(0) {
    charge_channel_num_samples.resize(NUM_CHARGE_CHANNELS, 0);
}

void LowBwTpcMonitor::clear() {
    num_fems = 0;
    num_charge_channels = 0;
    num_light_channels = 0;
    std::fill(charge_channel_num_samples.begin(), charge_channel_num_samples.end(), 0);
}

std::vector<int32_t> LowBwTpcMonitor::serialize() const {
    std::vector<int32_t> serialized_data;
    // Reserve space for efficiency
    serialized_data.reserve(num_members_ + NUM_CHARGE_CHANNELS);

    // Serialize the histogram metadata
    auto data = Serializer<LowBwTpcMonitor>::serialize_tuple(member_tuple());
    serialized_data.insert(serialized_data.end(), data.begin(), data.end());
    // Charge channel number of samples data
    serialized_data.insert(serialized_data.end(), charge_channel_num_samples.begin(), charge_channel_num_samples.end());
    return serialized_data;
}

std::vector<int32_t>::const_iterator LowBwTpcMonitor::deserialize(std::vector<int32_t>::const_iterator begin,
                                                            std::vector<int32_t>::const_iterator end) {

    auto it = begin;
    it = Serializer<LowBwTpcMonitor>::deserialize_tuple(member_tuple(), begin, end);

    // Ensure there's enough data for the bins
    if (std::distance(it, end) < NUM_CHARGE_CHANNELS) {
        throw std::runtime_error("Deserialization failed: not enough data for Number charge channels.");
    }

    // Copy num charge samples data
    std::copy(it, it + NUM_CHARGE_CHANNELS, charge_channel_num_samples.begin());
    it += NUM_CHARGE_CHANNELS;

    return it;
}

#ifdef USE_PYTHON
py::dict LowBwTpcMonitor::getMetricDict() {

    py::dict metric_dict;
    metric_dict["num_fems"] = num_fems;
    metric_dict["num_charge_channels"] = num_charge_channels;
    metric_dict["num_light_channels"] = num_light_channels;
    metric_dict["charge_channel_num_samples"] = vector_to_numpy_array_1d(charge_channel_num_samples);

    return metric_dict;
}
#endif

void LowBwTpcMonitor::print() const {
    std::cout << "++++++++++++ LowBwTpcMonitor +++++++++++++" << std::endl;
    std::cout << "  num_fems: " << num_fems << std::endl;
    std::cout << "  num_charge_channels: " << num_charge_channels << std::endl;
    std::cout << "  num_light_channels: " << num_light_channels << std::endl;
    std::cout << "  Charge Channel Samples (first 10): ";
    for (size_t i = 0; i < 10 && i < charge_channel_num_samples.size(); ++i) {
        std::cout << charge_channel_num_samples[i] << (i == 9 || i == charge_channel_num_samples.size() - 1 ? "" : ", ");
    }
    std::cout << "..." << std::endl;
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}