#ifndef RSA_CRYPT_H
#define RSA_CRYPT_H

#include "big_integer.h"
#include "keygen.h"

#include <iomanip>
#include <sstream>

namespace rsa {

big crypt(key const& key, big const& m);

template <size_t S>
std::array<unsigned char, S> crypt(key const& key, std::array<unsigned char, S> const& s) {
    std::vector<unsigned char> copy(s.begin(), s.end());
    std::array<unsigned char, S> result;
    std::string x = to_string<S>(crypt(key, big(copy)));
    std::copy(x.begin(), x.end(), result.begin());
    return result;
}

template <size_t S>
void crypt(key const& key, std::istream& in, std::ostream& out) {
    constexpr size_t SIZE = S / 4;
    while (true) {
        std::vector<unsigned char> data(SIZE);
        in.read(reinterpret_cast<char*>(data.data()), SIZE);
        data.resize(in.gcount());
        if (data.empty()) {
            break;
        }
        std::string a = to_string<SIZE>(big(data));
        out << to_string<SIZE>(crypt(key, big(data)));
    }
}

} // namespace rsa

#endif
