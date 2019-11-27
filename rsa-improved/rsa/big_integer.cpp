#include "big_integer.h"

rsa::big::big() { mpz_init(mpz); }

rsa::big::big(int i) { mpz_init_set_si(mpz, i); }

rsa::big::big(const rsa::big& other) { mpz_init_set(mpz, other.mpz); }

rsa::big::big(const std::vector<unsigned char>& str) {
    mpz_init(mpz);
    mpz_import(mpz, str.size(), 1, sizeof(unsigned char), 0, 0, str.data());
}

rsa::big::big(const std::string& str) { mpz_init_set_str(mpz, str.c_str(), 16); }

rsa::big& rsa::big::operator*=(const rsa::big& o) {
    mpz_mul(mpz, mpz, o.mpz);
    return *this;
}

rsa::big& rsa::big::operator-=(const rsa::big& o) {
    mpz_sub(mpz, mpz, o.mpz);
    return *this;
}

rsa::big& rsa::big::operator+=(const rsa::big& o) {
    mpz_add(mpz, mpz, o.mpz);
    return *this;
}

rsa::big& rsa::big::operator%=(const rsa::big& o) {
    mpz_mod(mpz, mpz, o.mpz);
    return *this;
}

rsa::big& rsa::big::operator/=(const rsa::big& o) {
    mpz_div(mpz, mpz, o.mpz);
    return *this;
}

rsa::big rsa::powm(const rsa::big& a, const rsa::big& b, const rsa::big& m) {
    big res;
    mpz_powm(res.mpz, a.mpz, b.mpz, m.mpz);
    return res;
}

rsa::big rsa::operator*(rsa::big a, const rsa::big& b) { return a *= b; }

rsa::big rsa::operator-(rsa::big a, const rsa::big& b) { return a -= b; }

bool rsa::operator<(const rsa::big& a, const rsa::big& b) { return mpz_cmp(a.mpz, b.mpz) <= 0; }

bool rsa::operator==(const rsa::big& a, const rsa::big& b) { return mpz_cmp(a.mpz, b.mpz) == 0; }

rsa::big rsa::operator%(rsa::big a, const rsa::big& b) { return a %= b; }

rsa::big rsa::operator/(rsa::big a, const rsa::big& b) { return a /= b; }

std::ostream& rsa::operator<<(std::ostream& o, const rsa::big& a) { return o << to_string<0>(a); }

size_t rsa::size(const rsa::big& a) { return mpz_sizeinbase(a.mpz, 2); }

rsa::big rsa::operator+(rsa::big a, const rsa::big& b) { return a += b; }
