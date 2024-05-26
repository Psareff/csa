#ifndef UTILITY_H_
#define UTILITY_H_

#include <cstdint>
#include <string>
#include <vector>

namespace utils
{
    std::string EncodeBase64(const std::string &txt);

    std::string DecodeBase64(const std::string &txt);

    std::vector<std::string> Split(const std::string &s, const char *delim);
}

#endif // UTILITY_H_
