//
// Created by Jon Sensenig on 10/01/25.
//

#ifndef DAQ_COMP_MONITOR_H
#define DAQ_COMP_MONITOR_H

#include "metric_base.h"

using namespace constants::daq_computer;

class DaqCompMonitor : public MetricBase {
private:
    int32_t daq_bit_word_;
    int32_t tpc_disk_;
    int32_t tof_disk_;
    int32_t sys_disk_;
    int32_t cpu_usage_;
    int32_t memory_usage_;
    int32_t disk_temp_;
    std::array<int32_t, NUM_CPUS> cpu_temp_;

    // Each bit indicates if a DAQ is running or not
    enum DaqRunningBits : uint32_t {
        tpc = 0,
        tpc_monitor = 1,
        tof = 2,
        tof_monitor = 3
    };

    // Implement  the serialize/deserialize
    size_t num_members_ = 7;
    auto member_tuple() {
        return std::tie(daq_bit_word_, tpc_disk_, tof_disk_, sys_disk_, cpu_usage_, memory_usage_, disk_temp_);
    };
    auto member_tuple() const {
        return std::tie(daq_bit_word_, tpc_disk_, tof_disk_, sys_disk_, cpu_usage_, memory_usage_, disk_temp_);
    };

public:
    DaqCompMonitor();

    // Helper to set the error word bits
    inline void setDaqBitWord(uint32_t set_bit) { daq_bit_word_ |= (0x1 << set_bit); }
    inline void unsetDaqBitWord(uint32_t unset_bit) { daq_bit_word_ &= ~(0x1 << unset_bit); }
    inline static uint32_t getDaqBitWord(uint32_t daq_word, uint32_t set_bit) { return daq_word & (0x1 << set_bit); }

    // Helper struct to allow easy access to the error bits
    struct DaqWord {
        uint32_t tpc;
        uint32_t tpc_monitor;
        uint32_t tof;
        uint32_t tof_monitor;
        DaqWord(uint32_t word) {
            tpc = DaqCompMonitor::getDaqBitWord(word, DaqRunningBits::tpc);
            tpc_monitor = DaqCompMonitor::getDaqBitWord(word, DaqRunningBits::tpc_monitor);
            tof = DaqCompMonitor::getDaqBitWord(word, DaqRunningBits::tof);
            tof_monitor = DaqCompMonitor::getDaqBitWord(word, DaqRunningBits::tof_monitor);
        }
    };

    void clear();
    void print() const;

    // Public setters for populating data
    void setTpcDisk(int32_t disk) { tpc_disk_ = disk; }
    void setTofDisk(int32_t disk) { tof_disk_ = disk; }
    void setSysDisk(int32_t disk) { sys_disk_ = disk; }
    void setCpuUsage(int32_t usage) { cpu_usage_ = usage; }
    void setMemoryUsage(int32_t usage) { memory_usage_ = usage; }
    void setDiskTemp(int32_t temp) { disk_temp_ = temp; }
    void setCpuTemp(std::array<int32_t, NUM_CPUS> temp_vec) { cpu_temp_ = std::move(temp_vec); }

    // Setters for error word bits
    void setTpcDaq() { setDaqBitWord(DaqRunningBits::tpc); }
    void setTpcMonitorDaq() { setDaqBitWord(DaqRunningBits::tpc_monitor); }
    void setTofDaq() { setDaqBitWord(DaqRunningBits::tof); }
    void setTofMonitorDaq() { setDaqBitWord(DaqRunningBits::tof_monitor); }
    void unsetTpcDaq() { unsetDaqBitWord(DaqRunningBits::tpc); }
    void unsetTpcMonitorDaq() { unsetDaqBitWord(DaqRunningBits::tpc_monitor); }
    void unsetTofDaq() { unsetDaqBitWord(DaqRunningBits::tof); }
    void unsetTofMonitorDaq() { unsetDaqBitWord(DaqRunningBits::tof_monitor); }

    // --- Getter Methods ---
    // Public setters for populating data
    int32_t getFullDaqBitWord() const { return daq_bit_word_; }
    int32_t getTpcDisk() const { return tpc_disk_; }
    int32_t getTofDisk() const { return tof_disk_; }
    int32_t getSysDisk() const { return sys_disk_; }
    int32_t getCpuUsage() const { return cpu_usage_; }
    int32_t getMemoryUsage() const { return memory_usage_; }
    int32_t getDiskTemp() const { return disk_temp_; }
    const std::array<int32_t, NUM_CPUS> getCpuTemp() const { return cpu_temp_; }

    // MetricBase interface implementation
    std::vector<int32_t> serialize() const override;
    std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                     std::vector<int32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};

#endif //DAQ_COMP_MONITOR_H
