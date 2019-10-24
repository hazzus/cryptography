#include "encryptor.h"

encryptor::encryptor(std::vector<size_t> const& shifts) : shifts(shifts) {}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

std::string encryptor::encrypt(const std::string& message) {
    std::string res;
    for (size_t i = 0, counter = 0; i < message.size(); i++) {
        if (!is_alpha(message[i]))
            continue;
        char c_rep = ((message[i] | 0x20) - 'a');
        size_t cur_shift = shifts[counter++ % shifts.size()];
        res.push_back((c_rep + cur_shift) % 26 + 'a');
    }
    return res;
}

void encryptor::encrypt(std::istream& in, std::ostream& out) {
    while (!in.eof()) {
        std::string block;
        in.read(const_cast<char*>(block.data()), shifts.size() * 512);
        std::string encoded = encrypt(block);
        out << encoded;
    }
}
