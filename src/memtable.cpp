#include "memtable.h"

void Memtable::put(const std::string& key, const std::string& value) {
    table_[key] = value;
}

lsm::optional<std::string> Memtable::get(const std::string& key) const {
    auto it = table_.find(key);
    if (it == table_.end()) return lsm::nullopt;
    return it->second;
}

bool Memtable::contains(const std::string& key) const {
    return table_.find(key) != table_.end();
}

void Memtable::del(const std::string& key) {
    table_[key] = lsm::nullopt;
}

size_t Memtable::size() const {
    return table_.size();
}

size_t Memtable::live_size() const {
    size_t count = 0;
    for (const auto& item : table_) {
        if (item.second.has_value()) count++;
    }
    return count;
}

bool Memtable::has_tombstone(const std::string& key) const {
    auto it = table_.find(key);
    return it != table_.end() && !it->second.has_value();
}

const Memtable::EntryMap& Memtable::entries() const {
    return table_;
}
