#include "lsm_kv_store.h"

// LSMKVStore implementation
void LSMKVStore::put(const std::string& key, const std::string& value) {
    mem_.put(key, value);
}

// For simplicity, we only check the memtable. In a full implementation, we would also check on-disk structures.
std::optional<std::string> LSMKVStore::get(const std::string& key) {
    return mem_.get(key);
}

void LSMKVStore::del(const std::string& key) {
    mem_.del(key);
}

size_t LSMKVStore::memtable_size() const {
    return mem_.size();
}
