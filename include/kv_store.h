#pragma once
#include <optional>
#include <string>

class KVStore {
public:
    virtual void put(const std::string& key, const std::string& value) = 0;
    virtual std::optional<std::string> get(const std::string& key) = 0;
    virtual void del(const std::string& key) = 0;
    virtual ~KVStore() = default;
};
