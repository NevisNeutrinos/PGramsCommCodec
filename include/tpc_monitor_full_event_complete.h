//
// End marker for full-event telemetry stream (0x4005).
//

#ifndef TPC_MONITOR_FULL_EVENT_COMPLETE_H
#define TPC_MONITOR_FULL_EVENT_COMPLETE_H

#include "metric_base.h"

class TpcMonitorFullEventComplete : public MetricBase {

public:
    enum Status : uint32_t {
        kOk = 0,
        kFileNotFound = 1,
        kEventNotFound = 2,
        kLlagEventNotFound = 3,
        // Exact L_lag match failed; payload still sent using the closest lag.
        kLlagUsedClosest = 4,
    };

private:
    uint32_t run_number_ = 0;
    uint32_t file_number_ = 0;
    uint32_t evt_number_ = 0;
    uint32_t l_lag_ = 0;
    uint32_t num_fem_headers_ = 0;
    uint32_t num_charge_packets_ = 0;
    uint32_t num_light_packets_ = 0;
    uint32_t status_code_ = 0;
    // ProcessEvents::EventErrorBit word for this event (0 if none / not decoded).
    uint32_t event_error_bit_word_ = 0;

    size_t num_members_ = 9;
    auto member_tuple() {
        return std::tie(run_number_, file_number_, evt_number_, l_lag_, num_fem_headers_,
                        num_charge_packets_, num_light_packets_, status_code_, event_error_bit_word_);
    }
    auto member_tuple() const {
        return std::tie(run_number_, file_number_, evt_number_, l_lag_, num_fem_headers_,
                        num_charge_packets_, num_light_packets_, status_code_, event_error_bit_word_);
    }

public:
    TpcMonitorFullEventComplete() = default;
    void clear();
    void print();

    void setRunNumber(uint32_t run) { run_number_ = run; }
    void setFileNumber(uint32_t file) { file_number_ = file; }
    void setEvtNumber(uint32_t evt) { evt_number_ = evt; }
    void setLLag(uint32_t l_lag) { l_lag_ = l_lag; }
    void setNumFemHeaders(uint32_t n) { num_fem_headers_ = n; }
    void setNumChargePackets(uint32_t n) { num_charge_packets_ = n; }
    void setNumLightPackets(uint32_t n) { num_light_packets_ = n; }
    void setStatusCode(uint32_t status) { status_code_ = status; }
    void setEventErrorBitWord(uint32_t word) { event_error_bit_word_ = word; }

    uint32_t getRunNumber() const { return run_number_; }
    uint32_t getFileNumber() const { return file_number_; }
    uint32_t getEvtNumber() const { return evt_number_; }
    uint32_t getLLag() const { return l_lag_; }
    uint32_t getNumFemHeaders() const { return num_fem_headers_; }
    uint32_t getNumChargePackets() const { return num_charge_packets_; }
    uint32_t getNumLightPackets() const { return num_light_packets_; }
    uint32_t getStatusCode() const { return status_code_; }
    uint32_t getEventErrorBitWord() const { return event_error_bit_word_; }

    std::vector<uint32_t> serialize() const override;
    std::vector<uint32_t>::const_iterator deserialize(std::vector<uint32_t>::const_iterator begin,
                                                      std::vector<uint32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif
};

#endif // TPC_MONITOR_FULL_EVENT_COMPLETE_H
