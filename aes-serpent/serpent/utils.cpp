#include "utils.hpp"

namespace serpent {

std::bitset<32> cshr(std::bitset<32> const& val, size_t shift) {
    assert(shift < 32);
    std::bitset<32> res;
    size_t cur = 0;
    for (size_t i = shift; i < 32; i++) {
        res[cur++] = val[i];
    }
    for (size_t i = 0; i < shift; i++) {
        res[cur++] = val[i];
    }
    return res;
}

std::bitset<32> shr(std::bitset<32> const& val, size_t shift) {
    assert(shift < 32);
    std::bitset<32> res;
    size_t cur = 0;
    for (size_t i = shift; i < 32; i++) {
        res[cur++] = val[i];
    }
    return res;
}

std::bitset<32> cshl(std::bitset<32> const& val, size_t shift) {
    assert(shift < 32);
    std::bitset<32> res;
    size_t cur = 0;
    for (size_t i = 32 - shift; i < 32; i++) {
        res[cur++] = val[i];
    }
    for (size_t i = 0; i < 32 - shift; i++) {
        res[cur++] = val[i];
    }
    return res;
}

std::bitset<32> shl(std::bitset<32> const& val, size_t shift) {
    assert(shift < 32);
    std::bitset<32> res;
    size_t cur = shift;
    for (size_t i = 0; i < 32 - shift; i++) {
        res[cur++] = val[i];
    }
    return res;
}

std::array<std::bitset<32>, 4> S(std::array<std::bitset<32>, 4> const& words, size_t round) {
    auto current_s = table::S[round % 8];
    std::array<std::bitset<32>, 4> result;
    for (size_t i = 0; i < 4; i++) {
        uint32_t cur = words[i].to_ulong();
        uint32_t res = 0;
        for (size_t j = 0; j < 4; j++) {
            unsigned char byte = (cur >> (8 * j)) & 0xff;
            unsigned char lower = byte >> 4;
            unsigned char upper = byte & 0x0f;
            res |= ((current_s[lower] << 4) | (current_s[upper])) << ((3 - j) * 8);
        }
        result[i] = std::bitset<32>(res);
    }
    return result;
}

std::array<std::bitset<32>, 4> S1(std::bitset<128> const& message, size_t round) {
    auto splited = split<4>(message);
    return S(splited, round);
}

std::bitset<128> S(std::bitset<128> const& message, size_t round) {
    auto res = S1(message, round);
    return res[0] + res[1] + res[2] + res[3];
}

// TODO S and invS are same except table
std::array<std::bitset<32>, 4> invS(std::array<std::bitset<32>, 4> const& words, size_t round) {
    auto current_s = table::INV_S[round % 8];
    std::array<std::bitset<32>, 4> result;
    for (size_t i = 0; i < 4; i++) {
        uint32_t cur = words[i].to_ulong();
        uint32_t res = 0;
        for (size_t j = 0; j < 4; j++) {
            unsigned char byte = (cur >> (8 * j)) & 0xff;
            unsigned char lower = byte >> 4;
            unsigned char upper = byte & 0x0f;
            res |= ((current_s[lower] << 4) | (current_s[upper])) << ((3 - j) * 8);
        }
        result[i] = std::bitset<32>(res);
    }
    return result;
    ;
}

std::array<std::bitset<32>, 4> invS1(std::bitset<128> const& message, size_t round) {
    auto splited = split<4>(message);
    return invS(splited, round);
}

std::bitset<128> invS(std::bitset<128> const& message, size_t round) {
    auto res = invS1(message, round);
    return res[0] + res[1] + res[2] + res[3];
}

void generate_keys(std::bitset<256> const& real, std::array<std::bitset<128>, 33>& keys) {
    std::vector<std::bitset<32>> words(140);
    for (size_t i = 0; i < 256; i++) {
        words[i / 32][i % 32] = real[i];
    }
    for (size_t i = 8; i < 140; i++) {
        std::bitset<32> ibit(i - 8);
        words[i] = cshr(words[i - 8] ^ words[i - 5] ^ words[i - 3] ^ words[i - 1] ^ table::phi ^ ibit, 11);
    }
    words.erase(words.begin(), words.begin() + 8);
    assert(words.size() == 132);

    std::vector<std::bitset<32>> almost_keys(132);
    for (size_t i = 0, j = 3, cnt = 0; i < 33; i++, (j == 0) ? j = 7 : j--) {
        auto pack = S({words[i * 4], words[i * 4 + 1], words[i * 4 + 2], words[i * 4 + 3]}, j);
        for (size_t i = 0; i < 4; i++) {
            almost_keys[cnt++] = pack[i];
        }
    }
    for (size_t i = 0; i < 33; i++) {
        keys[i] = almost_keys[4 * i] + almost_keys[4 * i + 1] + almost_keys[4 * i + 2] + almost_keys[4 * i + 3];
        keys[i] = permute(keys[i], table::IP);
    }
}

std::string from_block(std::bitset<128> const& b) {
    std::vector<unsigned char> res(16);
    size_t counter = 0;
    for (size_t i = 0; i < 16; i++) {
        for (size_t j = 0; j < 8; j++) {
            if (b[counter++]) {
                res[i] |= (1 << (7 - j));
            }
        }
    }
    return std::string(res.begin(), res.end());
}

std::bitset<128> linear(std::bitset<128> const& mes) {
    std::bitset<128> result;
    result = permute(mes, table::FP);
    auto X = split<4>(result);
    X[0] = cshl(X[0], 13);
    X[2] = cshl(X[2], 3);
    X[1] = X[1] ^ X[0] ^ X[2];
    X[3] = X[3] ^ X[2] ^ shl(X[0], 3);
    X[1] = cshl(X[1], 1);
    X[3] = cshl(X[3], 7);
    X[0] = X[0] ^ X[1] ^ X[3];
    X[2] = X[2] ^ X[3] ^ shl(X[1], 7);
    X[0] = cshl(X[0], 5);
    X[2] = cshl(X[2], 22);
    result = X[0] + X[1] + X[2] + X[3];
    result = permute(result, table::IP);
    return result;
}

std::bitset<128> inv_linear(std::bitset<128> const& mes) {
    std::bitset<128> result;
    result = permute(mes, table::FP);
    auto X = split<4>(result);
    X[2] = cshr(X[2], 22);
    X[0] = cshr(X[0], 5);
    X[2] = X[2] ^ X[3] ^ shl(X[1], 7);
    X[0] = X[0] ^ X[1] ^ X[3];
    X[3] = cshr(X[3], 7);
    X[1] = cshr(X[1], 1);
    X[3] = X[3] ^ X[2] ^ shl(X[0], 3);
    X[1] = X[1] ^ X[0] ^ X[2];
    X[2] = cshr(X[2], 3);
    X[0] = cshr(X[0], 13);
    result = X[0] + X[1] + X[2] + X[3];
    result = permute(result, table::IP);
    return result;
}

std::bitset<256> convert_key(const std::string& key) {
    std::bitset<256> real;
    if (key.size() == 16) {
        auto block_key = to_block<16>(key);
        copy_and_append(block_key, real);
    } else if (key.size() == 24) {
        auto block_key = to_block<24>(key);
        copy_and_append(block_key, real);
    } else if (key.size() == 32) {
        real = to_block<32>(key);
    } else {
        throw std::runtime_error("Wrong key size: " + std::to_string(key.size()));
    }
    return real;
}

} // namespace serpent
