//
// Created by Jon Sensenig on 9/4/25.
//

#include "../include/tpc_configs.h"
#include <stdexcept>
#include <iostream>
#include <algorithm> // for std::fill, std::copy

TpcConfigs::TpcConfigs()
    : summed_peak_thresh_(0),
      channel_multiplicity_(0),
      roi_delay_0_(0),
      roi_delay_1_(0),
      roi_precount_(0),
      roi_peak_window_(0),
      enable_top_(0x0),
      enable_middle_(0x0),
      enable_bottom_(0xFFFF),
      disc_threshold_0_(NUM_CHARGE_CHANNELS, 100),
      disc_threshold_1_(NUM_CHARGE_CHANNELS, 250),
      num_roi_words_(30),
      roi_deadtime_(240),
      disc_threshold_3_(NUM_CHARGE_CHANNELS, 0xFFF),
      disc_threshold_4_(NUM_CHARGE_CHANNELS, 0xFFF),
      pmt_gate_size_(750),
      pmt_beam_size_(200),
      fifo_blocksize_(0xFFFF)
{}

void TpcConfigs::clear() {

    summed_peak_thresh_ = 0;
    channel_multiplicity_ = 0;
    roi_delay_0_ = 0;
    roi_delay_1_ = 0;
    roi_precount_ = 0;
    roi_peak_window_ = 0;
    enable_top_ = 0x0;
    enable_middle_ = 0x0;
    enable_bottom_ = 0x0;
    std::fill(disc_threshold_0_.begin(), disc_threshold_0_.end(), 0);
    std::fill(disc_threshold_1_.begin(), disc_threshold_1_.end(), 0);
    num_roi_words_ = 0;
    roi_deadtime_ = 0;
    std::fill(disc_threshold_3_.begin(), disc_threshold_3_.end(), 0);
    std::fill(disc_threshold_4_.begin(), disc_threshold_4_.end(), 0);
    pmt_gate_size_ = 0;
    pmt_beam_size_ = 0;
    fifo_blocksize_ = 0;
}

std::vector<int32_t> TpcConfigs::serialize() const {
    std::vector<int32_t> serialized_data;

    // Reserve space for all scalar fields + all vectors
    serialized_data.reserve(
        14 +
        disc_threshold_0_.size() + disc_threshold_1_.size() +
        disc_threshold_3_.size() + disc_threshold_4_.size()
    );

    // Scalars
    serialized_data.push_back(summed_peak_thresh_);
    serialized_data.push_back(channel_multiplicity_);
    serialized_data.push_back(roi_delay_0_);
    serialized_data.push_back(roi_delay_1_);
    serialized_data.push_back(roi_precount_);
    serialized_data.push_back(roi_peak_window_);
    serialized_data.push_back(static_cast<int32_t>(enable_top_));
    serialized_data.push_back(static_cast<int32_t>(enable_middle_));
    serialized_data.push_back(static_cast<int32_t>(enable_bottom_));
    serialized_data.push_back(num_roi_words_);
    serialized_data.push_back(roi_deadtime_);
    serialized_data.push_back(pmt_gate_size_);
    serialized_data.push_back(pmt_beam_size_);
    serialized_data.push_back(static_cast<int32_t>(fifo_blocksize_));

    // Vectors
    serialized_data.insert(serialized_data.end(), disc_threshold_0_.begin(), disc_threshold_0_.end());
    serialized_data.insert(serialized_data.end(), disc_threshold_1_.begin(), disc_threshold_1_.end());
    serialized_data.insert(serialized_data.end(), disc_threshold_3_.begin(), disc_threshold_3_.end());
    serialized_data.insert(serialized_data.end(), disc_threshold_4_.begin(), disc_threshold_4_.end());

    return serialized_data;
}

std::vector<int32_t>::const_iterator TpcConfigs::deserialize(std::vector<int32_t>::const_iterator begin,
                                                             std::vector<int32_t>::const_iterator end) {
    auto it = begin;

    // Need 14 scalars first
    if (std::distance(it, end) < 14) {
        throw std::runtime_error("Deserialization failed: not enough data for TpcConfigs metadata.");
    }

    summed_peak_thresh_ = *it++;
    channel_multiplicity_ = *it++;
    roi_delay_0_ = *it++;
    roi_delay_1_ = *it++;
    roi_precount_ = *it++;
    roi_peak_window_ = *it++;
    enable_top_ = static_cast<uint32_t>(*it++);
    enable_middle_ = static_cast<uint32_t>(*it++);
    enable_bottom_ = static_cast<uint32_t>(*it++);
    num_roi_words_ = *it++;
    roi_deadtime_ = *it++;
    pmt_gate_size_ = *it++;
    pmt_beam_size_ = *it++;
    fifo_blocksize_ = static_cast<uint32_t>(*it++);

    // Vectors
    auto copy_vector = [&](std::vector<int32_t>& v) {
        if (std::distance(it, end) < static_cast<long>(v.size())) {
            throw std::runtime_error("Deserialization failed: not enough data for TpcConfigs vector.");
        }
        std::copy(it, it + v.size(), v.begin());
        it += v.size();
    };

    copy_vector(disc_threshold_0_);
    copy_vector(disc_threshold_1_);
    copy_vector(disc_threshold_3_);
    copy_vector(disc_threshold_4_);

    return it;
}

#ifdef USE_PYTHON
py::dict TpcConfigs::getMetricDict() {
    py::dict metric_dict;

    metric_dict["summed_peak_thresh"] = summed_peak_thresh_;
    metric_dict["channel_multiplicity"] = channel_multiplicity_;
    metric_dict["roi_delay_0"] = roi_delay_0_;
    metric_dict["roi_delay_1"] = roi_delay_1_;
    metric_dict["roi_precount"] = roi_precount_;
    metric_dict["roi_peak_window"] = roi_peak_window_;
    metric_dict["enable_top"] = enable_top_;
    metric_dict["enable_middle"] = enable_middle_;
    metric_dict["enable_bottom"] = enable_bottom_;
    metric_dict["num_roi_words"] = num_roi_words_;
    metric_dict["roi_deadtime"] = roi_deadtime_;
    metric_dict["pmt_gate_size"] = pmt_gate_size_;
    metric_dict["pmt_beam_size"] = pmt_beam_size_;
    metric_dict["fifo_blocksize"] = fifo_blocksize_;

    metric_dict["disc_threshold_0"] = vector_to_numpy_array_1d(disc_threshold_0_);
    metric_dict["disc_threshold_1"] = vector_to_numpy_array_1d(disc_threshold_1_);
    metric_dict["disc_threshold_3"] = vector_to_numpy_array_1d(disc_threshold_3_);
    metric_dict["disc_threshold_4"] = vector_to_numpy_array_1d(disc_threshold_4_);

    return metric_dict;
}
#endif

void TpcConfigs::print() const {
    std::cout << "++++++++++++ TpcConfigs +++++++++++++" << std::endl;
    std::cout << "  summed_peak_thresh: " << summed_peak_thresh_ << std::endl;
    std::cout << "  channel_multiplicity: " << channel_multiplicity_ << std::endl;
    std::cout << "  roi_delay_0: " << roi_delay_0_ << std::endl;
    std::cout << "  roi_delay_1: " << roi_delay_1_ << std::endl;
    std::cout << "  roi_precount: " << roi_precount_ << std::endl;
    std::cout << "  roi_peak_window: " << roi_peak_window_ << std::endl;
    std::cout << "  enable_top: " << enable_top_ << std::endl;
    std::cout << "  enable_middle: " << enable_middle_ << std::endl;
    std::cout << "  enable_bottom: " << enable_bottom_ << std::endl;
    std::cout << "  num_roi_words: " << num_roi_words_ << std::endl;
    std::cout << "  roi_deadtime: " << roi_deadtime_ << std::endl;
    std::cout << "  pmt_gate_size: " << pmt_gate_size_ << std::endl;
    std::cout << "  pmt_beam_size: " << pmt_beam_size_ << std::endl;
    std::cout << "  fifo_blocksize: " << fifo_blocksize_ << std::endl;

    auto print_vector = [](const std::string& name, const std::vector<int32_t>& v) {
        std::cout << "  " << name << " (first 10): ";
        for (size_t i = 0; i < 10 && i < v.size(); ++i) {
            std::cout << v[i] << (i == 9 || i == v.size() - 1 ? "" : ", ");
        }
        std::cout << "..." << std::endl;
    };

    print_vector("disc_threshold_0", disc_threshold_0_);
    print_vector("disc_threshold_1", disc_threshold_1_);
    print_vector("disc_threshold_3", disc_threshold_3_);
    print_vector("disc_threshold_4", disc_threshold_4_);

    std::cout << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}
