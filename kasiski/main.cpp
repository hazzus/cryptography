#include "encryptor.h"
#include <boost/bimap.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

std::set<size_t> get_distances(std::string const& encrypted) {
    if (encrypted.size() < 3) {
        return {};
    }
    std::set<size_t> distances;
    for (size_t substr_len = 3;; substr_len++) {
        bool was = false;
        for (size_t i = 0; i < encrypted.size() - substr_len; i++) {
            std::string current_substr = encrypted.substr(i, substr_len);
            size_t start = i + 1;
            std::vector<size_t> curs{start};
            while ((start = encrypted.find(current_substr, start)) !=
                   std::string::npos) {
                curs.push_back(start);
                start += 1;
            }
            if (curs.size() > 1) {
                was = true;
                for (size_t i = curs.size() - 1; i > 1; i--) {
                    distances.insert(curs[i] - curs[i - 1]);
                }
            }
        }
        if (!was)
            break;
    }
    return distances;
}

size_t gcd(size_t a, size_t b) {
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No file with message for encryption provided";
        return -1;
    }

    std::ifstream fin(argv[1]);
    // FIXME big files reading
    std::string encrypted((std::istreambuf_iterator<char>(fin)),
                          std::istreambuf_iterator<char>());

    auto res = get_distances(encrypted);
    std::cout << "Distances: ";
    for (size_t i : res) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    std::map<size_t, size_t> gcds;
    for (auto it = res.begin(); it != res.end()--; it++) {
        auto jt = it;
        for (jt++; jt != res.end(); jt++) {
            size_t cur_gcd = gcd(*it, *jt);
            auto gt = gcds.find(cur_gcd);
            if (gt == gcds.end()) {
                gcds[cur_gcd] = 1;
            } else {
                gcds[cur_gcd]++;
            }
        }
    }
    std::vector<std::pair<size_t, size_t>> elements(gcds.begin(), gcds.end());
    std::partial_sort(
        elements.begin(),
        ((5 > elements.size()) ? elements.end() : elements.begin() + 5),
        elements.end(),
        [](std::pair<size_t, size_t> a, std::pair<size_t, size_t> b) {
            return a.second > b.second;
        });
    std::cout << "Most possible columns amounts: ";
    for (auto p = elements.begin();
         p != elements.end() && p != elements.begin() + 5; p++) {
        std::cout << p->first << " ";
    }
    // TODO add konsole-based statistics examination
    std::cout << std::endl;
}
