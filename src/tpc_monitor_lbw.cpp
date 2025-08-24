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
    serialized_data.reserve(3 + charge_channel_num_samples.size());

    // Metadata
    serialized_data.push_back(num_fems);
    serialized_data.push_back(num_charge_channels);
    serialized_data.push_back(num_light_channels);

    // Data
    serialized_data.insert(serialized_data.end(),
                           charge_channel_num_samples.begin(),
                           charge_channel_num_samples.end());
    return serialized_data;
}

std::vector<int32_t>::const_iterator LowBwTpcMonitor::deserialize(std::vector<int32_t>::const_iterator begin,
                                                                  std::vector<int32_t>::const_iterator end) {
    auto it = begin;

    // Ensure there's enough data for the metadata fields
    if (std::distance(it, end) < 3) {
        throw std::runtime_error("Deserialization failed: not enough data for LowBwTpcMonitor metadata.");
    }
    num_fems = *it++;
    num_charge_channels = *it++;
    num_light_channels = *it++;

    // Ensure there's enough data for the samples vector
    if (std::distance(it, end) < static_cast<long>(charge_channel_num_samples.size())) {
        throw std::runtime_error("Deserialization failed: not enough data for LowBwTpcMonitor samples.");
    }
    std::copy(it, it + charge_channel_num_samples.size(), charge_channel_num_samples.begin());
    it += charge_channel_num_samples.size();

    return it;
}

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