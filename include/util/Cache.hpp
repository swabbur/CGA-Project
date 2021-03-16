#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <string>

/**
 * A cache for values that are costly to load or store.
 *
 * @tparam Key The type of keys used for loading and retrieving values.
 * @tparam Value The type of values that are stored in the cache.
 */
template<typename Key, typename Value>
class Cache {

    std::mutex mutex;
    std::map<std::string, Value> values;
    std::function<Value(Key)> load;

public:

    /**
     * Create a cache for values loaded using the provided load function.
     *
     * @param load The function used to load values.
     */
    explicit Cache(std::function<Value(Key)> load) : mutex(), values(), load(std::move(load)) {}

    /**
     * A cache should never be moved.
     */
    Cache(Cache const &) = delete;

    /**
     * Move the given cache, including its cached values and load function, into a new cache. Beware, the move is not
     * thread-safe.
     */
    Cache(Cache && cache) noexcept : mutex(), values(std::move(cache.values)), load(std::move(load)) {}

    /**
     * Destroy the cache and its contained values.
     */
    ~Cache() = default;

    /**
     * Either retrieve the value associated with the given key from the cache, or load, store, and return a new value
     * using the load function passed to the cache's constructor.
     *
     * @param key The key used to retrieve or load a value.
     * @return A reference to the retrieved or loaded value.
     */
    Value & get(Key const & key) {

        // Synchronize such that no values are unnecessarily loaded multiple times
        std::scoped_lock<std::mutex> lock(mutex);

        // Retrieve a cached value
        auto iterator = values.find(key);
        if (iterator != values.end()) {
            return iterator->second;
        }

        // Load, store, and return a new value
        Value value = load(key);
        auto [new_iterator, inserted] = values.emplace(key, std::move(value));
        if (!inserted) {
            throw std::runtime_error("Failed to store loaded value in cache: " + key);
        }
        return new_iterator->second;
    }

    /**
     * Clear the cache, destroying its contained values.
     */
    void clear() {
        values.clear();
    }
};
