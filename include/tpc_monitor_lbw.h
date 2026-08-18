//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef TPC_MONITOR_LBW_H
#define TPC_MONITOR_LBW_H

#include "metric_base.h"
#include <array>

using namespace constants::tpc_readout;

class LowBwTpcMonitor : public MetricBase {
private:
    // Metrics
    uint32_t error_bit_word_;  // OR of EventErrorBit across decoded events
    uint32_t run_number_;
    uint32_t file_number_;
    uint32_t evt_number_;
    // Optional `error_bit_words` after packed ADC arrays (LBW_ERROR_BIT_COUNTS):
    // packet_status, n_error_events, then 16 packed uint16 counts.
    uint32_t packet_status_word_;
    uint32_t n_error_events_;
    std::array<uint32_t, NUM_EVENT_ERROR_BITS> error_bit_counts_;
    bool emit_error_bit_counts_ = true;
    bool has_error_bit_counts_ = false;
    std::array<uint32_t, DOUBLE_PACK_CHARGE_CH> charge_baselines_;
    std::array<uint32_t, DOUBLE_PACK_CHARGE_CH> charge_rms_;
    std::array<uint32_t, DOUBLE_PACK_CHARGE_CH> charge_avg_num_hits_;
    std::array<uint32_t, DOUBLE_PACK_LIGHT_CH> light_baselines_;
    std::array<uint32_t, DOUBLE_PACK_LIGHT_CH> light_rms_;
    std::array<uint32_t, DOUBLE_PACK_LIGHT_CH> light_avg_num_rois_;

    // Implement  the serialize/deserialize
    size_t num_members_ = 4;
    auto member_tuple() {
        return std::tie(error_bit_word_, run_number_, file_number_, evt_number_);
    };
    auto member_tuple() const {
        return std::tie(error_bit_word_, run_number_, file_number_, evt_number_);
    };

public:
    // Query/packet-level flags (not EventErrorBit). Bit 0 of packet_status_word_.
    enum PacketStatusBits : uint32_t {
        file_not_closed = 0,
    };

    LowBwTpcMonitor();

    // error_bit_word_ uses ProcessEvents::EventErrorBit numbering
    // (bit i <=> enumerator value i+1). Do not mix PacketStatusBits into it.
    inline void orErrorBitWord(uint32_t word) { error_bit_word_ |= word; }
    inline void setPacketStatusBit(PacketStatusBits bit) {
        packet_status_word_ |= (0x1u << static_cast<uint32_t>(bit));
    }
    inline static uint32_t getErrorBitWord(uint32_t error_word, uint32_t set_bit) {
        return error_word & (0x1u << set_bit);
    }
    // Count every decoded event (including stride-skipped ones). word==0 still
    // increments n_error_events_ so ground can turn counts into fractions.
    void addErrorBitCounts(uint32_t word);
    // When false, serialize() matches the pre-count 0x4001 layout (no trailer).
    void setEmitErrorBitCounts(bool emit) { emit_error_bit_counts_ = emit; }
    bool getEmitErrorBitCounts() const { return emit_error_bit_counts_; }
    bool hasErrorBitCounts() const { return has_error_bit_counts_ || emit_error_bit_counts_; }
    // Old 0x4001 stub used error_bit_word bit 5 for file-not-closed.
    void setLegacyFileNotClosedBit() { error_bit_word_ |= (1u << 5); }

    void clear();
    void print();

    template<size_t N, size_t M>
    void PackDoubleWords(std::array<uint32_t, N> &source_array, std::array<uint32_t, M> &dest_array) const {
        // Packing two 16b per 32b words with i+1 in upper and i in the lower bits
        for (size_t i = 0; i < M; i++) {
            dest_array[i] = ((source_array[2 * i + 1]  & 0xFFFF) << 16) + (source_array[2 * i] & 0xFFFF);
        }
    }

    template<size_t M>
    std::vector<uint32_t> UnPackDoubleWords(std::array<uint32_t, M> &source_array) const {
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
    void setRunNumber(uint32_t run_number) { run_number_ = run_number; }
    void setFileNumber(uint32_t file_number) { file_number_ = file_number; }
    void setEvtNumber(uint32_t evt_number) { evt_number_ = evt_number; }
    void setPacketStatusWord(uint32_t word) { packet_status_word_ = word; }
    void setNErrorEvents(uint32_t n) { n_error_events_ = n; }
    void setChargeBaselines(std::array<uint32_t, NUM_CHARGE_CHANNELS> &baselines) {
        // Packed uint16: baseline×10, RMS×100, hits×10 (see LBW_*_SCALE).
        PackDoubleWords(baselines, charge_baselines_);
    }
    void setChargeRms(std::array<uint32_t, NUM_CHARGE_CHANNELS> &rms) {
        PackDoubleWords(rms, charge_rms_);
    }
    void setAvgNumHits(std::array<uint32_t, NUM_CHARGE_CHANNELS> &hits) {
        PackDoubleWords(hits, charge_avg_num_hits_);
    }
    void setLightBaselines(std::array<uint32_t, NUM_LIGHT_CHANNELS> &baselines) {
        PackDoubleWords(baselines, light_baselines_);
    }
    void setLightRms(std::array<uint32_t, NUM_LIGHT_CHANNELS> &rms) {
        PackDoubleWords(rms, light_rms_);
    }
    void setLightAvgNumRois(std::array<uint32_t, NUM_LIGHT_CHANNELS> &rois) {
        PackDoubleWords(rois, light_avg_num_rois_);
    }

    // --- Getter Methods ---
    const uint32_t getErrorBitWord() const { return error_bit_word_; }
    const uint32_t getRunNumber() const { return run_number_; }
    const uint32_t getFileNumber() const { return file_number_; }
    const uint32_t getEvtNumber() const { return evt_number_; }
    const uint32_t getPacketStatusWord() const { return packet_status_word_; }
    const uint32_t getNErrorEvents() const { return n_error_events_; }
    const std::array<uint32_t, NUM_EVENT_ERROR_BITS>& getErrorBitCounts() const {
        return error_bit_counts_;
    }
    const std::array<uint32_t, DOUBLE_PACK_CHARGE_CH>& getChargeBaselines() const { return charge_baselines_; }
    const std::array<uint32_t, DOUBLE_PACK_CHARGE_CH>& getChargeRms() const { return charge_rms_; }
    const std::array<uint32_t, DOUBLE_PACK_CHARGE_CH>& getAvgNumHits() const { return charge_avg_num_hits_; }

    const std::array<uint32_t, DOUBLE_PACK_LIGHT_CH>& getLightBaselines() const { return light_baselines_; }
    const std::array<uint32_t, DOUBLE_PACK_LIGHT_CH>& getLightRms() const { return light_rms_; }
    const std::array<uint32_t, DOUBLE_PACK_LIGHT_CH>& getLightAvgNumRois() const { return light_avg_num_rois_; }

    // MetricBase interface implementation
    std::vector<uint32_t> serialize() const override;
    std::vector<uint32_t>::const_iterator deserialize(std::vector<uint32_t>::const_iterator begin,
                                                     std::vector<uint32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};

#endif //TPC_MONITOR_LBW_H
