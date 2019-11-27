#include "crypt.h"

#include <cassert>

rsa::big rsa::crypt(const rsa::key& key, const rsa::big& m_or_c) {
    if (key.n < m_or_c) {
        std::cerr << "Wrong args in crypt\n";
        std::cerr << "N: " << key.n << std::endl;
        std::cerr << "GOT: " << m_or_c << std::endl;
        assert(false);
    }
    return powm(m_or_c, key.exponent, key.n);
}
