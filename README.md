# LSM Store

A small C++17 key-value store project. The current implementation is phase 1: an in-memory memtable wrapped by an `LSMKVStore` facade. Deletes are represented with tombstones, and the store can rotate a full active memtable into immutable in-memory memtables.

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Test

```sh
ctest --test-dir build --output-on-failure
```

If CMake is not available, the tests can also be compiled directly with a C++17 compiler:

```sh
g++ -std=c++17 -Iinclude src/memtable.cpp src/lsm_kv_store.cpp tests/memtable_test.cpp -o /tmp/memtable_test
g++ -std=c++17 -Iinclude src/memtable.cpp src/lsm_kv_store.cpp tests/lsm_kv_store_test.cpp -o /tmp/lsm_kv_store_test
```

## Run

```sh
./build/lsm_store_cli
```

Supported commands:

```text
put <key> <value>
get <key>
del <key>
size
quit
```

## Next Steps

- Flush immutable memtables into a simple sorted SSTable format.
- Load existing SSTables on startup and search newest-to-oldest on reads.
- Add compaction after multiple SSTables exist.
