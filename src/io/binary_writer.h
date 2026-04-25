#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class BinWriter
{
    std::vector<uint8_t> buffer;

public:
    BinWriter() = default;

    const size_t size() const { return buffer.size(); }
    const uint8_t *data() const { return buffer.data(); }

    void writeInt16LE(int16_t value);
    void writeInt16BE(int16_t value);

    void writeInt32LE(int32_t value);
    void writeInt32BE(int32_t value);

    void writeInt64LE(int64_t value);
    void writeInt64BE(int64_t value);

    void writeString(const std::string &text);
    void writeString(const std::string &text, char lineEnd);
    void writeBytes(const std::vector<uint8_t> &bytes);
    void writeBytes(const BinWriter &other);
    void writeByte(uint8_t byte);

    void saveToFile(const std::string &path);
    void saveToBuffer(std::vector<uint8_t> &outBuffer);

private:
    uint8_t *resize(size_t size);
};