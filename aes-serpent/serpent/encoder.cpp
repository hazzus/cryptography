#include "encoder.h"

#include <iostream>

serpent::encoder::encoder(std::string const& key) {
    if (key.size() > 32 || key.size() < 8) {
        // TODO throw 1; do sm
    }
    std::bitset<256> real = convert_key(key);
    generate_keys(real, keys);
}

std::bitset<128> serpent::encoder::encode(const std::bitset<128>& message) {
    auto result = permute(message, table::IP);
    for (size_t i = 0; i < 31; i++) {
        result = linear(S(result ^ keys[i], i));
    }
    result = S(result ^ keys[31], 31) ^ keys[32];
    result = permute(result, table::FP);
    return result;
}

std::string serpent::encoder::encode(std::string message) {
    std::string result;
    if (message.size() % 16 != 0) {
        message.append(16 - message.size() % 16, '\0');
    }
    for (size_t i = 0; i < message.size(); i += 16) {
        std::string chunk = message.substr(i, 16);
        auto block = to_block<16>(chunk);
        block = encode(block);
        result += from_block(block);
    }
    return result;
}
