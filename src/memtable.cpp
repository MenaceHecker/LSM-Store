#include "memtable.h"

void Memtable::put(const std::string& key, const std::string& value) {
    table_[key] = value;
}

lsm::optional<std::string> Memtable::get(const std::string& key) const {
    auto it = table_.find(key);
    if (it == table_.end()) return lsm::nullopt;
    return it->second;
}

void Memtable::del(const std::string& key) {
    table_.erase(key);
}

size_t Memtable::size() const {
    return table_.size();
}
