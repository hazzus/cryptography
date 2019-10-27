#ifndef SERPENT_DECODER_H
#define SERPENT_DECODER_H

#include "tables.hpp"
#include "utils.hpp"
#include <bitset>
#include <string>

namespace serpent {

struct decoder {
    template <size_t num> decoder(const std::bitset<num>& key) {
        std::bitset<256> real;
        copy_and_append(key, real);
        generate_keys(real, keys);
    }

    decoder(std::string const& key);

    std::bitset<128> decode(std::bitset<128> const& message);

    std::string decode(std::string message);

    void decode(std::istream const& in, std::ostream const& out);

  private:
    std::array<std::bitset<128>, 33> keys;
};

} // namespace serpent

#endif
