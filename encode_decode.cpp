#include "encode_decode.h"
#include <iostream>
#include <fstream>
#include <vector>

const std::string BASE64_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string input_ex = "hi";
std::string encodeBase64(const std::string &input) {
    std::string encoded;
    int val = 0; // Храним текущие биты текста
    int bits = -6; // Счетчик обработанных битов
    const unsigned int mask = 0x3F; // Маска для 6 бит (0b00111111)

    for (const auto &c : input) {
        val = (val << 8) + c; // "c" в данном случае будет не как буква, а как число
        bits += 8;
        while (bits >= 0) {
            encoded.push_back(BASE64_ALPHABET[(val >> bits) & mask]);
            bits -= 6;
        }
    }


    if (bits > -6) {
        encoded.push_back(BASE64_ALPHABET[((val << 8) >> (bits + 8)) & mask]);
    }

    while (encoded.size() % 4) {
        encoded.push_back('=');
    }

    return encoded;
}

std::string decodeBase64(const std::string &input) {
    std::string decoded;
    std::vector<int> T(256, -1);
    for (int i = 0; i < BASE64_ALPHABET.size(); i++) {
        T[BASE64_ALPHABET[i]] = i;
    }

    int val = 0, bits = -8;
    for (const auto &c : input) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        bits += 6;

        if (bits >= 0) {
            decoded.push_back(char((val >> bits) & 0xFF));
            bits -= 8;
        }
    }

    return decoded;
}

std::string readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "File reading error: " << filename << std::endl;
        exit(1);
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void writeFile(const std::string &filename, const std::string &content) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "File recording error: " << filename << std::endl;
        exit(1);
    }
    file << content;
}
