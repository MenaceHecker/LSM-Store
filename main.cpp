#include "lsm_kv_store.h"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    LSMKVStore db;

    std::cout << "LSM-Store Phase 1 (in-memory)\n";
    std::cout << "Commands:\n";
    std::cout << "  put <key> <value>\n";
    std::cout << "  get <key>\n";
    std::cout << "  del <key>\n";
    std::cout << "  size\n";
    std::cout << "  quit\n\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "quit" || cmd == "exit") break;

        if (cmd == "put") {
            std::string key, value;
            iss >> key;
            std::getline(iss, value);
            if (!value.empty() && value[0] == ' ') value.erase(0, 1);

            if (key.empty() || value.empty()) {
                std::cout << "usage: put <key> <value>\n";
                continue;
            }
            db.put(key, value);
            std::cout << "ok\n";
            continue;
        }

        if (cmd == "get") {
            std::string key;
            iss >> key;
            if (key.empty()) {
                std::cout << "usage: get <key>\n";
                continue;
            }
            auto v = db.get(key);
            if (!v) std::cout << "(nil)\n";
            else std::cout << *v << "\n";
            continue;
        }

        if (cmd == "del") {
            std::string key;
            iss >> key;
            if (key.empty()) {
                std::cout << "usage: del <key>\n";
                continue;
            }
            db.del(key);
            std::cout << "ok\n";
            continue;
        }

        if (cmd == "size") {
            std::cout << db.memtable_size() << "\n";
            continue;
        }

        if (!cmd.empty()) std::cout << "unknown command: " << cmd << "\n";
    }

    return 0;
}