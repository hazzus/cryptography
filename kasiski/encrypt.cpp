#include <fstream>
#include <iostream>
#include <vector>

#include "encryptor.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: encrypt <input file> <output file> <shifts>...";
        return -1;
    }
    std::vector<size_t> shifts;
    for (int i = 3; i < argc; i++) {
        shifts.push_back(std::stoul(argv[i]));
    }
    encryptor enc(shifts);
    std::ifstream fin(argv[1]);
    // FIXME big files reading
    std::string text((std::istreambuf_iterator<char>(fin)),
                     std::istreambuf_iterator<char>());
    std::string encrypted;
    try {
        encrypted = enc.encrypt(text);
    } catch (wrong_message_exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    // std::cout << "Encrypted version: " << std::endl;
    // std::cout << encrypted << std::endl;

    std::ofstream fout(argv[2]);
    fout << encrypted;
    return 0;
}
