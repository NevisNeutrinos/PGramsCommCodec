//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef METRIC_BASE_H
#define METRIC_BASE_H

#include <communication_codes.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include <limits.h>
#include <tuple>
#include <stdexcept>
#include "constants.h"

#ifdef USE_PYTHON
    #include <pybind11/pybind11.h>
    #include <pybind11/numpy.h>

    namespace py = pybind11;
#endif

// This is the core interface for all serializable objects.

class MetricBase {
public:

    // // Central location for fixed constants.
    // constexpr static size_t NUM_CHARGE_CHANNELS = 64;
    // constexpr static size_t NUM_LIGHT_CHANNELS = 32;
    // constexpr static size_t CHARGE_BINS = 16;
    // constexpr static size_t LIGHT_BINS = 20;

    // Virtual destructor is essential for a polymorphic base class.
    virtual ~MetricBase() = default;

    /**
     * @brief Serializes the object's state into a vector of 32-bit integers.
     * @return A vector containing the serialized data.
     */
    virtual std::vector<uint32_t> serialize() const = 0;

    /**
     * @brief Deserializes data from an iterator and populates the object.
     * @param begin A const iterator to the beginning of the data range.
     * @param end A const iterator to the end of the data range.
     * @return An iterator pointing to the position after the last consumed element.
     */
    virtual std::vector<uint32_t>::const_iterator deserialize(std::vector<uint32_t>::const_iterator begin,
                                                             std::vector<uint32_t>::const_iterator end) = 0;

    /**
     * @brief Deserializes the object from a complete vector of data.
     * @details This is a convenience wrapper for the iterator-based deserialize method,
     * making it easier to use with Python bindings.
     * @param data The vector containing the data to deserialize.
     */
    void deserialize(const std::vector<uint32_t>& data) {
        deserialize(data.begin(), data.end());
    }

    /**
     * Helper functions to set and access bits in bit words
     */
    // Since the type isn't explicitly available for an enum member, cast it to the underlying type
    template <typename T>
    static constexpr auto to_underlying(T enum_member) noexcept {
        return static_cast<std::underlying_type_t<T>>(enum_member);
    }

    template <typename T>
    static inline void setBitWord(T &bit_word, T set_bit, bool unset) {
        if(set_bit >= static_cast<T>(sizeof(T) * CHAR_BIT)) {
            std::cerr << "Trying to shift a bit beyond the size of the bitword!" << std::endl;
        }
        if (unset) bit_word &= ~(0x1 << set_bit);
        else bit_word |= (0x1 << set_bit);
    }

    template <typename T>
    inline static auto getBit(T bit_word, T set_bit) {

        if(set_bit >= static_cast<T>(sizeof(T) * CHAR_BIT)) {
            std::cerr << "Trying to shift a bit beyond the size of the bitword!" << std::endl;
        }
        return bit_word & (0x1 << set_bit);
    }

    /**
     *  struct to automate the serialize/deserialize the metric classes
     * @tparam T The derived class to be serialized/de-serialized.
     */
    template <typename T>
    struct Serializer {
        // Convert tuple of ints to vector
        template <typename... Args>
        static std::vector<uint32_t> serialize_tuple(const std::tuple<Args...> t) {
            std::vector<uint32_t> result;
            result.reserve(sizeof...(Args));
            std::apply([&](const auto&... elems) {
                ((result.push_back(static_cast<uint32_t>(elems))), ...);
            }, t);
            return result;
        }

        // Deserialize vector into tuple
        template <typename... Args, typename Iter>
        static Iter deserialize_tuple(std::tuple<Args...> t, Iter it, Iter end) {
            auto size = std::distance(it, end);
            if (size < static_cast<decltype(size)>(sizeof...(Args)))
                throw std::runtime_error("Not enough data to deserialize tuple");

            std::apply([&](auto&... elems) {
                ((elems = *it++), ...);
            }, t);

            return it;
        }
    };

// Python binding functions
#ifdef USE_PYTHON
    /**
    * @brief Get a map of the metrics with a key giving the name and value giving the metric
    * @return The std::map is converted to a python dictionary holding the metrics
    */
    virtual py::dict getMetricDict() = 0;

    /**
    * @brief Convert a std::vector to a numpy array
    * @return Return  a numpy array
    */
    template <typename T>
    static py::array_t<T> vector_to_numpy_array_1d(const std::vector<T>& vec) {
        return py::array_t(vec.size(), vec.data());
    }

    /**
     * Return a copy of the C++ std::array to python as a numpy array.
     * @tparam T The array type (dtype in python)
     * @tparam N Size of the array
     * @param arr The C++ std::array
     * @return Return a copy of the std::array as a numpy array
     */
    template <typename T, size_t N>
    py::array_t<T> array_to_numpy_array_1d(const std::array<T, N>& arr) {
        return py::array_t<T>({N}, arr.data());
    }
#endif

};

#endif //METRIC_BASE_H
