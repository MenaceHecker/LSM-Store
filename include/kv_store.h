#pragma once
#include <string>
#include "optional_compat.h"

class KVStore {
public:
    virtual void put(const std::string& key, const std::string& value) = 0;
    virtual lsm::optional<std::string> get(const std::string& key) = 0;
    virtual void del(const std::string& key) = 0;
    virtual ~KVStore() = default;
};
