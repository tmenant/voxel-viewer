#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace File
{
    std::vector<uint8_t> readAllBytes(const std::string &path);
    std::string readText(const char *filePath);

    void writeAllBytes(const std::vector<uint8_t> &buffer, const std::string &path);
}