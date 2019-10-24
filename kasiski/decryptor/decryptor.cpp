#include "decryptor.h"

decryptor::decryptor(std::string const& alphabet) : alphabet(alphabet) {}

std::vector<size_t> decryptor::kasiski_exam(const std::string& encrypted) {
    auto distances = get_distances(get_positions(encrypted));
    std::vector<std::pair<size_t, size_t>> delim_count(23);
    for (size_t del = 3; del < 26; del++) {
        delim_count[del - 3].first = del;
        for (size_t i : distances) {
            if (i % del == 0) {
                delim_count[del - 3].second++;
            }
        }
    }
    std::sort(delim_count.begin(), delim_count.end(),
              [](auto a, auto b) { return a.second > b.second; });
    std::vector<size_t> result;
    std::transform(delim_count.begin(), delim_count.end(),
                   std::back_inserter(result), [](auto p) { return p.first; });
    return result;
}

std::unordered_map<std::string, std::vector<size_t>>
decryptor::get_positions(const std::string& encrypted) {
    std::unordered_map<std::string, std::vector<size_t>> res;
    for (size_t xgram_len = 2; xgram_len < 4; xgram_len++) {
        for (size_t i = 0; i < encrypted.size() - xgram_len; i++) {
            std::string current = encrypted.substr(i, xgram_len);
            auto it = res.find(current);
            if (it == res.end()) {
                res.insert({current, {i}});
            } else {
                res[current].push_back(i);
            }
        }
    }
    return res;
}

std::vector<size_t> decryptor::get_distances(
    const std::unordered_map<std::string, std::vector<size_t>>& positions) {
    std::vector<size_t> result;
    for (auto const& x : positions) {
        for (size_t i = 0; i < x.second.size() - 1; i++) {
            result.push_back(x.second[i + 1] - x.second[i]);
        }
    }
    return result;
}
