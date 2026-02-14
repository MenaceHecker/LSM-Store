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
    expect(!m.get("missing").has_value(), "missing key returns nullopt");

    m.put("a", "1");
    m.put("b", "2");
    expect(m.size() == 2, "size after puts");

    auto a = m.get("a");
    expect(a.has_value() && *a == "1", "get(a) == 1");

    m.put("a", "10");
    a = m.get("a");
    expect(a.has_value() && *a == "10", "overwrite updates value");

    m.del("a");
    expect(!m.get("a").has_value(), "deleted key not found");
    expect(m.size() == 1, "size after delete");

    m.del("does_not_exist");
    expect(m.size() == 1, "deleting missing key doesn't change size");

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }
    std::cout << failures << " TEST(S) FAILED\n";
    return 1;
}
