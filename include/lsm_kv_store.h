#pragma once
#include <cstddef>
#include <vector>
#include "kv_store.h"
#include "memtable.h"

class LSMKVStore final : public KVStore {
public:
    explicit LSMKVStore(size_t memtable_flush_threshold = 0);

    void put(const std::string& key, const std::string& value) override;
    lsm::optional<std::string> get(const std::string& key) const override;
    void del(const std::string& key) override;

    size_t memtable_size() const;
    size_t live_size() const;
    size_t immutable_memtable_count() const;

private:
    void rotate_memtable_if_needed();

    size_t memtable_flush_threshold_;
    Memtable mem_;
    std::vector<Memtable> immutable_memtables_;
};
