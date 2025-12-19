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
    std::array<uint32_t, DOUBLE_PACK_CHARGE_ONE_FRAME> charge_samples_{};
    // std::array<uint32_t, DOUBLE_PACK_LIGHT_SAMPLE> light_samples_;

    // Implement  the serialize/deserialize
    size_t num_members_ = 1;
    auto member_tuple() {
        return std::tie(channel_number_);
    };
    auto member_tuple() const {
        return std::tie(channel_number_);
    };

public:

    TpcMonitorChargeEvent();
    void clear();
    void print();

    template<size_t N, size_t M>
    void PackDoubleWords(std::array<uint32_t, N> &source_array, std::array<uint32_t, M> &dest_array) {
        // Packing two 16b per 32b words with i+1 in upper and i in the lower bits
        for (size_t i = 0; i < M; i++) {
            dest_array[i] = ((source_array[2 * i + 1]  & 0xFFFF) << 16) + (source_array[2 * i] & 0xFFFF);
        }
    }

    template<size_t M>
    std::vector<uint32_t> UnPackDoubleWords(std::array<uint32_t, M> &source_array) {
        // Unpacking two 16b per 32b words with i+1 in upper and i in the lower bits
        // std::array<uint32_t, 2*M> dest_array;
        std::vector<uint32_t> dest_array;
        dest_array.resize(2 * M);
        for (size_t i = 0; i < M; i++) {
            dest_array[2 * i] = source_array[i] & 0xFFFF;
            dest_array[2 * i + 1] = (source_array[i] >> 16) & 0xFFFF;
        }
        return dest_array;
    }

    void setChannelNumber(uint32_t channel_number) { channel_number_ = channel_number; }
    void setChargeSamples(std::array<uint32_t, CHARGE_ONE_FRAME> &charge_one_frame) {
        PackDoubleWords(charge_one_frame, charge_samples_);
    }

    // --- Getter Methods ---
    const uint32_t channel_number() const { return channel_number_; }
    const std::array<uint32_t, DOUBLE_PACK_CHARGE_ONE_FRAME>& getChargeSamples() const { return charge_samples_; }

    // MetricBase interface implementation
    std::vector<uint32_t> serialize() const override;
    std::vector<uint32_t>::const_iterator deserialize(std::vector<uint32_t>::const_iterator begin,
                                                     std::vector<uint32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};
#endif //TPC_MONITOR_CHARGE_EVENTS_H
