#include "lsm_kv_store.h"
#include <map>

LSMKVStore::LSMKVStore(size_t memtable_flush_threshold)
    : memtable_flush_threshold_(memtable_flush_threshold) {}

// LSMKVStore basic implementation
void LSMKVStore::put(const std::string& key, const std::string& value) {
    mem_.put(key, value);
    rotate_memtable_if_needed();
}

lsm::optional<std::string> LSMKVStore::get(const std::string& key) const {
    if (mem_.contains(key)) return mem_.get(key);

    for (auto it = immutable_memtables_.rbegin(); it != immutable_memtables_.rend(); ++it) {
        if (it->contains(key)) return it->get(key);
    }
    return lsm::nullopt;
}

void LSMKVStore::del(const std::string& key) {
    mem_.del(key);
    rotate_memtable_if_needed();
}

size_t LSMKVStore::memtable_size() const {
    return mem_.size();
}

size_t LSMKVStore::live_size() const {
    std::map<std::string, lsm::optional<std::string>> visible;

    for (const auto& immutable_memtable : immutable_memtables_) {
        for (const auto& entry : immutable_memtable.entries()) {
            visible[entry.first] = entry.second;
        }
    }

    for (const auto& entry : mem_.entries()) {
        visible[entry.first] = entry.second;
    }

    size_t count = 0;
    for (const auto& entry : visible) {
        if (entry.second.has_value()) count++;
    }
    return count;
}

size_t LSMKVStore::immutable_memtable_count() const {
    return immutable_memtables_.size();
}

void LSMKVStore::rotate_memtable_if_needed() {
    if (memtable_flush_threshold_ == 0 || mem_.size() < memtable_flush_threshold_) return;

    immutable_memtables_.push_back(mem_);
    mem_ = Memtable();
}
