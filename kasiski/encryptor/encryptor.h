#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <algorithm>
#include <array>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

struct wrong_message_exception : public std::runtime_error {
    wrong_message_exception(std::string const& cause)
        : std::runtime_error(cause) {}
};

struct encryptor {
    encryptor(std::vector<size_t> const& shifts);
    encryptor(encryptor const&) = default;
    encryptor(encryptor&&) = default;

    std::string encrypt(std::string const& message);
    void encrypt(std::istream& in, std::ostream& out);

    void set_alphabet(std::string const& alphabet);
    void set_shifts(std::vector<size_t> const& shifts);

  private:
    std::vector<size_t> shifts;
};

#endif // ENCRYPTOR_H
