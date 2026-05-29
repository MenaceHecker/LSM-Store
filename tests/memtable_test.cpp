#include "memtable.h"
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
    Memtable m;

    expect(m.size() == 0, "initial size is 0");
    expect(m.live_size() == 0, "initial live size is 0");
    expect(!m.contains("missing"), "missing key is not contained");
    expect(!m.get("missing").has_value(), "missing key returns nullopt");

    m.put("a", "1");
    m.put("b", "2");
    expect(m.size() == 2, "size after puts");
    expect(m.live_size() == 2, "live size after puts");
    expect(m.contains("a"), "put key is contained");

    auto a = m.get("a");
    expect(a.has_value() && *a == "1", "get(a) == 1");

    m.put("a", "10");
    a = m.get("a");
    expect(a.has_value() && *a == "10", "overwrite updates value");
    expect(!m.has_tombstone("a"), "put clears tombstone state");

    m.del("a");
    expect(!m.get("a").has_value(), "deleted key not found");
    expect(m.contains("a"), "deleted key is still contained as tombstone");
    expect(m.size() == 2, "delete keeps tombstone record");
    expect(m.live_size() == 1, "live size after delete");
    expect(m.has_tombstone("a"), "deleted key has tombstone");

    m.del("does_not_exist");
    expect(m.size() == 3, "deleting missing key records tombstone");
    expect(m.live_size() == 1, "deleting missing key doesn't change live size");
    expect(m.has_tombstone("does_not_exist"), "missing delete has tombstone");

    m.put("a", "100");
    a = m.get("a");
    expect(a.has_value() && *a == "100", "put after delete restores value");
    expect(!m.has_tombstone("a"), "put after delete removes tombstone");
    expect(m.size() == 3, "put after delete reuses tombstone slot");
    expect(m.live_size() == 2, "live size after restoring deleted key");

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }
    std::cout << failures << " TEST(S) FAILED\n";
    return 1;
}
