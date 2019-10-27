#ifndef SERPENT_ENCODER_H
#define SERPENT_ENCODER_H

#include "tables.hpp"
#include "utils.hpp"
#include <bitset>
#include <string>

namespace serpent {

struct encoder {
    template <size_t num> encoder(const std::bitset<num>& key) {
        std::bitset<256> real;
        copy_and_append(key, real);
        generate_keys(real, keys);
    }

    encoder(std::string const& key);

    std::bitset<128> encode(std::bitset<128> const& message);

    std::string encode(std::string message);

    void encode(std::istream const& in, std::ostream const& out);

  private:
    std::array<std::bitset<128>, 33> keys;
};

} // namespace serpent

#endif
