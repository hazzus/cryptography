#include <algorithm>
#include <array>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

uint32_t add(uint32_t a, uint32_t b) { return a + b; }

void round(std::array<uint32_t, 32>& state) {
    // 1. add
    for (uint32_t i = 0; i < 16; i++) {
        state[16 + i] = add(state[16 + i], state[i]);
    }
    // 2. rotate 7
    for (uint32_t i = 0; i < 16; i++) {
        state[i] = (state[i] << 7) | (state[i] >> 25);
    }
    // 3. swap
    for (uint32_t i = 0; i < 8; i++) {
        std::swap(state[i], state[i + 8]);
    }
    // 4. xor
    for (uint32_t i = 0; i < 16; i++) {
        state[i] ^= state[16 + i];
    }
    // 5. swap
    for (uint32_t i = 0; i < 4; i++) {
        std::swap(state[16 + i * 4], state[18 + i * 4]);
        std::swap(state[17 + i * 4], state[19 + i * 4]);
    }
    // 6. add == 1
    for (uint32_t i = 0; i < 16; i++) {
        state[16 + i] = add(state[16 + i], state[i]);
    }
    // 7. rotate 11
    for (uint32_t i = 0; i < 16; i++) {
        state[i] = (state[i] << 11) | (state[i] >> 21);
    }
    // 8. swap
    for (uint32_t i = 0; i < 2; i++) {
        for (uint32_t j = 0; j < 4; j++) {
            std::swap(state[i * 8 + j], state[i * 8 + j + 4]);
        }
    }
    // 9. xor
    for (uint32_t i = 0; i < 16; i++) {
        state[i] ^= state[16 + i];
    }
    // 10. swap
    for (uint32_t i = 0; i < 8; i++) {
        std::swap(state[16 + i * 2], state[i * 2 + 17]);
    }
}

template <uint32_t I, uint32_t R, uint32_t B, uint32_t F, uint32_t H>
void cubehash(const uint8_t* in, uint8_t* out, uint64_t len) {
    std::array<uint32_t, 32> state_4byte;
    uint8_t* state_1byte = reinterpret_cast<uint8_t*>(state_4byte.data());
    // init

    for (size_t i = 0; i < 32; i++) {
        state_4byte[i] = 0;
    }
    state_4byte[0] = H / 8;
    state_4byte[1] = B;
    state_4byte[2] = R;

    for (uint32_t i = 0; i < I; i++) {
        round(state_4byte);
    }

    auto nlen = B * (len / B + 1);
    std::vector<uint8_t> nin(nlen);
    std::memcpy(nin.data(), in, len);
    nin[len] = 1 << 7;

    // working
    for (uint64_t i = 0; i < nlen; i += B) {
        // mix message with state
        for (uint32_t j = 0; j < B; j++) {
            state_1byte[j] ^= nin[i + j];
        }
        // roundit
        for (uint32_t i = 0; i < R; i++) {
            round(state_4byte);
        }
    }

    // final
    state_4byte[31] ^= 1;
    for (uint32_t i = 0; i < F; i++) {
        round(state_4byte);
    }

    // writing result
    std::memcpy(out, state_1byte, H / 8);
}

template <uint32_t I, uint32_t R, uint32_t B, uint32_t F, uint32_t H>
std::string cubehash(std::string const& s) {
    uint8_t res[H / 8];
    cubehash<I, R, B, F, H>(reinterpret_cast<const uint8_t*>(s.c_str()), res,
                            s.length());
    std::stringstream ss;
    for (size_t i = 0; i < H / 8; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<uint32_t>(res[i]);
    }
    return ss.str();
}

template <uint32_t I, uint32_t R, uint32_t B, uint32_t F, uint32_t H>
void test(std::string const& mes, std::string const& exp) {
    auto got = cubehash<I, R, B, F, H>(mes);
    if (got != exp) {
        throw std::runtime_error("Test failed \n" + exp + "\n" + got);
    }
}

void test_them_all() {
    test<80, 8, 1, 80, 512>(
        "Hello",
        "7ce309a25e2e1603ca0fc369267b4d43f0b1b744ac45d6213ca08e75675664448e2"
        "f62fdbf7bbd637ce40fc293286d75b9d09e8dda31bd029113e02ecccfd39b");

    test<80, 8, 1, 80, 512>(
        "hello",
        "01ee7f4eb0e0ebfdb8bf77460f64993faf13afce01b55b0d3d2a63690d25010f712710"
        "9455a7c143ef12254183e762b15575e0fcc49c79a0471a970ba8a66638");

    test<160, 16, 32, 160, 512>(
        "", "4a1d00bbcfcb5a9562fb981e7f7db3350fe2658639d948b9d57452c22328bb32"
            "f468b072208450bad5ee178271408be0b16e5633ac8a1e3cf9864cfbfc8e043a");

    test<80, 8, 1, 80, 512>(
        "", "90bc3f2948f7374065a811f1e47a208a53b1a2f3be1c0072759ed49c9c6c7f28"
            "f26eb30d5b0658c563077d599da23f97df0c2c0ac6cce734ffe87b2e76ff7294");

    test<10, 1, 1, 10, 512>(
        "", "3f917707df9acd9b94244681b3812880e267d204f1fdf795d398799b584fa8f1f4"
            "a0b2dbd52fd1c4b6c5e020dc7a96192397dd1bce9b6d16484049f85bb71f2f");

    test<160, 16, 32, 160, 256>(
        "", "44c6de3ac6c73c391bf0906cb7482600ec06b216c7c54a2a8688a6a42676577d");

    test<80, 8, 1, 80, 256>(
        "", "38d1e8a22d7baac6fd5262d83de89cacf784a02caa866335299987722aeabc59");

    test<10, 1, 1, 10, 256>(
        "", "80f72e07d04ddadb44a78823e0af2ea9f72ef3bf366fd773aa1fa33fc030e5cb");

    auto message = "The quick brown fox jumps over the lazy dog";
    test<160, 16, 32, 160, 512>(
        message,
        "bdba44a28cd16b774bdf3c9511def1a2baf39d4ef98b92c27cf5e37beb8990b7"
        "cdb6575dae1a548330780810618b8a5c351c1368904db7ebdf8857d596083a86");

    test<80, 8, 1, 80, 512>(
        message,
        "ca942b088ed9103726af1fa87b4deb59e50cf3b5c6dcfbcebf5bba22fb39a6be"
        "9936c87bfdd7c52fc5e71700993958fa4e7b5e6e2a3672122475c40f9ec816ba");
}

int main(int argc, char* argv[]) {

    try {
        test_them_all();
    } catch (std::runtime_error const& e) {
        std::cout << e.what();
        return -1;
    }

    std::cout << "All tests passed!" << std::endl;

    if (argc < 2)
        return -1;

    std::cout << "CubeHash 80+8/1+80-512 of \"" << argv[1]
              << "\" is:" << std::endl;
    std::cout << cubehash<80, 8, 1, 80, 512>(argv[1]) << std::endl;
}
