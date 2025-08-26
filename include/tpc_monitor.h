//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef TPC_MONITOR_H
#define TPC_MONITOR_H

#include "metric_base.h"
#include "histogram.h"

class TpcMonitor : public MetricBase {
private:
    std::vector<Histogram> charge_histograms;
    std::vector<Histogram> light_histograms;

public:
    TpcMonitor();

    void clear();
    void print() const;
    const std::vector<Histogram>& getChargeHistograms() const { return charge_histograms; }
    const std::vector<Histogram>& getLightHistograms() const { return light_histograms; }
    const void fillChargeChannelHistogram(size_t channel, int32_t word) { charge_histograms.at(channel).fill(word); };
    const void fillLightChannelHistogram(size_t channel, int32_t word) { light_histograms.at(channel).fill(word); };

    // MetricBase interface implementation
    std::vector<int32_t> serialize() const override;
    std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                     std::vector<int32_t>::const_iterator end) override;
#ifdef USE_PYTHON
    py::dict getMetricDict() override;
#endif

};

#endif //TPC_MONITOR_H
