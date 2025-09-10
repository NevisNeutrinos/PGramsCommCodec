//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "metric_base.h"

using namespace constants::tpc_readout;

class Histogram : public MetricBase {
private:
    // Configuration
    int32_t min_value;
    int32_t max_value;
    int32_t num_bins;
    double bin_width;

    // Data storage
    std::vector<int32_t> bins;
    int32_t below_range_count;
    int32_t above_range_count;

    // Implement  the serialize/deserialize
    size_t num_members_ = 5;
    auto member_tuple() {
        return std::tie(min_value, max_value, num_bins, below_range_count, above_range_count);
    };
    auto member_tuple() const {
        return std::tie(min_value, max_value, num_bins, below_range_count, above_range_count);
    };

public:
    // Default constructor for deserialization purposes
    Histogram();
    // Parameterized constructor
    Histogram(int32_t min, int32_t max, int32_t bins_count);

    // Public API
    void fill(int32_t value);
    void clear();
    void print() const;

    // --- Getter Methods ---
    int32_t getMinValue() const { return min_value; }
    int32_t getMaxValue() const { return max_value; }
    int32_t getNumBins() const { return num_bins; }
    const std::vector<int32_t>& getBins() const { return bins; }
    int32_t getBelowRangeCount() const { return below_range_count; }
    int32_t getAboveRangeCount() const { return above_range_count; }

    // MetricBase interface implementation
    std::vector<int32_t> serialize() const override;
    std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                     std::vector<int32_t>::const_iterator end) override;

#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};

#endif //HISTOGRAM_H
