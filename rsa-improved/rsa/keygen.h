#ifndef RSA_KEYGEN_H
#define RSA_KEYGEN_H

#include <algorithm>
#include <array>
#include <iostream>
#include <mpc.h>
#include <utility>

#include "big_integer.h"

namespace rsa {

struct key {
    big exponent;
    big n;
};

static gmp_randstate_t state;
template <size_t N>
big generate_prime() {
    big base(1), res;
    mpz_mul_2exp(base.mpz, base.mpz, 1023);
    mpz_urandomb(res.mpz, state, N - 1);
    mpz_add(res.mpz, res.mpz, base.mpz);
    mpz_nextprime(res.mpz, res.mpz);
    return res;
}

static std::array<big, 3> fermas = {big(65537), big(257), big(17)};
big ferma(big const& x);

big euclid(big a, big b, big& x, big& y);

template <size_t N>
std::pair<key, key> keygen() {
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(nullptr));
    big p, n, q;
    do {
        p = generate_prime<N>();
        q = generate_prime<N>();
        n = p * q;
    } while (size(n) != N * 2);
    big phi = (p - 1) * (q - 1);
    big exp = ferma(phi);
    big x, y;
    euclid(exp, phi, x, y);
    x += phi;
    x %= phi;
    return {{exp, n}, {x, n}};
}

} // namespace rsa

#endif
