//
// Created by Jon Sensenig on 10/07/25.
//

#ifndef TPC_READOUT_MONITOR_H
#define TPC_READOUT_MONITOR_H

#include "metric_base.h"

using namespace constants::tpc_readout;

class TpcReadoutMonitor : public MetricBase {
private:
    int32_t error_bit_word_;
    int32_t readout_state_;
    int32_t last_command_;
    int32_t last_command_status_;

    int32_t num_events_upper_;
    int32_t num_events_lower_;

    int32_t event_diff_upper_;
    int32_t event_diff_lower_;

    int32_t num_dma_loops_upper_;
    int32_t num_dma_loops_lower_;

    int32_t received_mbytes_upper_;
    int32_t received_mbytes_lower_;

    int32_t avg_event_size_upper_;
    int32_t avg_event_size_lower_;

    int32_t num_files_upper_;
    int32_t num_files_lower_;

    std::array<int32_t, NUM_BOARDS> board_status_;

    // // Assign errors to the bits in the error word
    // enum ErrorBits : uint32_t {
    //     daq_state_cmd = 1,
    //     pcie_license = 1,
    //     config_load = 2,
    //     comms_config_load = 3,
    //     // Do not change these
    //     pcie_lib_init = 4,
    //     pcie_card_open = 5,
    //     pcie_control_buff = 6,
    //
    //     xmit_get_config = 7,
    // };

    enum class State : int {
        kIdle = 0,
        kConfigured = 1,
        kRunning = 2,
        kStopped = 3
      };

    // Implement  the serialize/deserialize
    size_t num_members_ = 14;
    auto member_tuple() {
        return std::tie(error_bit_word_, readout_state_, last_command_, last_command_status_,
                      num_events_upper_, num_events_lower_, event_diff_upper_,
                      event_diff_lower_, num_dma_loops_upper_, num_dma_loops_lower_, received_mbytes_upper_,
                      received_mbytes_lower_, avg_event_size_upper_, avg_event_size_lower_, num_files_upper_,
                      num_files_lower_);
    };
    auto member_tuple() const {
        return std::tie(error_bit_word_, readout_state_, last_command_, last_command_status_,
                        num_events_upper_, num_events_lower_, event_diff_upper_,
                        event_diff_lower_, num_dma_loops_upper_, num_dma_loops_lower_, received_mbytes_upper_,
                        received_mbytes_lower_, avg_event_size_upper_, avg_event_size_lower_, num_files_upper_,
                        num_files_lower_);
    };

    static inline int32_t getUpper32(size_t word) { return (word >> 32) & UINT32_MAX; }
    static inline int32_t getLower32(size_t word) { return word & UINT32_MAX; }
    static inline size_t getFullWord(size_t upper_word, size_t lower_word) { return (upper_word << 32) + lower_word; }

public:
    TpcReadoutMonitor();

    // Assign errors to the bits in the error word
    enum ErrorBits : uint32_t {
        daq_state_cmd = 1,
        pcie_license = 1,
        config_load = 2,
        comms_config_load = 3,
        // Do not change these
        pcie_lib_init = 4,
        pcie_card_open = 5,
        pcie_control_buff = 6,
        xmit_get_config = 7,
        lightfem_get_config = 8,
        chargefem_get_config = 9,
        trigger_get_config = 9,
        datahandler_get_config = 10,
    };

    // Helper to set the error word bits
    inline void setErrorBitWord(uint32_t set_bit) { error_bit_word_ |= (0x1 << set_bit); }
    inline static uint32_t getErrorBitWord(uint32_t error_word, uint32_t set_bit) { return error_word & (0x1 << set_bit); }

    // Helper struct to allow easy access to the error bits
    struct ErrorWord {
        uint32_t daq_state_cmd;
        ErrorWord(uint32_t word) {
            daq_state_cmd = TpcReadoutMonitor::getErrorBitWord(word, ErrorBits::daq_state_cmd);
        }
    };

    void clear();
    void print() const;

    // Public setters for populating data
    void setReadoutState(int32_t state) { readout_state_ = state; };
    void setLastCommand(int32_t command) { last_command_ = command; };
    void setLastCommandStatus(int32_t command_status) { last_command_status_ = command_status; };
    void setNumEvents(size_t num_events) {
      num_events_upper_ = getUpper32(num_events);
      num_events_lower_ = getLower32(num_events);
    };
    void setEventDiff(size_t event_diff) {
        event_diff_upper_ = getUpper32(event_diff);
        event_diff_lower_ = getLower32(event_diff);
    };
    void setNumDmaLoops(size_t dma_loops) {
        num_dma_loops_upper_ = getUpper32(dma_loops);
        num_dma_loops_lower_ = getLower32(dma_loops);
    };
     void setReceivedMbytes(size_t received_mbytes) {
       received_mbytes_upper_ = getUpper32(received_mbytes);
       received_mbytes_lower_ = getLower32(received_mbytes);
     }
     void setAvgEventSize(size_t avg_event_size) {
       avg_event_size_upper_ = getUpper32(avg_event_size);
       avg_event_size_lower_ = getLower32(avg_event_size);
     }
     void setNumFiles(size_t num_files) {
       num_files_upper_ = getUpper32(num_files);
       num_files_lower_ = getLower32(num_files);
     }
     void setBoardStatus(std::vector<int32_t> &board_status) {
         for (size_t i = 0; i < board_status.size(); i++) {
             board_status_.at(i) = board_status.at(i);
         }
     }

    // --- Getter Methods ---
    int32_t getReadoutState() const { return readout_state_; }
    int32_t getLastCommand() const { return last_command_; }
    int32_t getLastCommandStatus() const { return last_command_status_; }
    size_t getNumEvents() const { return getFullWord(num_events_upper_, num_events_lower_); }
    size_t getEventDiff() const { return getFullWord(num_events_upper_, num_events_lower_); }
    size_t getNumDmaLoops() const { return getFullWord(num_dma_loops_upper_, num_dma_loops_lower_); }
    size_t getReceivedMbytes() const { return getFullWord(received_mbytes_upper_, received_mbytes_lower_); }
    size_t getAvgEventSize() const { return getFullWord(avg_event_size_upper_, avg_event_size_lower_); }
    size_t getNumFiles() const { return getFullWord(num_files_upper_, num_files_lower_); }
    const std::array<int32_t, NUM_BOARDS> getBoardStatus() const { return board_status_; }

    // MetricBase interface implementation
    std::vector<int32_t> serialize() const override;
    std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                     std::vector<int32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};

#endif //TPC_READOUT_MONITOR_H
