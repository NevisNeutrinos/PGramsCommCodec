//
// Created by Jon Sensenig on 12/9/25.
//

#include "tpc_monitor_charge_event.h"

TpcMonitorChargeEvent::TpcMonitorChargeEvent() : channel_number_(0), run_number_(0), file_number_(0),
                                                 evt_number_(0), num_samples_(0) {
    // Don't know the size of the light samples yet so we can't initialize it
}

void TpcMonitorChargeEvent::clear() {
    channel_number_ = 0;
    run_number_ = 0;
    file_number_ = 0;
    evt_number_ = 0;
    num_samples_ = 0;
    // Repeated calls will be more efficient if we preserve the vector size. Hence, the choice of fill() over clear()
    std::fill(charge_samples_.begin(), charge_samples_.end(), 0);
}

std::vector<uint32_t> TpcMonitorChargeEvent::serialize() const {
    std::vector<uint32_t> serialized_data;
    // Reserve space for efficiency
    serialized_data.reserve(num_members_);
    auto data = Serializer<TpcMonitorChargeEvent>::serialize_tuple(member_tuple());
    serialized_data.insert(serialized_data.end(), data.begin(), data.end());

    // Charge channel number of samples data
    serialized_data.insert(serialized_data.end(), charge_samples_.begin(), charge_samples_.end());

    return serialized_data;
}

std::vector<uint32_t>::const_iterator TpcMonitorChargeEvent::deserialize(std::vector<uint32_t>::const_iterator begin,
                                                            std::vector<uint32_t>::const_iterator end) {
    auto it = begin;
    it = Serializer<TpcMonitorChargeEvent>::deserialize_tuple(member_tuple(), begin, end);

    // Ensure there's enough data for the bins
    if (static_cast<size_t>(std::distance(it, end)) < num_samples_) {
        throw std::runtime_error("Deserialization failed: not enough data for num_samples");
    }

    // Copy num charge samples data
    charge_samples_.resize(num_samples_);
    std::copy(it, it + num_samples_, charge_samples_.begin());
    it += num_samples_;

    return it;
}

#ifdef USE_PYTHON
py::dict TpcMonitorChargeEvent::getMetricDict() {

    py::dict metric_dict;
    metric_dict["channel_number"] = channel_number_;
    metric_dict["run_number"] = run_number_;
    metric_dict["file_number"] = file_number_;
    metric_dict["evt_number"] = evt_number_;
    metric_dict["charge_samples"] = vector_to_numpy_array_1d(UnPackDoubleWords(charge_samples_));

    return metric_dict;
}
#endif

void TpcMonitorChargeEvent::print() {
    std::cout << "++++++++++++ TpcMonitorChargeEvent +++++++++++++" << std::endl;
    std::cout << "  Channel: " << channel_number_ << std::endl;
    std::cout << "  run_number: " << run_number_ << std::endl;
    std::cout << "  file_number: " << file_number_ << std::endl;
    std::cout << "  evt_number: " << evt_number_ << std::endl;
    std::cout << "  Number of samples: " << num_samples_ << std::endl;
    std::cout << "  Charge Samples (first 10): ";
    auto tmp_vec = UnPackDoubleWords(charge_samples_);
    for (size_t i = 0; i < 10; ++i) {
        std::cout << tmp_vec[i] << (i == tmp_vec.size() - 1 ? "" : ", ");
    }
    std::cout << "..." << std::endl;
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}