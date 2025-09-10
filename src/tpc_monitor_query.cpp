//
// Created by Jon Sensenig on 9/10/25.
//
#include "tpc_monitor_query.h"


TpcMonitorQuery::TpcMonitorQuery() : fem_slots_{}
 {}
// TpcMonitorQuery::~TpcMonitorQuery();

void TpcMonitorQuery::clear() {
    std::fill(fem_slots_.begin(), fem_slots_.end(), 0);
}

std::vector<int32_t> TpcMonitorQuery::serialize() const {
    std::vector<int32_t> serialized_data;
    // Reserve space for efficiency
    size_t num_words = NUM_FEMS;
    serialized_data.reserve(num_words);

    // Serialize the histogram metadata
    // auto data = Serializer<LowBwTpcMonitor>::serialize_tuple(member_tuple());
    // serialized_data.insert(serialized_data.end(), data.begin(), data.end());
    // Fill the Fem slots
    serialized_data.insert(serialized_data.end(), fem_slots_.begin(), fem_slots_.end());
    return serialized_data;
}

std::vector<int32_t>::const_iterator TpcMonitorQuery::deserialize(std::vector<int32_t>::const_iterator begin,
                                                            std::vector<int32_t>::const_iterator end) {

    auto it = begin;
    // it = Serializer<LowBwTpcMonitor>::deserialize_tuple(member_tuple(), begin, end);

    // Ensure there's enough data for the bins
    if (std::distance(it, end) < NUM_FEMS) {
        throw std::runtime_error("Deserialization failed: not enough data for Number charge channels.");
    }

    // Copy num charge samples data
    std::copy(it, it + NUM_FEMS, fem_slots_.begin());
    it += NUM_FEMS;

    return it;
}

#ifdef USE_PYTHON
py::dict TpcMonitorQuery::getMetricDict() {

    py::dict metric_dict;
    metric_dict["fem_slots"] = array_to_numpy_array_1d(fem_slots_);

    return metric_dict;
}
#endif
