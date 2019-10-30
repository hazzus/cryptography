#include <iostream>

#include "serpent/decoder.h"

#include <cstring>
#include <fstream>

using namespace serpent;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./serpent-encode <input file> <output file> <key>";
        return -1;
    }
    size_t key_len = std::strlen(argv[3]);
    if (key_len != 16 && key_len != 24 && key_len != 32) {
        std::cerr << "Key length " << key_len << " is not allowed. Allowed key lengths: 16, 24, 32" << std::endl;
        return -1;
    }
    serpent::decoder dec(argv[3]);
    std::ifstream in(argv[1], std::ios::binary);
    std::ofstream out(argv[2], std::ios::binary);
    dec.decode(in, out);
    return 0;
}
