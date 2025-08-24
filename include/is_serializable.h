//
// Created by Jon Sensenig on 8/23/25.
//

#ifndef IS_SERIALIZABLE_H
#define IS_SERIALIZABLE_H

#include <vector>
#include <cstdint>

// This is the core interface for all serializable objects.

class ISerializable {
public:

    // Central location for fixed constants.
    constexpr static size_t NUM_CHARGE_CHANNELS = 64;
    constexpr static size_t NUM_LIGHT_CHANNELS = 32;
    constexpr static size_t CHARGE_BINS = 16;
    constexpr static size_t LIGHT_BINS = 20;

    // Virtual destructor is essential for a polymorphic base class.
    virtual ~ISerializable() = default;

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
};

#endif //IS_SERIALIZABLE_H
