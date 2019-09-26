#include <fstream>
#include <iostream>

#include "decryptor.h"
#include <map>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./kasiski <encrypted file>" << std::endl;
        return 1;
    }
    std::ifstream fin(argv[1]);
    std::string encrypted{std::istreambuf_iterator<char>(fin),
                          std::istreambuf_iterator<char>()};
    std::string test_part =
        encrypted.substr(0, std::min<size_t>(100, encrypted.size()));
    std::cout << "==== Part of encrypted message ====" << std::endl;
    std::cout << test_part << std::endl;
    std::cout << "===================================" << std::endl;
    decryptor decr;
    auto possible_len = decr.kasiski_exam(encrypted);
    std::cout << "Key lengths in posibility order: ";
    for (size_t i : possible_len) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Using english frequencies" << std::endl;
    std::string eng = "etaoinshrdlcumwfgypbvkjxqz";
    for (size_t len : possible_len) {
        std::cout << "==== Trying key length " << len << " ====" << std::endl;
        std::vector<size_t> replacement(len);

        for (size_t start = 0; start < len; start++) {
            std::cout << "Possible key for position " << start << ": ";
            std::vector<std::pair<size_t, size_t>> counter(26);
            for (size_t i = 0; i < 26; i++) {
                counter[i].first = i;
            }
            for (size_t x = start; x < encrypted.size(); x += len) {
                counter[encrypted[x] - 'a'].second++;
            }
            std::sort(counter.begin(), counter.end(),
                      [](auto a, auto b) { return a.second > b.second; });
            std::vector<size_t> dist_count(26);
            for (size_t i = 0; i < 26; i++) {
                int diff = (int)counter[i].first - (int)(eng[i] - 'a');
                if (diff < 0)
                    diff += 26;
                dist_count[diff]++;
            }
            size_t maxi = 0;
            for (size_t i = 1; i < 26; i++) {
                if (dist_count[maxi] < dist_count[i]) {
                    maxi = i;
                }
            }
            std::cout << maxi;
            replacement[start] = maxi;
            std::cout << std::endl;
        }
        std::cout << "==== Decrypted part of encrypted message ===="
                  << std::endl;
        for (size_t i = 0; i < test_part.size(); i++) {
            int a = (int)(test_part[i] - 'a') - (int)replacement[i % len];
            if (a < 0)
                a += 26;
            std::cout << (char)(a + 'a');
        }
        std::cout << std::endl;
        std::cout << "Is encrypted part ok? [y/n] (Else next possible key "
                     "length will be used)"
                  << std::endl;
        char c;
        std::cin >> c;
        if (c == 'y') {
            std::cout << "Answer written to answer.txt";
            std::ofstream fout("answer.txt");
            for (size_t i = 0; i < encrypted.size(); i++) {
                int a = (int)(test_part[i] - 'a') - (int)replacement[i % len];
                if (a < 0)
                    a += 26;
                fout << (char)(a + 'a');
            }
            break;
        }
    }

    return 0;
}
