cmake_minimum_required(VERSION 3.16)
project(lsm_store_phase1 LANGUAGES CXX)

add_library(lsm_store
    src/memtable.cpp
    src/lsm_kv_store.cpp
)
target_compile_features(lsm_store PUBLIC cxx_std_17)
target_include_directories(lsm_store PUBLIC include)

add_executable(lsm_store_cli main.cpp)
target_link_libraries(lsm_store_cli PRIVATE lsm_store)

add_executable(memtable_test tests/memtable_test.cpp)
target_link_libraries(memtable_test PRIVATE lsm_store)

add_executable(lsm_kv_store_test tests/lsm_kv_store_test.cpp)
target_link_libraries(lsm_kv_store_test PRIVATE lsm_store)

enable_testing()
add_test(NAME memtable_test COMMAND memtable_test)
add_test(NAME lsm_kv_store_test COMMAND lsm_kv_store_test)
