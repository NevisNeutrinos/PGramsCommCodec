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
        // Light discriminator IDs
        constexpr uint16_t COSMIC_DISC_ID = 0x1;
        constexpr uint16_t BEAM_GATE_DISC_ID = 0x4;
        // Expected number of channels and samples per channel for charge & light
        // prefix "TOTAL" is the number of channels on the hardware
        // prefix "NUM" is number of used channels, currently 180 for charge and 36 for light
        constexpr size_t TOTAL_CHARGE_CHANNELS = 192;
        constexpr size_t NUM_CHARGE_CHANNELS = 192;
        constexpr size_t TOTAL_LIGHT_CHANNELS = 40;
        constexpr size_t NUM_LIGHT_CHANNELS = 36;
        constexpr size_t NUM_CHARGE_SAMPLES = 763;
        constexpr size_t CHARGE_ONE_FRAME = 256;
        constexpr size_t NUM_LIGHT_SAMPLES = 208;
        constexpr size_t CHARGE_START_SAMPLES = 256;
        constexpr size_t CHARGE_END_SAMPLES = 512;
        // Since the ADC words are 12b we can pack 2 per 32b word
        constexpr double packed_words_divisor_ = 0.5;
        constexpr size_t DOUBLE_PACK_CHARGE_CH = packed_words_divisor_ * NUM_CHARGE_CHANNELS;
        constexpr size_t DOUBLE_PACK_LIGHT_CH = packed_words_divisor_ * NUM_LIGHT_CHANNELS;
        constexpr size_t DOUBLE_PACK_LIGHT_SAMPLE = packed_words_divisor_ * NUM_LIGHT_SAMPLES;
        constexpr size_t DOUBLE_PACK_CHARGE_ONE_FRAME = packed_words_divisor_ * CHARGE_ONE_FRAME;

        // Number of bins in the readout histograms
        constexpr static size_t CHARGE_BINS = 16;
        constexpr static size_t LIGHT_BINS = 20;
    }
}
#endif //CONSTANTS_H
