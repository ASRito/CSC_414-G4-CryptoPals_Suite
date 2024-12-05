#pragma once

#include <string>
#include <set>

using namespace std;

bool detect_ecb(const string& ciphertext, size_t block_size = 16);

bool detect_ecb(const string& ciphertext, size_t block_size) {
    set<string> unique_blocks;
    size_t num_blocks = ciphertext.size() / block_size;

    for (size_t i = 0; i < num_blocks; ++i) {
        string block = ciphertext.substr(i * block_size, block_size);

        if (unique_blocks.find(block) != unique_blocks.end()) {
            return true; // ECB detected
        }
        unique_blocks.insert(block);
    }

    return false;
}