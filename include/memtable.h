#pragma once
#include <cstddef>
#include <map>
#include <optional>
#include <string>

using std::optional;
using std::string;

class Memtable {
public:
    void put(const string& key, const string& value);
    optional<string> get(const string& key) const;
    void del(const string& key);
    std::size_t size() const;

private:
    std::map<std::string, std::string> table_;
};
