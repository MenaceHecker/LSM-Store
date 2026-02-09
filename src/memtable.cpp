#include "memtable.h"

void Memtable::put(const string& key, const string& value) {
    table_[key] = value;
}

// optional<string> Memtable::get(const string& key) const {
//     auto it = table_.find(key);
//     if (it == table_.end()) {
//         return std::nullopt;
//     }
//     return it->second;
// 

void Memtable::del(const string& key) {
    table_.erase(key);
}

std::size_t Memtable::size() const {
    return table_.size();
}
