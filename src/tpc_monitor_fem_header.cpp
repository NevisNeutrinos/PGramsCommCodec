#include "tpc_monitor_fem_header.h"

void TpcMonitorFemHeader::clear() {
    slot_number_ = 0;
    run_number_ = 0;
    file_number_ = 0;
    evt_number_ = 0;
    event_id_ = 0;
    frame_id_ = 0;
    trigger_frame_ = 0;
    trigger_sample_ = 0;
}

std::vector<uint32_t> TpcMonitorFemHeader::serialize() const {
    return Serializer<TpcMonitorFemHeader>::serialize_tuple(member_tuple());
}

std::vector<uint32_t>::const_iterator TpcMonitorFemHeader::deserialize(
    std::vector<uint32_t>::const_iterator begin, std::vector<uint32_t>::const_iterator end) {
    return Serializer<TpcMonitorFemHeader>::deserialize_tuple(member_tuple(), begin, end);
}

#ifdef USE_PYTHON
py::dict TpcMonitorFemHeader::getMetricDict() {
    py::dict metric_dict;
    metric_dict["slot_number"] = slot_number_;
    metric_dict["run_number"] = run_number_;
    metric_dict["file_number"] = file_number_;
    metric_dict["evt_number"] = evt_number_;
    metric_dict["event_id"] = event_id_;
    metric_dict["frame_id"] = frame_id_;
    metric_dict["trigger_frame"] = trigger_frame_;
    metric_dict["trigger_sample"] = trigger_sample_;
    return metric_dict;
}
#endif

void TpcMonitorFemHeader::print() {
    std::cout << "TpcMonitorFemHeader slot=" << slot_number_ << " run=" << run_number_
              << " file=" << file_number_ << " evt=" << evt_number_ << " e#=" << event_id_
              << " f#=" << frame_id_ << " t#=" << trigger_frame_ << " s#=" << trigger_sample_
              << std::endl;
}
