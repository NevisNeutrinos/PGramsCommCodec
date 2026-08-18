//
// Created by Jon Sensenig on 8/23/25.
//

#include "../include/tpc_monitor_lbw.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

LowBwTpcMonitor::LowBwTpcMonitor() : error_bit_word_(0), run_number_(0), file_number_(0), evt_number_(0),
    packet_status_word_(0), n_error_events_(0), emit_error_bit_counts_(true), has_error_bit_counts_(false) {
    std::fill(error_bit_counts_.begin(), error_bit_counts_.end(), 0);
    std::fill(charge_baselines_.begin(), charge_baselines_.end(), 0);
    std::fill(charge_rms_.begin(), charge_rms_.end(), 0);
    std::fill(charge_avg_num_hits_.begin(), charge_avg_num_hits_.end(), 0);
    std::fill(light_baselines_.begin(), light_baselines_.end(), 0);
    std::fill(light_rms_.begin(), light_rms_.end(), 0);
    std::fill(light_avg_num_rois_.begin(), light_avg_num_rois_.end(), 0 );
}

void LowBwTpcMonitor::clear() {
    error_bit_word_ = 0;
    run_number_ = 0;
    file_number_ = 0;
    evt_number_ = 0;
    packet_status_word_ = 0;
    n_error_events_ = 0;
    has_error_bit_counts_ = false;
    std::fill(error_bit_counts_.begin(), error_bit_counts_.end(), 0);
    std::fill(charge_baselines_.begin(), charge_baselines_.end(), 0);
    std::fill(charge_rms_.begin(), charge_rms_.end(), 0);
    std::fill(charge_avg_num_hits_.begin(), charge_avg_num_hits_.end(), 0);
    std::fill(light_baselines_.begin(), light_baselines_.end(), 0);
    std::fill(light_rms_.begin(), light_rms_.end(), 0);
    std::fill(light_avg_num_rois_.begin(), light_avg_num_rois_.end(), 0 );
}

void LowBwTpcMonitor::addErrorBitCounts(uint32_t word) {
    error_bit_word_ |= word;
    n_error_events_++;
    if (word == 0) {
        return;
    }
    for (uint32_t b = 0; b < NUM_EVENT_ERROR_BITS; ++b) {
        if (word & (1u << b)) {
            error_bit_counts_[b]++;
        }
    }
}

std::vector<uint32_t> LowBwTpcMonitor::serialize() const {
    std::vector<uint32_t> serialized_data;
    serialized_data.reserve(num_members_ + 3 * DOUBLE_PACK_CHARGE_CH + 3 * DOUBLE_PACK_LIGHT_CH
                            + 2 + DOUBLE_PACK_ERROR_BITS);

    auto data = Serializer<LowBwTpcMonitor>::serialize_tuple(member_tuple());
    serialized_data.insert(serialized_data.end(), data.begin(), data.end());

    serialized_data.insert(serialized_data.end(), charge_baselines_.begin(), charge_baselines_.end());
    serialized_data.insert(serialized_data.end(), charge_rms_.begin(), charge_rms_.end());
    serialized_data.insert(serialized_data.end(), charge_avg_num_hits_.begin(), charge_avg_num_hits_.end());
    serialized_data.insert(serialized_data.end(), light_baselines_.begin(), light_baselines_.end());
    serialized_data.insert(serialized_data.end(), light_rms_.begin(), light_rms_.end());
    serialized_data.insert(serialized_data.end(), light_avg_num_rois_.begin(), light_avg_num_rois_.end());

    if (emit_error_bit_counts_) {
        serialized_data.push_back(packet_status_word_);
        serialized_data.push_back(n_error_events_);
        std::array<uint32_t, NUM_EVENT_ERROR_BITS> capped = error_bit_counts_;
        for (uint32_t& c : capped) {
            if (c > 0xFFFFu) {
                c = 0xFFFFu;
            }
        }
        std::array<uint32_t, DOUBLE_PACK_ERROR_BITS> packed_counts{};
        PackDoubleWords(capped, packed_counts);
        serialized_data.insert(serialized_data.end(), packed_counts.begin(), packed_counts.end());
    }

    return serialized_data;
}

std::vector<uint32_t>::const_iterator LowBwTpcMonitor::deserialize(std::vector<uint32_t>::const_iterator begin,
                                                            std::vector<uint32_t>::const_iterator end) {

    auto it = begin;
    it = Serializer<LowBwTpcMonitor>::deserialize_tuple(member_tuple(), begin, end);

    if (static_cast<size_t>(std::distance(it, end)) < (3*DOUBLE_PACK_CHARGE_CH + 3*DOUBLE_PACK_LIGHT_CH)) {
        throw std::runtime_error("Deserialization failed: not enough data for 3*NUM_CHARGE_CH + 3*NUM_LIGHT_CH.");
    }

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

    packet_status_word_ = 0;
    n_error_events_ = 0;
    std::fill(error_bit_counts_.begin(), error_bit_counts_.end(), 0);
    has_error_bit_counts_ = false;
    // 0x4001 is a positional uint32 array (no field names). Extra words after
    // the ADC arrays are `error_bit_words`. Absent extra words => old packet.
    const auto remaining = static_cast<size_t>(std::distance(it, end));
    if (remaining >= 2 + DOUBLE_PACK_ERROR_BITS) {
        has_error_bit_counts_ = true;
        packet_status_word_ = *it++;
        n_error_events_ = *it++;
        std::array<uint32_t, DOUBLE_PACK_ERROR_BITS> packed_counts{};
        std::copy(it, it + DOUBLE_PACK_ERROR_BITS, packed_counts.begin());
        it += DOUBLE_PACK_ERROR_BITS;
        auto unpacked = UnPackDoubleWords(packed_counts);
        for (size_t i = 0; i < NUM_EVENT_ERROR_BITS && i < unpacked.size(); ++i) {
            error_bit_counts_[i] = unpacked[i];
        }
    }

    return it;
}

#ifdef USE_PYTHON
py::dict LowBwTpcMonitor::getMetricDict() {

    py::dict metric_dict;
    metric_dict["error_bit_word"] = error_bit_word_;
    metric_dict["run_number"] = run_number_;
    metric_dict["file_number"] = file_number_;
    metric_dict["evt_number"] = evt_number_;
    if (hasErrorBitCounts()) {
        metric_dict["packet_status_word"] = packet_status_word_;
        metric_dict["n_error_events"] = n_error_events_;
        metric_dict["error_bit_words"] = array_to_numpy_array_1d(error_bit_counts_);
    }
    metric_dict["charge_baseline"] = vector_to_numpy_array_1d(UnPackDoubleWords(charge_baselines_));
    metric_dict["charge_rms"] = vector_to_numpy_array_1d(UnPackDoubleWords(charge_rms_));
    metric_dict["charge_avg_num_hits"] = vector_to_numpy_array_1d(UnPackDoubleWords(charge_avg_num_hits_));
    metric_dict["light_baseline"] = vector_to_numpy_array_1d(UnPackDoubleWords(light_baselines_));
    metric_dict["light_rms"] = vector_to_numpy_array_1d(UnPackDoubleWords(light_rms_));
    metric_dict["light_avg_num_hits"] = vector_to_numpy_array_1d(UnPackDoubleWords(light_avg_num_rois_));

    return metric_dict;
}
#endif

void LowBwTpcMonitor::print() {
    std::cout << "++++++++++++ LowBwTpcMonitor +++++++++++++" << std::endl;
    std::cout << "  error_bit_word: " << error_bit_word_ << std::endl;
    std::cout << "  packet_status_word: " << packet_status_word_ << std::endl;
    std::cout << "  n_error_events: " << n_error_events_ << std::endl;
    std::cout << "  error_bit_words: ";
    print_array(error_bit_counts_);
    std::cout << "  run_number: " << run_number_ << std::endl;
    std::cout << "  file_number: " << file_number_ << std::endl;
    std::cout << "  evt_number: " << evt_number_ << std::endl;
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
