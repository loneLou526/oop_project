#ifndef ENCODE_DECODE_H
#define ENCODE_DECODE_H

#include <string>

// Прототипы функций
std::string encodeBase64(const std::string &input);
std::string decodeBase64(const std::string &input);
std::string readFile(const std::string &filename);
void writeFile(const std::string &filename, const std::string &content);

#endif // ENCODE_DECODE_H
