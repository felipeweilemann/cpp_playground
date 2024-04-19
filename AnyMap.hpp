#include <iostream>
#include <map>
#include <any>
#include <string>
#include <typeinfo>

#include <iostream>
#include <map>
#include <any>
#include <string>
#include <typeinfo>

class AnyMap {
public:
    // Add or update a key-value pair
    template <typename T>
    void set(const std::string& key, T value) {
        data[key] = std::any(value);
    }

    // Retrieve a value by key, with type checking
    template <typename T>
    T get(const std::string& key) const {
        if (data.find(key) == data.end()) {
            throw std::runtime_error("Key not found");
        }

        try {
            return std::any_cast<T>(data.at(key));
        } catch (const std::bad_any_cast& e) {
            throw std::runtime_error("Failed to cast value: " + std::string(e.what()));
        }
    }

    // Check if the key exists
    bool contains(const std::string& key) const {
        return data.find(key) != data.end();
    }

    // Remove a key-value pair by key
    bool removeByKey(const std::string& key) {
        auto it = data.find(key);
        if (it != data.end()) {
            data.erase(it);
            return true; // Key was found and removed
        }
        return false; // Key was not found
    }

    // Remove the first key-value pair that matches the provided value
    template <typename T>
    bool removeByValue(T value) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (std::any_cast<T>(&it->second) && std::any_cast<T>(it->second) == value) {
                data.erase(it);
                return true; // Found and removed the value
            }
        }
        return false; // Value not found
    }

private:
    std::map<std::string, std::any> data;
};