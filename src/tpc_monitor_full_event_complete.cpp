#include "tpc_monitor_full_event_complete.h"

void TpcMonitorFullEventComplete::clear() {
    run_number_ = 0;
    file_number_ = 0;
    evt_number_ = 0;
    l_lag_ = 0;
    num_fem_headers_ = 0;
    num_charge_packets_ = 0;
    num_light_packets_ = 0;
    status_code_ = 0;
}

std::vector<uint32_t> TpcMonitorFullEventComplete::serialize() const {
    return Serializer<TpcMonitorFullEventComplete>::serialize_tuple(member_tuple());
}

std::vector<uint32_t>::const_iterator TpcMonitorFullEventComplete::deserialize(
    std::vector<uint32_t>::const_iterator begin, std::vector<uint32_t>::const_iterator end) {
    return Serializer<TpcMonitorFullEventComplete>::deserialize_tuple(member_tuple(), begin, end);
}

#ifdef USE_PYTHON
py::dict TpcMonitorFullEventComplete::getMetricDict() {
    py::dict metric_dict;
    metric_dict["run_number"] = run_number_;
    metric_dict["file_number"] = file_number_;
    metric_dict["evt_number"] = evt_number_;
    metric_dict["l_lag"] = l_lag_;
    metric_dict["num_fem_headers"] = num_fem_headers_;
    metric_dict["num_charge_packets"] = num_charge_packets_;
    metric_dict["num_light_packets"] = num_light_packets_;
    metric_dict["status_code"] = status_code_;
    return metric_dict;
}
#endif

void TpcMonitorFullEventComplete::print() {
    std::cout << "TpcMonitorFullEventComplete run=" << run_number_ << " file=" << file_number_
              << " evt=" << evt_number_ << " l_lag=" << l_lag_
              << " fem=" << num_fem_headers_ << " charge=" << num_charge_packets_
              << " light=" << num_light_packets_ << " status=" << status_code_ << std::endl;
}
