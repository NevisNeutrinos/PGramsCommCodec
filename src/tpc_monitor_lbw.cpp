//
// Created by Jon Sensenig on 8/23/25.
//

#include "../include/tpc_monitor_lbw.h"
#include <stdexcept>
#include <iostream>

LowBwTpcMonitor::LowBwTpcMonitor() : error_bit_word_(0) {
    std::fill(charge_baselines_.begin(), charge_baselines_.end(), 0);
    std::fill(charge_rms_.begin(), charge_rms_.end(), 0);
    std::fill(charge_avg_num_hits_.begin(), charge_avg_num_hits_.end(), 0);
    std::fill(light_baselines_.begin(), light_baselines_.end(), 0);
    std::fill(light_rms_.begin(), light_rms_.end(), 0);
    std::fill(light_avg_num_rois_.begin(), light_avg_num_rois_.end(), 0 );
}

void LowBwTpcMonitor::clear() {
    std::fill(charge_baselines_.begin(), charge_baselines_.end(), 0);
    std::fill(charge_rms_.begin(), charge_rms_.end(), 0);
    std::fill(charge_avg_num_hits_.begin(), charge_avg_num_hits_.end(), 0);
    std::fill(light_baselines_.begin(), light_baselines_.end(), 0);
    std::fill(light_rms_.begin(), light_rms_.end(), 0);
    std::fill(light_avg_num_rois_.begin(), light_avg_num_rois_.end(), 0 );
}

std::vector<int32_t> LowBwTpcMonitor::serialize() const {
    std::vector<int32_t> serialized_data;
    // Reserve space for efficiency
    serialized_data.reserve(num_members_);

    // Serialize the histogram metadata
    auto data = Serializer<LowBwTpcMonitor>::serialize_tuple(member_tuple());
    serialized_data.insert(serialized_data.end(), data.begin(), data.end());

    // Charge channel number of samples data
    serialized_data.insert(serialized_data.end(), charge_baselines_.begin(), charge_baselines_.end());
    serialized_data.insert(serialized_data.end(), charge_rms_.begin(), charge_rms_.end());
    serialized_data.insert(serialized_data.end(), charge_avg_num_hits_.begin(), charge_avg_num_hits_.end());
    serialized_data.insert(serialized_data.end(), light_baselines_.begin(), light_baselines_.end());
    serialized_data.insert(serialized_data.end(), light_rms_.begin(), light_rms_.end());
    serialized_data.insert(serialized_data.end(), light_avg_num_rois_.begin(), light_avg_num_rois_.end());

    return serialized_data;
}

std::vector<int32_t>::const_iterator LowBwTpcMonitor::deserialize(std::vector<int32_t>::const_iterator begin,
                                                            std::vector<int32_t>::const_iterator end) {

    auto it = begin;
    it = Serializer<LowBwTpcMonitor>::deserialize_tuple(member_tuple(), begin, end);

    // Ensure there's enough data for the bins
    if (static_cast<size_t>(std::distance(it, end)) < (3*DOUBLE_PACK_CHARGE_CH + 3*DOUBLE_PACK_LIGHT_CH)) {
        throw std::runtime_error("Deserialization failed: not enough data for 3*NUM_CHARGE_CH + 3*NUM_LIGHT_CH.");
    }

    // Copy num charge samples data
    std::copy(it, it + DOUBLE_PACK_CHARGE_CH, charge_baselines_.begin());
    it += DOUBLE_PACK_CHARGE_CH;
    std::copy(it, it + DOUBLE_PACK_CHARGE_CH, charge_rms_.begin());
    it += DOUBLE_PACK_CHARGE_CH;
    std::copy(it, it + DOUBLE_PACK_CHARGE_CH, charge_avg_num_hits_.begin());
    it += DOUBLE_PACK_CHARGE_CH;
    std::copy(it, it+DOUBLE_PACK_LIGHT_CH, light_baselines_.begin());
    it += DOUBLE_PACK_LIGHT_CH;
    std::copy(it, it + DOUBLE_PACK_LIGHT_CH, light_rms_.begin());
    it += DOUBLE_PACK_LIGHT_CH;
    std::copy(it, it + DOUBLE_PACK_LIGHT_CH, light_avg_num_rois_.begin());
    it += DOUBLE_PACK_LIGHT_CH;

    return it;
}

#ifdef USE_PYTHON
py::dict LowBwTpcMonitor::getMetricDict() {

    py::dict metric_dict;
    metric_dict["error_bit_word"] = error_bit_word_;
    // Charge channel metrics
    metric_dict["charge_baseline"] = vector_to_numpy_array_1d(UnPackDoubleWords(charge_baselines_));
    metric_dict["charge_rms"] = vector_to_numpy_array_1d(UnPackDoubleWords(charge_rms_));
    metric_dict["charge_avg_num_hits"] = vector_to_numpy_array_1d(UnPackDoubleWords(charge_avg_num_hits_));
    // Light channel metrics
    metric_dict["light_baseline"] = vector_to_numpy_array_1d(UnPackDoubleWords(light_baselines_));
    metric_dict["light_rms"] = vector_to_numpy_array_1d(UnPackDoubleWords(light_rms_));
    metric_dict["light_avg_num_hits"] = vector_to_numpy_array_1d(UnPackDoubleWords(light_avg_num_rois_));

    return metric_dict;
}
#endif

void LowBwTpcMonitor::print() {
    std::cout << "++++++++++++ LowBwTpcMonitor +++++++++++++" << std::endl;
    std::cout << "  error_bit_word: " << error_bit_word_ << std::endl;
    std::cout << "  Charge Baselines (first 10): ";
    auto tmp_vec = UnPackDoubleWords(charge_baselines_);
    print_vec(tmp_vec);
    std::cout << "  Charge RMS (first 10): ";
    tmp_vec = UnPackDoubleWords(charge_rms_);
    print_vec(tmp_vec);
    std::cout << "  Charge Avg Num Hits (first 10): ";
    tmp_vec = UnPackDoubleWords(charge_avg_num_hits_);
    print_vec(tmp_vec);
    std::cout << "  Light Baselines (first 10): ";
    tmp_vec = UnPackDoubleWords(light_baselines_);
    print_vec(tmp_vec);
    std::cout << "  Light RMS (first 10): ";
    tmp_vec = UnPackDoubleWords(light_rms_);
    print_vec(tmp_vec);
    std::cout << "  Light Avg Num ROIs (first 10): ";
    tmp_vec = UnPackDoubleWords(light_avg_num_rois_);
    print_vec(tmp_vec);
    std::cout << "..." << std::endl;
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}