//
// Created by Jon Sensenig on 8/23/25.
//

#include "../include/histogram.h"
#include <stdexcept>
#include <iostream>
#include <cmath>

Histogram::Histogram() : min_value(0), max_value(1), num_bins(1), bin_width(1.0),
                         below_range_count(0), above_range_count(0) {
    bins.resize(num_bins, 0);
}

Histogram::Histogram(int32_t min, int32_t max, int32_t bins_count)
    : min_value(min), max_value(max), num_bins(bins_count),
      below_range_count(0), above_range_count(0) {
    if (max <= min) {
        throw std::invalid_argument("max_value must be greater than min_value.");
    }
    if (bins_count <= 0) {
        throw std::invalid_argument("Number of bins must be positive.");
    }
    bins.resize(num_bins, 0);
    bin_width = static_cast<double>(max_value - min_value) / num_bins;
}

void Histogram::fill(int32_t value) {
    if (value < min_value) {
        below_range_count++;
    } else if (value >= max_value) {
        above_range_count++;
    } else {
        int bin_index = static_cast<int>(std::floor((value - min_value) / bin_width));
        if (bin_index >= 0 && bin_index < num_bins) {
            bins[bin_index]++;
        }
    }
}

void Histogram::clear() {
    std::fill(bins.begin(), bins.end(), 0);
    below_range_count = 0;
    above_range_count = 0;
}

std::vector<int32_t> Histogram::serialize() const {
    std::vector<int32_t> serialized_data;
    // Reserve space for efficiency
    serialized_data.reserve(5 + bins.size());

    // Metadata
    serialized_data.push_back(min_value);
    serialized_data.push_back(max_value);
    serialized_data.push_back(num_bins);
    serialized_data.push_back(below_range_count);
    serialized_data.push_back(above_range_count);

    // Bin data
    serialized_data.insert(serialized_data.end(), bins.begin(), bins.end());
    return serialized_data;
}

std::vector<int32_t>::const_iterator Histogram::deserialize(std::vector<int32_t>::const_iterator begin,
                                                            std::vector<int32_t>::const_iterator end) {
    // Use an iterator to read data, which is much safer than indexing.
    auto it = begin;

    // Ensure there's enough data for metadata
    if (std::distance(it, end) < 5) {
        throw std::runtime_error("Deserialization failed: not enough data for Histogram metadata.");
    }

    min_value = *it++;
    max_value = *it++;
    num_bins = *it++;
    below_range_count = *it++;
    above_range_count = *it++;

    // Re-initialize the histogram's structure based on deserialized metadata
    if (max_value <= min_value || num_bins <= 0) {
        throw std::runtime_error("Deserialization failed: invalid histogram parameters.");
    }
    bins.resize(num_bins);
    bin_width = static_cast<double>(max_value - min_value) / num_bins;

    // Ensure there's enough data for the bins
    if (std::distance(it, end) < num_bins) {
        throw std::runtime_error("Deserialization failed: not enough data for Histogram bins.");
    }

    // Copy bin data
    std::copy(it, it + num_bins, bins.begin());
    it += num_bins;

    // Return the iterator pointing to the next piece of data
    return it;
}

void Histogram::print() const {
    std::cout << "--- Histogram ---" << std::endl;
    std::cout << "Range: [" << min_value << ", " << max_value << "), Bins: " << num_bins << std::endl;
    std::cout << "Values below range: " << below_range_count << std::endl;
    for (int i = 0; i < num_bins; ++i) {
        double bin_start = min_value + i * bin_width;
        double bin_end = bin_start + bin_width;
        std::cout << "  Bin " << i << " [" << bin_start << ", " << bin_end << "): " << bins[i] << std::endl;
    }
    std::cout << "Values above range: " << above_range_count << std::endl;
    std::cout << "-----------------" << std::endl;
}