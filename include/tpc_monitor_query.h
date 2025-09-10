//
// Created by Jon Sensenig on 9/10/25.
//

#ifndef TPC_MONITOR_QUERY_H
#define TPC_MONITOR_QUERY_H

#include "metric_base.h"

using namespace constants::tpc_readout; // localize the relevant constants scope

class TpcMonitorQuery : public MetricBase {
private:

    // std::array<uint32_t, NUM_CHARGE_CHANNELS>
    std::array<int32_t, NUM_FEMS> fem_slots_;

public:

    // Default constructor for deserialization purposes
    TpcMonitorQuery();

    // Public API
    void clear();
    void print() const;

    // --- Getter Methods ---
    const std::array<int32_t, NUM_FEMS>& getFemSlots() const { return fem_slots_; }

    // MetricBase interface implementation
    std::vector<int32_t> serialize() const override;
    std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                     std::vector<int32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};

#endif //TPC_MONITOR_QUERY_H
