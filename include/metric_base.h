//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef METRIC_BASE_H
#define METRIC_BASE_H

#include <vector>
#include <cstdint>
#include <tuple>
#include <stdexcept>
#include <algorithm>


#ifdef USE_PYTHON
    #include <pybind11/pybind11.h>
    #include <pybind11/numpy.h>

    namespace py = pybind11;
#endif

// This is the core interface for all serializable objects.

class MetricBase {
public:

    // Central location for fixed constants.
    constexpr static size_t NUM_CHARGE_CHANNELS = 64;
    constexpr static size_t NUM_LIGHT_CHANNELS = 32;
    constexpr static size_t CHARGE_BINS = 16;
    constexpr static size_t LIGHT_BINS = 20;

    // Virtual destructor is essential for a polymorphic base class.
    virtual ~MetricBase() = default;

    /**
     * @brief Serializes the object's state into a vector of 32-bit integers.
     * @return A vector containing the serialized data.
     */
    virtual std::vector<int32_t> serialize() const = 0;

    /**
     * @brief Deserializes data from an iterator and populates the object.
     * @param begin A const iterator to the beginning of the data range.
     * @param end A const iterator to the end of the data range.
     * @return An iterator pointing to the position after the last consumed element.
     */
    virtual std::vector<int32_t>::const_iterator deserialize(std::vector<int32_t>::const_iterator begin,
                                                             std::vector<int32_t>::const_iterator end) = 0;

    /**
     * @brief Deserializes the object from a complete vector of data.
     * @details This is a convenience wrapper for the iterator-based deserialize method,
     * making it easier to use with Python bindings.
     * @param data The vector containing the data to deserialize.
     */
    void deserialize(const std::vector<int32_t>& data) {
        deserialize(data.begin(), data.end());
    }

    /**
     * Function to create a tuple of all the concrete class members.
     * The order of the members in std::tie(member0, member1, ..., memberN) defines the
     * serialization order.
     *
     * This isn't useful for more complicated metrics so do not make it pure virtual.
     *
     * @return tuple of form std::tuple<int &, int &, ..., int &>
     */
    virtual auto member_tuple();
    virtual auto member_tuple() const;

    /**
     *  struct to automate the serialize/deserialize the metric classes
     * @tparam T The derived class to be serialized/de-serialized.
     */
    template <typename T>
    struct Serializer {
        // Convert tuple of ints to vector
        template <typename... Args>
        static std::vector<int32_t> serialize_tuple(const std::tuple<Args...>& t) {
            std::vector<int32_t> result;
            result.reserve(sizeof...(Args));
            std::apply([&](const auto&... elems) {
                ((result.push_back(static_cast<int32_t>(elems))), ...);
            }, t);
            return result;
        }

        // Deserialize vector into tuple
        template <typename... Args, typename Iter>
        static Iter deserialize_tuple(std::tuple<Args...>& t, Iter it, Iter end) {
            auto idx = 0;
            auto size = std::distance(it, end);
            if (size < sizeof...(Args))
                throw std::runtime_error("Not enough data to deserialize tuple");

            std::apply([&](auto&... elems) {
                ((elems = *it++), ...);
            }, t);

            return it;
        }
    };


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
#endif

};

#endif //METRIC_BASE_H
