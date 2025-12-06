//
// Created by Jon Sensenig on 9/10/25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>

namespace constants {
    namespace daq_computer {
        constexpr size_t NUM_CPUS = 6;
    }
    namespace tpc_readout {
        // Expected number of FEMs
        constexpr size_t NUM_FEMS = 4;
        constexpr size_t NUM_BOARDS = NUM_FEMS + 2; // + XMIT and Trigger
        // Trigger constants
        constexpr size_t NUM_PRESCALES = 9;
        // Expected number of channels and samples per channel for charge & light
        constexpr size_t NUM_CHARGE_CHANNELS = 192;
        constexpr size_t NUM_LIGHT_CHANNELS = 36;
        constexpr size_t NUM_CHARGE_SAMPLES = 763;
        constexpr size_t NUM_LIGHT_SAMPLES = 30;
        // Number of bins in the readout histograms
        constexpr static size_t CHARGE_BINS = 16;
        constexpr static size_t LIGHT_BINS = 20;
    }
}
#endif //CONSTANTS_H
