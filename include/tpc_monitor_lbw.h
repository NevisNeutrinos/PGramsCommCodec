//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef TPC_MONITOR_LBW_H
#define TPC_MONITOR_LBW_H

#include "metric_base.h"

using namespace constants::tpc_readout;

class LowBwTpcMonitor : public MetricBase {
private:
    // Metrics
    int32_t error_bit_word_;
    std::array<int32_t, DOUBLE_PACK_CHARGE_CH> charge_baselines_;
    std::array<int32_t, DOUBLE_PACK_CHARGE_CH> charge_rms_;
    std::array<int32_t, DOUBLE_PACK_CHARGE_CH> charge_avg_num_hits_;
    std::array<int32_t, DOUBLE_PACK_LIGHT_CH> light_baselines_;
    std::array<int32_t, DOUBLE_PACK_LIGHT_CH> light_rms_;
    std::array<int32_t, DOUBLE_PACK_LIGHT_CH> light_avg_num_rois_;

    // Assign errors to the bits in the error word
    enum ErrorBits : uint32_t {
        checksum = 1,
        fem_count = 2,
        charge_samples = 3,
        light_samples = 4
    };

    // Implement  the serialize/deserialize
    size_t num_members_ = 1;
    auto member_tuple() {
        return std::tie(error_bit_word_);
    };
    auto member_tuple() const {
        return std::tie(error_bit_word_);
    };

public:
    LowBwTpcMonitor();

    // Helper to set the error word bits
    inline void setErrorBitWord(uint32_t set_bit) { error_bit_word_ |= (0x1 << set_bit); }
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
    void print();

    template<size_t N, size_t M>
    void PackDoubleWords(std::array<int32_t, N> &source_array, std::array<int32_t, M> &dest_array) {
        // Packing two 16b per 32b words with i+1 in upper and i in the lower bits
        for (size_t i = 0; i < M; i++) {
            dest_array[i] = ((source_array[2 * i + 1]  & 0xFFFF) << 16) + (source_array[2 * i] & 0xFFFF);
        }
    }

    template<size_t M>
    std::vector<int32_t> UnPackDoubleWords(std::array<int32_t, M> &source_array) {
        // Unpacking two 16b per 32b words with i+1 in upper and i in the lower bits
        // std::array<int32_t, 2*M> dest_array;
        std::vector<int32_t> dest_array;
        dest_array.resize(2 * M);
        for (size_t i = 0; i < M; i++) {
            dest_array[2 * i] = source_array[i] & 0xFFFF;
            dest_array[2 * i + 1] = (source_array[i] >> 16) & 0xFFFF;
        }
        return dest_array;
    }

    template <typename T, size_t N>
    void print_array(std::array<T, N> &array) {
        for (size_t i = 0; i < array.size(); ++i) {
            std::cout << array[i] << (i == array.size() - 1 ? "" : ", ");
        }
        std::cout << "..." << std::endl;
    }

    template <typename T>
    void print_vec(std::vector<T> &vec) {
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << vec[i] << (i == vec.size() - 1 ? "" : ", ");
        }
        std::cout << "..." << std::endl;
    }

    // Public setters for populating data
    void setChargeBaselines(std::array<int32_t, NUM_CHARGE_CHANNELS> &baselines) {
        PackDoubleWords(baselines, charge_baselines_);
    }
    void setChargeRms(std::array<int32_t, NUM_CHARGE_CHANNELS> &rms) {
        PackDoubleWords(rms, charge_rms_);
    }
    void setAvgNumHits(std::array<int32_t, NUM_CHARGE_CHANNELS> &hits) {
        PackDoubleWords(hits, charge_avg_num_hits_);
    }
    void setLightBaselines(std::array<int32_t, NUM_LIGHT_CHANNELS> &baselines) {
        PackDoubleWords(baselines, light_baselines_);
    }
    void setLightRms(std::array<int32_t, NUM_LIGHT_CHANNELS> &rms) {
        PackDoubleWords(rms, light_rms_);
    }
    void setLightAvgNumRois(std::array<int32_t, NUM_LIGHT_CHANNELS> &rois) {
        PackDoubleWords(rois, light_avg_num_rois_);
    }

    // --- Getter Methods ---
    const std::array<int32_t, DOUBLE_PACK_CHARGE_CH>& getChargeBaselines() const { return charge_baselines_; }
    const std::array<int32_t, DOUBLE_PACK_CHARGE_CH>& getChargeRms() const { return charge_rms_; }
    const std::array<int32_t, DOUBLE_PACK_CHARGE_CH>& getAvgNumHits() const { return charge_avg_num_hits_; }

    const std::array<int32_t, DOUBLE_PACK_LIGHT_CH>& getLightBaselines() const { return light_baselines_; }
    const std::array<int32_t, DOUBLE_PACK_LIGHT_CH>& getLightRms() const { return light_rms_; }
    const std::array<int32_t, DOUBLE_PACK_LIGHT_CH>& getLightAvgNumRois() const { return light_avg_num_rois_; }

    // MetricBase interface implementation
    std::vector<int32_t> serialize() const override;
    std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                     std::vector<int32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};

#endif //TPC_MONITOR_LBW_H
