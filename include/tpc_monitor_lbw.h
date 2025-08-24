//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef TPC_MONITOR_LBW_H
#define TPC_MONITOR_LBW_H

#include "is_serializable.h"
#include "histogram.h"

class LowBwTpcMonitor : public ISerializable {
private:
    int32_t num_fems;
    int32_t num_charge_channels;
    int32_t num_light_channels;
    std::vector<int32_t> charge_channel_num_samples;

public:
    LowBwTpcMonitor();

    void clear();
    void print() const;

    // Public setters for populating data
    void setNumFems(int32_t count) { num_fems = count; }
    void setNumChargeChannels(int32_t count) { num_charge_channels = count; }
    void setNumLightChannels(int32_t count) { num_light_channels = count; }
    void setChargeChannelSamples(size_t index, int32_t samples) {
        if (index < charge_channel_num_samples.size()) {
            charge_channel_num_samples[index] = samples;
        }
    }

    // --- Getter Methods ---
    int32_t getNumFems() const { return num_fems; }
    int32_t getNumChargeChannels() const { return num_charge_channels; }
    int32_t getNumLightChannels() const { return num_light_channels; }
    const std::vector<int32_t>& getChargeChannelNumSamples() const { return charge_channel_num_samples; }

    // ISerializable interface implementation
    std::vector<int32_t> serialize() const override;
    std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                     std::vector<int32_t>::const_iterator end) override;
};

#endif //TPC_MONITOR_LBW_H
