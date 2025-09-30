//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef TPC_CONFIGS_H
#define TPC_CONFIGS_H

#include "metric_base.h"
#include <vector>
#include <iostream>

using namespace constants::tpc_readout;

class TpcConfigs : public MetricBase {
private:

    /*
    * Note: We only use the Cosmic multiplicity and summed amplitude.
    *       This is the case since the other types require additional
    *       conditions such as a beam gate.
    */

    // Configuration parameters ot be set from the ground
    int32_t summed_peak_thresh_;    // Summed peak ampltitudes of 5 adjacent channels
    int32_t channel_multiplicity_;  // Number of 5 adjacent channels above Disc 1 threshold
    int32_t roi_delay_0_;           // Number of samples to shift the waveforms to perform the subtraction
    int32_t roi_delay_1_;           // ?
    int32_t roi_precount_;          // number of samples before disc. 0 threshold to include in the ROI
    int32_t roi_peak_window_;       // pmt_width, window where the waveform peak is found
    uint32_t enable_top_ = 0x0;            // Enable mask for channels on top connector
    uint32_t enable_middle_ = 0x0;         // Enable mask for channels on middle connector
    uint32_t enable_bottom_ = 0xFFFF;      // Enable mask for channels on bottom connector
    int32_t num_roi_words_ = 30;    // Number of samples in the ROI
    int32_t roi_deadtime_ = 240;    // The number of samples after the ROI which are an enforced deadtime
    uint32_t fifo_blocksize_ = 0xFFFF; // not sure if this will be configurable during flight

    // Trigger parameters
    int32_t trigger_source_;
    int32_t software_trigger_rate_hz_;
    int32_t tpc_dead_time_;
    std::array<int32_t, NUM_PRESCALES> prescale_;

    // Discriminator thresholds
    std::array<int32_t, NUM_LIGHT_CHANNELS> disc_threshold_0_; // Discriminator threshold 0 for the arming disc.
    std::array<int32_t, NUM_LIGHT_CHANNELS> disc_threshold_1_; // Discriminator threshold 1 to decide when to save the ROI

    // Won't use these
    int32_t pmt_gate_size_ = 750; // : 750,
    int32_t pmt_beam_size_ = 202; // : 202,
    int32_t beam_multiplicity_ = 100; // : 100,
    int32_t beam_summed_adc_thresh_ = 500; // : 500,
    int32_t michel_multiplicity_ = 100; // : 100,
    int32_t michel_summed_adc_thresh_ = 100; // : 500,
    std::array<int32_t, NUM_LIGHT_CHANNELS> disc_threshold_3_;
    std::array<int32_t, NUM_LIGHT_CHANNELS> disc_threshold_4_;


    // // Implement  the serialize/deserialize
    size_t num_members_ = 15;

    auto member_tuple() {
        return std::tie(summed_peak_thresh_, channel_multiplicity_,
        roi_delay_0_, roi_delay_1_, roi_precount_, roi_peak_window_,
        enable_top_, enable_middle_, enable_bottom_, num_roi_words_,
        roi_deadtime_, fifo_blocksize_, trigger_source_, software_trigger_rate_hz_,
        tpc_dead_time_);
    };

    auto member_tuple() const {
        return std::tie(summed_peak_thresh_, channel_multiplicity_,
        roi_delay_0_, roi_delay_1_, roi_precount_, roi_peak_window_,
        enable_top_, enable_middle_, enable_bottom_, num_roi_words_,
        roi_deadtime_, fifo_blocksize_, trigger_source_, software_trigger_rate_hz_,
        tpc_dead_time_);
    };

public:
    TpcConfigs();

    void clear();
    void print() const;

    // MetricBase interface implementation
    std::vector<int32_t> serialize() const override;
    std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                     std::vector<int32_t>::const_iterator end) override;
#ifdef USE_PYTHON
    py::dict getMetricDict() override;
    void setMetricDict(py::dict &config);

    // Config setter helper functions
    template<typename T>
    void AssignScalar(T &config_param, py::dict &config, std::string config_key) {
        std::cout << "Config/type " << config_key << "/" << typeid(T).name() << std::endl;
        if (!config.contains(py::str(config_key)) ) {
            throw std::runtime_error("Missing key [" + config_key + "]");
        }
        config_param = config[py::str(config_key)].cast<T>();
    }

    template<size_t N>
    void AssignVector(std::array<int32_t, N> &param_vec, py::dict &config, std::string config_key) {
        std::vector<int32_t> tmp_thresh;
        if (!config.contains(py::str(config_key)) ) {
            throw std::runtime_error("Missing key [" + config_key + "]");
        }
        py::object obj = config[py::str(config_key)];
        if (py::isinstance<py::array>(obj)) {
            // Handle numpy array explicitly
            py::array arr = py::cast<py::array>(obj);
            if (arr.ndim() != 1) {
                throw std::runtime_error("Expected but did not find, 1D array for " + config_key);
            }
            tmp_thresh.resize(arr.shape(0));
            std::memcpy(tmp_thresh.data(), arr.data(), arr.nbytes());
        } else {
            // Fall back: assume it's a Python list
            AssignScalar(tmp_thresh, config, config_key);
        }
        // Make sure we have the right number of thresholds
        if (tmp_thresh.size() != N) {
            throw std::runtime_error( "Incorrect number of " + config_key + " thresholds! Expected/Received " +
                                     std::to_string(N) + "/" + std::to_string(tmp_thresh.size()) );
        }
        for (size_t t = 0; t < tmp_thresh.size(); t++) { param_vec.at(t) = tmp_thresh.at(t); }
    }
#endif

    // ===== Getters & Setters =====

    // Scalars
    int32_t getSummedPeakThresh() const { return summed_peak_thresh_; }
    void setSummedPeakThresh(int32_t v) { summed_peak_thresh_ = v; }

    int32_t getChannelMultiplicity() const { return channel_multiplicity_; }
    void setChannelMultiplicity(int32_t v) { channel_multiplicity_ = v; }

    int32_t getRoiDelay0() const { return roi_delay_0_; }
    void setRoiDelay0(int32_t v) { roi_delay_0_ = v; }

    int32_t getRoiDelay1() const { return roi_delay_1_; }
    void setRoiDelay1(int32_t v) { roi_delay_1_ = v; }

    int32_t getRoiPrecount() const { return roi_precount_; }
    void setRoiPrecount(int32_t v) { roi_precount_ = v; }

    int32_t getRoiPeakWindow() const { return roi_peak_window_; }
    void setRoiPeakWindow(int32_t v) { roi_peak_window_ = v; }

    uint32_t getEnableTop() const { return enable_top_; }
    void setEnableTop(uint32_t v) { enable_top_ = v; }

    uint32_t getEnableMiddle() const { return enable_middle_; }
    void setEnableMiddle(uint32_t v) { enable_middle_ = v; }

    uint32_t getEnableBottom() const { return enable_bottom_; }
    void setEnableBottom(uint32_t v) { enable_bottom_ = v; }

    int32_t getNumRoiWords() const { return num_roi_words_; }
    void setNumRoiWords(int32_t v) { num_roi_words_ = v; }

    int32_t getRoiDeadtime() const { return roi_deadtime_; }
    void setRoiDeadtime(int32_t v) { roi_deadtime_ = v; }

    int32_t getPmtGateSize() const { return pmt_gate_size_; }
    void setPmtGateSize(int32_t v) { pmt_gate_size_ = v; }

    int32_t getPmtBeamSize() const { return pmt_beam_size_; }
    void setPmtBeamSize(int32_t v) { pmt_beam_size_ = v; }

    int32_t getFifoBlocksize() const { return fifo_blocksize_; }
    void setFifoBlocksize(int32_t v) { fifo_blocksize_ = v; }

    int32_t getTriggerSource() const { return trigger_source_; }
    void setTriggerSource(int32_t v) { trigger_source_ = v; }

    int32_t getSoftwareTriggerRateHz() const { return software_trigger_rate_hz_; }
    void setSoftwareTriggerRateHz(int32_t v) { software_trigger_rate_hz_ = v; }

    int32_t getTpcDeadTime() const { return tpc_dead_time_; }
    void setTpcDeadTime(int32_t v) { tpc_dead_time_ = v; }

    // prescale
    const std::array<int32_t, NUM_PRESCALES>& getPrescale() const { return prescale_; }
    std::array<int32_t, NUM_PRESCALES>& getPrescale() { return prescale_; }
    void setPrescale(const std::array<int32_t, NUM_PRESCALES>& v) { prescale_ = v; }

    // disc_threshold_0
    const std::array<int32_t, NUM_LIGHT_CHANNELS>& getDiscThreshold0() const { return disc_threshold_0_; }
    std::array<int32_t, NUM_LIGHT_CHANNELS>& getDiscThreshold0() { return disc_threshold_0_; }
    void setDiscThreshold0(const std::array<int32_t, NUM_LIGHT_CHANNELS>& v) { disc_threshold_0_ = v; }

    // disc_threshold_1
    const std::array<int32_t, NUM_LIGHT_CHANNELS>& getDiscThreshold1() const { return disc_threshold_1_; }
    std::array<int32_t, NUM_LIGHT_CHANNELS>& getDiscThreshold1() { return disc_threshold_1_; }
    void setDiscThreshold1(const std::array<int32_t, NUM_LIGHT_CHANNELS>& v) { disc_threshold_1_ = v; }

    // disc_threshold_3
    const std::array<int32_t, NUM_LIGHT_CHANNELS>& getDiscThreshold3() const { return disc_threshold_3_; }
    std::array<int32_t, NUM_LIGHT_CHANNELS>& getDiscThreshold3() { return disc_threshold_3_; }
    void setDiscThreshold3(const std::array<int32_t, NUM_LIGHT_CHANNELS>& v) { disc_threshold_3_ = v; }

    // disc_threshold_4
    const std::array<int32_t, NUM_LIGHT_CHANNELS>& getDiscThreshold4() const { return disc_threshold_4_; }
    std::array<int32_t, NUM_LIGHT_CHANNELS>& getDiscThreshold4() { return disc_threshold_4_; }
    void setDiscThreshold4(const std::array<int32_t, NUM_LIGHT_CHANNELS>& v) { disc_threshold_4_ = v; }
};

#endif //TPC_CONFIGS_H
