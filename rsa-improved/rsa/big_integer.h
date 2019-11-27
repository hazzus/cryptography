#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <array>
#include <cstring>
#include <mpc.h>
#include <vector>

namespace rsa {

struct big {
    big();
    big(int i);
    big(big const& other);
    explicit big(std::vector<unsigned char> const& str);
    explicit big(std::string const& str);

    big& operator*=(big const& o);
    big& operator-=(big const& o);
    big& operator+=(big const& o);
    big& operator%=(big const& o);
    big& operator/=(big const& o);

    template <size_t N>
    friend big generate_prime();

    friend bool operator<(big const& a, big const& b);
    friend bool operator==(big const& a, big const& b);
    friend big powm(big const& a, big const& b, big const& m);
    friend size_t size(big const& a);

    template <size_t SIZE>
    friend std::string to_string(big const& a);

    // ~big();

  private:
    mpz_t mpz;
};

big operator*(big a, big const& b);
big operator-(big a, big const& b);
big operator+(big a, big const& b);
big operator%(big a, big const& b);
big operator/(big a, big const& b);
bool operator<(big const& a, big const& b);
bool operator==(big const& a, big const& b);

big powm(big const& a, big const& b, big const& m);

template <size_t SIZE>
std::string to_string(const rsa::big& a) {
    if constexpr (SIZE == 0) {
        char* tmp = mpz_get_str(nullptr, 16, a.mpz);
        std::string res = tmp;

        void (*freefunc)(void*, size_t);
        mp_get_memory_functions(nullptr, nullptr, &freefunc);
        freefunc(tmp, std::strlen(tmp) + 1);

        return res;
    } else {
        std::vector<unsigned char> res(SIZE);
        size_t written = 0;
        mpz_export(res.data(), &written, 1, sizeof(unsigned char), 0, 0, a.mpz);
        res.resize(written);
        return std::string(res.begin(), res.end());
    }
}

std::ostream& operator<<(std::ostream& o, big const& a);

size_t size(big const& a);

} // namespace rsa

#endif // BIG_INTEGER_H
