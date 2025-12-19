//
// Created by Jon Sensenig on 12/9/25.
//

#include "tpc_monitor_light_event.h"

TpcMonitorLightEvent::TpcMonitorLightEvent() : channel_number_(0) {
    std::fill(light_samples_.begin(), light_samples_.end(), 0);
}

void TpcMonitorLightEvent::clear() {
    channel_number_ = 0;
    std::fill(light_samples_.begin(), light_samples_.end(), 0);
}

std::vector<uint32_t> TpcMonitorLightEvent::serialize() const {
    std::vector<uint32_t> serialized_data;
    // Reserve space for efficiency
    serialized_data.reserve(num_members_);
    auto data = Serializer<TpcMonitorLightEvent>::serialize_tuple(member_tuple());
    serialized_data.insert(serialized_data.end(), data.begin(), data.end());

    // Light channel number of samples data
    serialized_data.insert(serialized_data.end(), light_samples_.begin(), light_samples_.end());

    return serialized_data;
}

std::vector<uint32_t>::const_iterator TpcMonitorLightEvent::deserialize(std::vector<uint32_t>::const_iterator begin,
                                                            std::vector<uint32_t>::const_iterator end) {
    auto it = begin;
    it = Serializer<TpcMonitorLightEvent>::deserialize_tuple(member_tuple(), begin, end);

    // Ensure there's enough data for the bins
    if (static_cast<size_t>(std::distance(it, end)) < light_samples_.size()) {
        throw std::runtime_error("Deserialization failed: not enough data for ONE_FRAME_CHARGE_SAMPLES");
    }

    // Copy num light samples data
    std::copy(it, it + light_samples_.size(), light_samples_.begin());
    it += light_samples_.size();

    return it;
}

#ifdef USE_PYTHON
py::dict TpcMonitorLightEvent::getMetricDict() {

    py::dict metric_dict;
    metric_dict["channel_number"] = channel_number_;
    metric_dict["light_samples"] = vector_to_numpy_array_1d(UnPackDoubleWords(light_samples_));

    return metric_dict;
}
#endif

void TpcMonitorLightEvent::print() {
    std::cout << "++++++++++++ TpcMonitorLightEvent +++++++++++++" << std::endl;
    std::cout << "  Channel: " << channel_number_ << std::endl;
    std::cout << "  Light Samples (first 10): ";
    auto tmp_vec = UnPackDoubleWords(light_samples_);
    for (size_t i = 0; i < 10; ++i) {
        std::cout << tmp_vec[i] << (i == tmp_vec.size() - 1 ? "" : ", ");
    }
    std::cout << "..." << std::endl;
    std::cout << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
}