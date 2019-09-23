#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <vector>

struct wrong_message_exception : public std::runtime_error {
    wrong_message_exception(std::string const& cause)
        : std::runtime_error(cause) {}
};

struct encryptor {
    encryptor(std::vector<size_t> const& shifts,
              std::string const& alphabet = "abcdefghijkllmnopqrstuvwxyz");
    encryptor(encryptor const&) = default;
    encryptor(encryptor&&) = default;

    std::string encrypt(std::string const& message);
    std::vector<size_t> encrypt(std::vector<size_t> const& message);

    void set_alphabet(std::string const& alphabet);
    void set_shifts(std::vector<size_t> const& shifts);

  private:
    std::vector<size_t> __convert(std::string const& message);
    void __set_reversed_alphabet();

    std::string alphabet;
    std::array<size_t, 256> reversed_alphabet;
    std::vector<size_t> shifts;
};

#endif // ENCRYPTOR_H
