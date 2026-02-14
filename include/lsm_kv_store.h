#pragma once
#include <cstddef>
#include "kv_store.h"
#include "memtable.h"

class LSMKVStore final : public KVStore {
public:
    void put(const std::string& key, const std::string& value) override;
    lsm::optional<std::string> get(const std::string& key) override;
    void del(const std::string& key) override;

    size_t memtable_size() const;

private:
    Memtable mem_;
};
