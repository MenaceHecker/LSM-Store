#include "lsm_kv_store.h"
#include <iostream>
#include <string>

static int failures = 0;

static void expect(bool cond, const std::string& msg) {
    if (!cond) {
        std::cerr << "FAIL: " << msg << "\n";
        failures++;
    }
}

int main() {
    LSMKVStore db;

    expect(db.memtable_size() == 0, "initial memtable size is 0");
    expect(db.live_size() == 0, "initial live size is 0");
    expect(db.immutable_memtable_count() == 0, "initial immutable memtable count is 0");
    expect(!db.get("missing").has_value(), "missing key returns nullopt");

    db.put("a", "1");
    db.put("b", "two words");
    expect(db.memtable_size() == 2, "memtable size after puts");
    expect(db.live_size() == 2, "live size after puts");

    auto a = db.get("a");
    expect(a.has_value() && *a == "1", "get(a) == 1");

    auto b = db.get("b");
    expect(b.has_value() && *b == "two words", "values can contain spaces");

    db.put("a", "10");
    a = db.get("a");
    expect(a.has_value() && *a == "10", "overwrite updates value");
    expect(db.memtable_size() == 2, "overwrite does not grow memtable");
    expect(db.live_size() == 2, "overwrite does not change live size");

    db.del("a");
    expect(!db.get("a").has_value(), "deleted key not found");
    expect(db.memtable_size() == 2, "delete keeps tombstone in memtable");
    expect(db.live_size() == 1, "delete reduces live size");

    db.del("missing");
    expect(db.memtable_size() == 3, "deleting missing key records tombstone");
    expect(db.live_size() == 1, "deleting missing key leaves live size unchanged");

    const LSMKVStore& read_only_db = db;
    expect(read_only_db.get("b").has_value(), "get works through const reference");

    LSMKVStore rotating_db(2);
    rotating_db.put("a", "1");
    expect(rotating_db.memtable_size() == 1, "thresholded memtable holds first write");
    expect(rotating_db.immutable_memtable_count() == 0, "no rotation before threshold");

    rotating_db.put("b", "2");
    expect(rotating_db.memtable_size() == 0, "active memtable resets after threshold");
    expect(rotating_db.immutable_memtable_count() == 1, "rotation creates immutable memtable");
    expect(rotating_db.live_size() == 2, "live size includes immutable memtables");

    auto flushed_a = rotating_db.get("a");
    expect(flushed_a.has_value() && *flushed_a == "1", "get reads from immutable memtable");

    rotating_db.put("a", "new");
    flushed_a = rotating_db.get("a");
    expect(flushed_a.has_value() && *flushed_a == "new", "active value wins over immutable value");
    expect(rotating_db.live_size() == 2, "overwrite across memtables does not grow live size");

    rotating_db.del("b");
    expect(rotating_db.memtable_size() == 0, "delete can trigger rotation");
    expect(rotating_db.immutable_memtable_count() == 2, "second rotation creates another immutable memtable");
    expect(!rotating_db.get("b").has_value(), "newer tombstone masks older immutable value");
    expect(rotating_db.live_size() == 1, "tombstone across memtables reduces live size");

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }
    std::cout << failures << " TEST(S) FAILED\n";
    return 1;
}
