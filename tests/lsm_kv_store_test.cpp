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

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }
    std::cout << failures << " TEST(S) FAILED\n";
    return 1;
}
