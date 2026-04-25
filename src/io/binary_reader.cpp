#include <algorithm>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>

#include "binary_reader.h"

std::string BinaryReader::readNChars(const std::vector<uint8_t> &buffer, size_t size, size_t &offset)
{
    if (offset + size > buffer.size())
        throw std::runtime_error("buffer is too small");

    std::string result(reinterpret_cast<const char *>(buffer.data() + offset), size);
    offset += size;
    return result;
}

int16_t BinaryReader::readInt16LE(const std::vector<uint8_t> &buffer, size_t &offset)
{
    if (offset + 2 > buffer.size())
        throw std::runtime_error("buffer is too small");

    const uint8_t *data = reinterpret_cast<const uint8_t *>(buffer.data());

    uint16_t result = static_cast<uint16_t>(data[offset]) | (static_cast<uint16_t>(data[offset + 1]) << 8);

    offset += 2;

    return static_cast<int16_t>(result);
}

int16_t BinaryReader::readInt16BE(const std::vector<uint8_t> &buffer, size_t &offset)
{
    if (offset + 2 > buffer.size())
        throw std::runtime_error("buffer is too small");

    const uint8_t *data = reinterpret_cast<const uint8_t *>(buffer.data());

    uint16_t result = (static_cast<uint16_t>(data[offset]) << 8) | static_cast<uint16_t>(data[offset + 1]);

    offset += 2;

    return static_cast<int16_t>(result);
}

int32_t BinaryReader::readInt32LE(const std::vector<uint8_t> &buffer, size_t &offset)
{
    if (offset + 4 > buffer.size())
        throw std::runtime_error("buffer is too small");

    const uint8_t *data = reinterpret_cast<const uint8_t *>(buffer.data());

    uint32_t result = static_cast<uint32_t>(data[offset])
        | (static_cast<uint32_t>(data[offset + 1]) << 8)
        | (static_cast<uint32_t>(data[offset + 2]) << 16)
        | (static_cast<uint32_t>(data[offset + 3]) << 24);

    offset += 4;

    return static_cast<int32_t>(result);
}

int32_t BinaryReader::readInt32BE(const std::vector<uint8_t> &buffer, size_t &offset)
{
    if (offset + 4 > buffer.size())
        throw std::runtime_error("buffer is too small");

    const uint8_t *data = reinterpret_cast<const uint8_t *>(buffer.data());

    uint32_t result =
        (static_cast<uint32_t>(data[offset]) << 24)
        | (static_cast<uint32_t>(data[offset + 1]) << 16)
        | (static_cast<uint32_t>(data[offset + 2]) << 8)
        | (static_cast<uint32_t>(data[offset + 3]));

    offset += 4;

    return static_cast<int32_t>(result);
}

int64_t BinaryReader::readInt64LE(const std::vector<uint8_t> &buffer, size_t &offset)
{
    if (offset + 8 > buffer.size())
        throw std::runtime_error("buffer is too small");

    const uint8_t *data = reinterpret_cast<const uint8_t *>(buffer.data());

    uint64_t result =
        static_cast<uint64_t>(data[offset])
        | (static_cast<uint64_t>(data[offset + 1]) << 8)
        | (static_cast<uint64_t>(data[offset + 2]) << 16)
        | (static_cast<uint64_t>(data[offset + 3]) << 24)
        | (static_cast<uint64_t>(data[offset + 4]) << 32)
        | (static_cast<uint64_t>(data[offset + 5]) << 40)
        | (static_cast<uint64_t>(data[offset + 6]) << 48)
        | (static_cast<uint64_t>(data[offset + 7]) << 56);

    offset += 8;

    return static_cast<int64_t>(result);
}

int64_t BinaryReader::readInt64BE(const std::vector<uint8_t> &buffer, size_t &offset)
{
    if (offset + 8 > buffer.size())
        throw std::runtime_error("buffer is too small");

    const uint8_t *data = reinterpret_cast<const uint8_t *>(buffer.data());

    uint64_t result =
        (static_cast<uint64_t>(data[offset]) << 56)
        | (static_cast<uint64_t>(data[offset + 1]) << 48)
        | (static_cast<uint64_t>(data[offset + 2]) << 40)
        | (static_cast<uint64_t>(data[offset + 3]) << 32)
        | (static_cast<uint64_t>(data[offset + 4]) << 24)
        | (static_cast<uint64_t>(data[offset + 5]) << 16)
        | (static_cast<uint64_t>(data[offset + 6]) << 8)
        | (static_cast<uint64_t>(data[offset + 7]));

    offset += 8;

    return static_cast<int64_t>(result);
}

uint8_t BinaryReader::readByte(const std::vector<uint8_t> &buffer, size_t &offset)
{
    if (offset + 1 > buffer.size())
    {
        throw std::runtime_error("buffer is too small");
    }

    return buffer[offset++];
}

std::string BinaryReader::readLineTrimmed(const std::vector<uint8_t> &buffer, size_t &offset)
{
    for (size_t i = offset; i < buffer.size(); i++)
    {
        if (buffer[i] != '\n')
            continue;

        size_t size = i - offset;

        std::string line(reinterpret_cast<const char *>(&buffer[offset]), size);
        offset += size + 1;

        return line;
    }

    throw std::runtime_error("line terminator not found");
}

std::string BinaryReader::readStringWithLength(const std::vector<uint8_t> &buffer, size_t &offset)
{
    int32_t size = readInt32LE(buffer, offset);

    if (offset + size > buffer.size())
        throw std::runtime_error("buffer too small");

    std::string line(reinterpret_cast<const char *>(&buffer[offset]), size);
    offset += size;

    return line;
}

std::vector<uint8_t> BinaryReader::readBytesWithLength(const std::vector<uint8_t> &buffer, size_t &offset)
{
    int32_t size = readInt32LE(buffer, offset);

    if (offset + size > buffer.size())
        throw std::runtime_error("buffer too small");

    std::vector<uint8_t> result(size);
    std::memcpy(result.data(), buffer.data() + offset, size);
    offset += size;

    return result;
}

std::vector<uint8_t> BinaryReader::readExact(const std::vector<uint8_t> &buffer, uint32_t size, size_t &offset)
{
    if (offset + size > buffer.size())
        throw std::runtime_error("buffer is too small");

    std::vector<uint8_t> result(size);
    std::memcpy(result.data(), buffer.data() + offset, size);
    offset += size;

    return result;
}

std::vector<uint8_t> BinaryReader::readUntil(const std::vector<uint8_t> &buffer, const std::vector<uint8_t> &pattern, size_t &offset)
{
    auto it = std::search(buffer.begin() + offset, buffer.end(), pattern.begin(), pattern.end());

    if (it == buffer.end())
    {
        throw std::runtime_error("Pattern not found.");
    }

    std::size_t pos = std::distance(buffer.begin(), it);
    std::vector<uint8_t> slice(buffer.begin() + offset, buffer.begin() + pos);

    offset = pos + pattern.size();

    return slice;
}

size_t BinaryReader::sizeUntil(const std::vector<uint8_t> &buffer, const std::vector<uint8_t> &pattern, size_t &offset)
{
    auto it = std::search(buffer.begin() + offset, buffer.end(), pattern.begin(), pattern.end());

    if (it == buffer.end())
    {
        throw std::runtime_error("Pattern not found.");
    }

    return std::distance(buffer.begin(), it) - offset;
}
