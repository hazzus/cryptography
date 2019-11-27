#include "keygen.h"
namespace rsa {

big euclid(big a, big b, big& x, big& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    big x1, y1;
    big d = euclid(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

big ferma(const big& x) {
    return *std::find_if(fermas.begin(), fermas.end(), [x](auto const& a) { return a < x; });
}

} // namespace rsa
