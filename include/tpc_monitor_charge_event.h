//
// Created by Jon Sensenig on 12/9/25.
//

#ifndef TPC_MONITOR_CHARGE_EVENTS_H
#define TPC_MONITOR_CHARGE_EVENT_H

#include "metric_base.h"

using namespace constants::tpc_readout;

class TpcMonitorChargeEvent : public MetricBase {

private:

    uint32_t channel_number_;
    uint32_t num_samples_;
    std::vector<uint32_t> charge_samples_{};

    // Implement  the serialize/deserialize
    size_t num_members_ = 2;
    auto member_tuple() {
        return std::tie(channel_number_, num_samples_);
    };
    auto member_tuple() const {
        return std::tie(channel_number_, num_samples_);
    };

public:

    TpcMonitorChargeEvent();
    void clear();
    void print();

    void PackDoubleWords(std::vector<uint32_t> &source_array, std::vector<uint32_t> &dest_array) {
        // Packing two 16b per 32b words with i+1 in upper and i in the lower bits
        for (size_t i = 0; i < source_array.size(); i++) {
            dest_array[i] = ((source_array[2 * i + 1]  & 0xFFFF) << 16) + (source_array[2 * i] & 0xFFFF);
        }
    }

    std::vector<uint32_t> UnPackDoubleWords(std::vector<uint32_t> &source_array) {
        // Unpacking two 16b per 32b words with i+1 in upper and i in the lower bits
        std::vector<uint32_t> dest_array;
        dest_array.resize(2 * source_array.size());
        for (size_t i = 0; i < source_array.size(); i++) {
            dest_array[2 * i] = source_array[i] & 0xFFFF;
            dest_array[2 * i + 1] = (source_array[i] >> 16) & 0xFFFF;
        }
        return dest_array;
    }

    void setChannelNumber(uint32_t channel_number) { channel_number_ = channel_number; }
    void setChargeSamples(std::vector<uint32_t> &charge_one_frame) {
        num_samples_ = (charge_one_frame.size() + 1) / 2;
        charge_samples_.resize(num_samples_);
        PackDoubleWords(charge_one_frame, charge_samples_);
    }

    // --- Getter Methods ---
    const uint32_t channel_number() const { return channel_number_; }
    const uint32_t getNumSamples() const { return num_samples_; }
    const std::vector<uint32_t>& getChargeSamples() const { return charge_samples_; }

    // MetricBase interface implementation
    std::vector<uint32_t> serialize() const override;
    std::vector<uint32_t>::const_iterator deserialize(std::vector<uint32_t>::const_iterator begin,
                                                     std::vector<uint32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};
#endif //TPC_MONITOR_CHARGE_EVENTS_H
