#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace BinaryReader
{
    int16_t readInt16BE(const std::vector<uint8_t> &buffer, size_t &offset);
    int16_t readInt16LE(const std::vector<uint8_t> &buffer, size_t &offset);

    int32_t readInt32LE(const std::vector<uint8_t> &buffer, size_t &offset);
    int32_t readInt32BE(const std::vector<uint8_t> &buffer, size_t &offset);

    int64_t readInt64LE(const std::vector<uint8_t> &buffer, size_t &offset);
    int64_t readInt64BE(const std::vector<uint8_t> &buffer, size_t &offset);

    std::string readNChars(const std::vector<uint8_t> &buffer, size_t size, size_t &offset);
    std::string readLineTrimmed(const std::vector<uint8_t> &buffer, size_t &offset);
    std::string readStringWithLength(const std::vector<uint8_t> &buffer, size_t &offset);

    std::vector<uint8_t> readBytesWithLength(const std::vector<uint8_t> &buffer, size_t &offset);
    std::vector<uint8_t> readExact(const std::vector<uint8_t> &buffer, uint32_t size, size_t &offset);
    std::vector<uint8_t> readUntil(const std::vector<uint8_t> &buffer, const std::vector<uint8_t> &pattern, size_t &offset);

    size_t sizeUntil(const std::vector<uint8_t> &buffer, const std::vector<uint8_t> &pattern, size_t &offset);
    uint8_t readByte(const std::vector<uint8_t> &buffer, size_t &offset);
};
