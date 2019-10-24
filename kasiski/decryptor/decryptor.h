#ifndef DECRYPTOR_H
#define DECRYPTOR_H

#include <algorithm>
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

class decryptor {
  public:
    decryptor(std::string const& alphabet = "abcdefghijklmnopqrstuvwxyz");

    static std::vector<size_t> kasiski_exam(std::string const& encrypted);

  private:
    static std::unordered_map<std::string, std::vector<size_t>>
    get_positions(std::string const& encrypted);

    static std::vector<size_t> get_distances(
        std::unordered_map<std::string, std::vector<size_t>> const& positions);

    std::string alphabet;
};

#endif // DECRYPTOR_H
