//
// Per-FEM trigger/header fields for full-event telemetry (0x4004).
//

#ifndef TPC_MONITOR_FEM_HEADER_H
#define TPC_MONITOR_FEM_HEADER_H

#include "metric_base.h"

class TpcMonitorFemHeader : public MetricBase {

private:
    uint32_t slot_number_ = 0;
    uint32_t run_number_ = 0;
    uint32_t file_number_ = 0;
    uint32_t evt_number_ = 0;
    uint32_t event_id_ = 0;       // FEMHeader3 e#
    uint32_t frame_id_ = 0;       // FEMHeader4 f#
    uint32_t trigger_frame_ = 0;  // FEMHeader6 t# (4-bit field)
    uint32_t trigger_sample_ = 0; // FEMHeader6 s#

    size_t num_members_ = 8;
    auto member_tuple() {
        return std::tie(slot_number_, run_number_, file_number_, evt_number_, event_id_, frame_id_,
                        trigger_frame_, trigger_sample_);
    }
    auto member_tuple() const {
        return std::tie(slot_number_, run_number_, file_number_, evt_number_, event_id_, frame_id_,
                        trigger_frame_, trigger_sample_);
    }

public:
    TpcMonitorFemHeader() = default;
    void clear();
    void print();

    void setSlotNumber(uint32_t slot) { slot_number_ = slot; }
    void setRunNumber(uint32_t run) { run_number_ = run; }
    void setFileNumber(uint32_t file) { file_number_ = file; }
    void setEvtNumber(uint32_t evt) { evt_number_ = evt; }
    void setEventId(uint32_t event_id) { event_id_ = event_id; }
    void setFrameId(uint32_t frame_id) { frame_id_ = frame_id; }
    void setTriggerFrame(uint32_t trigger_frame) { trigger_frame_ = trigger_frame; }
    void setTriggerSample(uint32_t trigger_sample) { trigger_sample_ = trigger_sample; }

    uint32_t getSlotNumber() const { return slot_number_; }
    uint32_t getRunNumber() const { return run_number_; }
    uint32_t getFileNumber() const { return file_number_; }
    uint32_t getEvtNumber() const { return evt_number_; }
    uint32_t getEventId() const { return event_id_; }
    uint32_t getFrameId() const { return frame_id_; }
    uint32_t getTriggerFrame() const { return trigger_frame_; }
    uint32_t getTriggerSample() const { return trigger_sample_; }

    std::vector<uint32_t> serialize() const override;
    std::vector<uint32_t>::const_iterator deserialize(std::vector<uint32_t>::const_iterator begin,
                                                      std::vector<uint32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif
};

#endif // TPC_MONITOR_FEM_HEADER_H
