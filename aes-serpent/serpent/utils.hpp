#ifndef SERPENT_UTILS_HPP
#define SERPENT_UTILS_HPP

#include <bitset>
#include <cassert>
#include <vector>

#include "tables.hpp"

namespace serpent {

template <size_t num> void copy_and_append(std::bitset<num> const& key, std::bitset<256>& real) {
    for (size_t i = 0; i < std::min<size_t>(num, 256); i++) {
        real[i] = key[i];
    }
    if (num < 256) {
        real[num] = 1;
    }
    for (size_t i = num + 1; i < 256; i++) {
        real[i] = 0;
    }
}

template <size_t num1, size_t num2> std::bitset<num1 + num2> operator+(std::bitset<num1> const& one, std::bitset<num2> const& two) {
    std::bitset<num1 + num2> res;
    for (size_t i = 0; i < num1; i++) {
        res[i] = one[i];
    }
    for (size_t i = num1; i < num1 + num2; i++) {
        res[i] = two[i - num1];
    }
    return res;
}

template <size_t del, size_t num> std::array<std::bitset<num / del>, del> split(std::bitset<num> const& message) {
    std::array<std::bitset<num / del>, del> splited;
    size_t counter = 0;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 32; j++) {
            splited[i][j] = message[counter++];
        }
    }
    return splited;
}

std::bitset<32> cshr(std::bitset<32> const& val, size_t shift);

std::bitset<32> shr(std::bitset<32> const& val, size_t shift);

std::bitset<32> cshl(std::bitset<32> const& val, size_t shift);

std::bitset<32> shl(std::bitset<32> const& val, size_t shift);

std::array<std::bitset<32>, 4> S(std::array<std::bitset<32>, 4> const& words, size_t round);

std::array<std::bitset<32>, 4> S1(std::bitset<128> const& message, size_t round);

std::bitset<128> S(std::bitset<128> const& message, size_t round);

// TODO S and invS are same except table
std::array<std::bitset<32>, 4> invS(std::array<std::bitset<32>, 4> const& words, size_t round);

std::array<std::bitset<32>, 4> invS1(std::bitset<128> const& message, size_t round);

std::bitset<128> invS(std::bitset<128> const& message, size_t round);

void generate_keys(std::bitset<256> const& real, std::array<std::bitset<128>, 33>& keys);

template <size_t num, size_t resnum> std::bitset<resnum> permute(std::bitset<num> const& original, std::array<size_t, resnum> const& table) {
    std::bitset<resnum> result;
    for (size_t i = 0; i < resnum; i++) {
        result[i] = original[table[i]];
    }
    return result;
}

template <size_t size> std::bitset<size * 8> to_block(std::string const& s) {
    assert(s.size() == size);
    std::bitset<size * 8> result;
    size_t counter = 0;
    for (char const& c : s) {
        for (size_t i = 0; i < 8; i++) {
            result[counter++] = (c >> (7 - i)) & 1;
        }
    }
    return result;
}

std::string from_block(std::bitset<128> const& b);

std::bitset<128> linear(std::bitset<128> const& mes);

std::bitset<128> inv_linear(std::bitset<128> const& mes);

std::bitset<256> convert_key(std::string const& key);

} // namespace serpent

#endif
