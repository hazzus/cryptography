#include "encryptor.h"

encryptor::encryptor(std::vector<size_t> const& shifts,
                     std::string const& alphabet)
    : alphabet(alphabet), shifts(shifts) {
    __set_reversed_alphabet();
}

std::string encryptor::encrypt(const std::string& message) {
    std::vector<std::size_t> symbols = __convert(message);
    auto enc_nums = encrypt(symbols);
    std::string result;
    std::for_each(
        enc_nums.begin(), enc_nums.end(),
        [this, &result](size_t index) { result.push_back(alphabet[index]); });
    return result;
}

std::vector<size_t> encryptor::encrypt(const std::vector<size_t>& message) {
    std::vector<std::size_t> result;
    for (std::size_t i = 0; i < message.size(); i++) {
        if (message[i] >= alphabet.size()) {
            throw wrong_message_exception(
                "Illigal char, not in alphabet. Index in message: " +
                std::to_string(i));
        }
        result.push_back((message[i] + shifts[i % shifts.size()]) %
                         alphabet.size());
    }
    return result;
}

std::vector<size_t> encryptor::__convert(const std::string& message) {
    std::vector<std::size_t> symbols;
    std::transform(message.begin(), message.end(), std::back_inserter(symbols),
                   [=](char sym) {
                       if (sym != ' ' && sym != '\n')
                           return reversed_alphabet[static_cast<size_t>(sym)];
                   });
    return symbols;
}

void encryptor::__set_reversed_alphabet() {
    reversed_alphabet.fill(alphabet.size());
    for (size_t i = 0; i < alphabet.size(); i++) {
        reversed_alphabet[static_cast<size_t>(alphabet[i])] = i;
    }
}
