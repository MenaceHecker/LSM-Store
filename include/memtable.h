#pragma once
#include <cstddef>
#include <map>
#include <string>
#include "optional_compat.h"

class Memtable {
public:
    void put(const std::string& key, const std::string& value);
    lsm::optional<std::string> get(const std::string& key) const;
    void del(const std::string& key);
    size_t size() const;

private:
    std::map<std::string, std::string> table_;
};
