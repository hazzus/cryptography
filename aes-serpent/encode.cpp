#include <iostream>

#include "serpent/encoder.h"

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
    serpent::encoder enc(argv[3]);
    std::ifstream in(argv[1], std::ios::binary);
    std::ofstream out(argv[2], std::ios::binary);
    while (!in.eof()) {
        std::vector<unsigned char> buffer(256);
        in.read(reinterpret_cast<char*>(buffer.data()), 256);
        buffer.resize(in.gcount());
        out << enc.encode(std::string{buffer.begin(), buffer.end()});
    }
    return 0;
}
