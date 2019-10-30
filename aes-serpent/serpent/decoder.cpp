#include "decoder.h"

#include <iostream>

serpent::decoder::decoder(std::string const& key) {
    if (key.size() > 32 || key.size() < 8) {
        throw std::runtime_error("Wrong key size");
    }
    std::bitset<256> real = convert_key(key);
    generate_keys(real, keys);
}

std::bitset<128> serpent::decoder::decode(std::bitset<128> const& message) {
    std::bitset<128> result = permute(message, table::IP);
    result = S(result ^ keys[32], 31, true) ^ keys[31];
    for (int i = 30; i >= 0; i--) {
        result = S(inv_linear(result), i, true) ^ keys[i];
    }
    result = permute(result, table::FP);
    return result;
}

std::string serpent::decoder::decode(std::string message) {
    assert(message.size() % 16 == 0);
    std::string result;
    for (size_t i = 0; i < message.size(); i += 16) {
        std::string chunk = message.substr(i, 16);
        auto block = to_block<16>(chunk);
        block = decode(block);
        result += from_block(block);
    }
    return result;
}

void serpent::decoder::decode(std::istream& in, std::ostream& out) {
    while (!in.eof()) {
        std::vector<unsigned char> buffer(256);
        in.read(reinterpret_cast<char*>(buffer.data()), 256);
        buffer.resize(static_cast<size_t>(in.gcount()));
        out << decode(std::string{buffer.begin(), buffer.end()});
    }
}
