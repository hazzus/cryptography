#include <fstream>
#include <iostream>

#include "rsa/crypt.h"
#include "rsa/keygen.h"

#include <cassert>

using namespace std;

int main() {
    constexpr size_t SIZE = 1024;
    std::cout << "Hello, this is RSA encoder/decoder utility!" << std::endl;
    std::cout << "Some commands availiable: " << std::endl;
    std::cout << "keygen <keys filename> - generate " << SIZE << " bits size prime numbers, their keys and write them to file" << std::endl;
    std::cout << "crypt <keys filename> <input file> <output file>" << std::endl;
    std::cout << "exit" << std::endl;

    while (true) {
        std::cout << "$> ";
        std::string command;
        std::cin >> command;
        if (command == "keygen") {
            std::string file;
            std::cin >> file;
            std::ofstream key(file, ios::binary);
            auto keypairs = rsa::keygen<SIZE>();
            key << keypairs.first.exponent << std::endl;
            key << keypairs.first.n << std::endl;
            key.close();
            std::ofstream pub(file + ".pub", ios::binary);
            pub << keypairs.second.exponent << std::endl;
            pub << keypairs.second.n << std::endl;
        } else if (command == "crypt") {
            std::string keys, in, out;
            std::cin >> keys >> in >> out;
            std::ifstream kin(keys, ios::binary);

            std::ifstream fin(in, ios::binary);
            std::ofstream fout(out, ios::binary);

            std::string exp, n;
            kin >> exp >> n;
            rsa::key key{rsa::big(exp), rsa::big(n)};

            rsa::crypt<SIZE>(key, fin, fout);
        } else if (command == "exit" || cin.eof()) {
            break;
        } else {
            std::cout << "I dunno such command" << std::endl;
        }
    }

    return 0;
}
