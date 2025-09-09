//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef TPC_MONITOR_LBW_H
#define TPC_MONITOR_LBW_H

#include "metric_base.h"

class LowBwTpcMonitor : public MetricBase {
private:
    int32_t num_fems;
    int32_t num_charge_channels;
    int32_t num_light_channels;
    std::vector<int32_t> charge_channel_num_samples;

    // Implement  the serialize/deserialize
    size_t num_members_ = 3;
    auto member_tuple() {
        return std::tie(num_fems, num_charge_channels, num_light_channels);
    };
    auto member_tuple() const {
        return std::tie(num_fems, num_charge_channels, num_light_channels);
    };

public:
    LowBwTpcMonitor();

    void clear();
    void print() const;

    // Public setters for populating data
    void setNumFems(int32_t count) { num_fems = count; }
    void setNumChargeChannels(int32_t count) { num_charge_channels = count; }
    void setNumLightChannels(int32_t count) { num_light_channels = count; }
    void setChargeChannelSamples(size_t channel, int32_t samples) {
        if (channel < charge_channel_num_samples.size()) {
            charge_channel_num_samples.at(channel) = samples;
        }
    }

    // --- Getter Methods ---
    int32_t getNumFems() const { return num_fems; }
    int32_t getNumChargeChannels() const { return num_charge_channels; }
    int32_t getNumLightChannels() const { return num_light_channels; }
    const std::vector<int32_t>& getChargeChannelNumSamples() const { return charge_channel_num_samples; }

    // MetricBase interface implementation
    std::vector<int32_t> serialize() const override;
    std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                     std::vector<int32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};

#endif //TPC_MONITOR_LBW_H
