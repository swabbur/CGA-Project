#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <string>

template<typename Key, typename Value>
class Cache {

    std::mutex mutex;
    std::map<std::string, Value> values;
    std::function<Value(Key)> load;

public:

    explicit Cache(std::function<Value(Key)> load);

    Cache(Cache const &) = delete;

    Cache(Cache &&) = delete;

    ~Cache();

    Value & get(Key const & key);
};

template<typename Key, typename Value>
Cache<Key, Value>::Cache(std::function<Value(Key)> load) : mutex(), values(), load(std::move(load)) {}

template<typename Key, typename Value>
Cache<Key, Value>::~Cache() = default;

template<typename Key, typename Value>
Value & Cache<Key, Value>::get(Key const & key) {
    std::scoped_lock<std::mutex> lock(mutex);
    auto iterator = values.find(key);
    if (iterator == values.end()) {
        Value value = load(key);
        auto [new_iterator, inserted] = values.emplace(key, std::move(value));
        if (!inserted) {
            throw std::runtime_error("Failed to add loaded value to cache: " + key);
        }
        return new_iterator->second;
    }
    return iterator->second;
}
