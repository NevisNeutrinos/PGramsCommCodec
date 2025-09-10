//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef TPC_MONITOR_LBW_H
#define TPC_MONITOR_LBW_H

#include "metric_base.h"

using namespace constants::tpc_readout;

class LowBwTpcMonitor : public MetricBase {
private:
    int32_t num_fems;
    int32_t num_charge_channels;
    int32_t num_light_channels;
    int32_t charge_channel_balance;
    int32_t light_channel_balance;
    int32_t error_bit_word;
    std::vector<int32_t> charge_channel_num_samples;

    // Assign errors to the bits in the error word
    enum ErrorBits : uint32_t {
        checksum = 1,
        fem_count = 2,
        charge_samples = 3,
        light_samples = 4
    };

    // Implement  the serialize/deserialize
    size_t num_members_ = 5;
    auto member_tuple() {
        return std::tie(num_fems, num_charge_channels, num_light_channels, charge_channel_balance,
            light_channel_balance, error_bit_word);
    };
    auto member_tuple() const {
        return std::tie(num_fems, num_charge_channels, num_light_channels, charge_channel_balance,
            light_channel_balance, error_bit_word);
    };

public:
    LowBwTpcMonitor();

    // Helper to set the error word bits
    inline void setErrorBitWord(uint32_t set_bit) { error_bit_word |= (0x1 << set_bit); }
    inline static uint32_t getErrorBitWord(uint32_t error_word, uint32_t set_bit) { return error_word & (0x1 << set_bit); }

    // Helper struct to allow easy access to the error bits
    struct ErrorWord {
        uint32_t checksum;
        uint32_t fem_count;
        uint32_t charge_samples;
        uint32_t light_samples;
        ErrorWord(uint32_t word) {
            checksum = LowBwTpcMonitor::getErrorBitWord(word, ErrorBits::checksum);
            fem_count = LowBwTpcMonitor::getErrorBitWord(word, ErrorBits::fem_count);
            charge_samples = LowBwTpcMonitor::getErrorBitWord(word, ErrorBits::charge_samples);
            light_samples = LowBwTpcMonitor::getErrorBitWord(word, ErrorBits::light_samples);
        }
    };

    void clear();
    void print() const;

    // Public setters for populating data
    void setNumFems(int32_t count) { num_fems = count; }
    void setNumChargeChannels(int32_t count) { num_charge_channels = count; }
    void setNumLightChannels(int32_t count) { num_light_channels = count; }
    void setChargeChannelBalance(int32_t count) { charge_channel_balance = count; }
    void setLightChannelBalance(int32_t count) { light_channel_balance = count; }

    // Setters for error word bits
    void setChecksumError() { setErrorBitWord(ErrorBits::checksum); }
    void setFemCountError() { setErrorBitWord(ErrorBits::fem_count); }
    void setChargeSampleError() { setErrorBitWord(ErrorBits::charge_samples); }
    void setLightSampleError() { setErrorBitWord(ErrorBits::light_samples); }

    void setChargeChannelSamples(size_t channel, int32_t samples) {
        if (channel < charge_channel_num_samples.size()) {
            charge_channel_num_samples.at(channel) = samples;
        }
    }

    // --- Getter Methods ---
    int32_t getNumFems() const { return num_fems; }
    int32_t getNumChargeChannels() const { return num_charge_channels; }
    int32_t getNumLightChannels() const { return num_light_channels; }
    int32_t getChargeChannelBalance() const { return charge_channel_balance; }
    int32_t getLightChannelBalance() const { return light_channel_balance; }
    int32_t getErrorBitWord() const { return error_bit_word; }
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
