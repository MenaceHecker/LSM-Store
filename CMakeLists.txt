cmake_minimum_required(VERSION 3.16)
project(lsm_store_phase1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(lsm_store
    src/memtable.cpp
    src/lsm_kv_store.cpp
)

add_executable(lsm_store_cli main.cpp)
target_link_libraries(lsm_store_cli PRIVATE lsm_store)

add_executable(memtable_test tests/memtable_test.cpp)
target_link_libraries(memtable_test PRIVATE lsm_store)
