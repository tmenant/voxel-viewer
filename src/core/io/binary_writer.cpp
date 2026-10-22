#include "binary_writer.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "file.h"

uint8_t *BinWriter::resize(size_t size)
{
    size_t currentSize = buffer.size();
    buffer.resize(currentSize + size);
    return &buffer[currentSize];
}

void BinWriter::writeInt16LE(int16_t value)
{
    uint8_t *ptr = resize(2);

    ptr[0] = static_cast<uint8_t>(value);
    ptr[1] = static_cast<uint8_t>(value >> 8);
}

void BinWriter::writeInt16BE(int16_t value)
{
    uint8_t *ptr = resize(2);

    ptr[0] = static_cast<uint8_t>(value >> 8);
    ptr[1] = static_cast<uint8_t>(value);
}

void BinWriter::writeInt32LE(int32_t value)
{
    uint8_t *ptr = resize(4);

    ptr[0] = static_cast<uint8_t>(value);
    ptr[1] = static_cast<uint8_t>(value >> 8);
    ptr[2] = static_cast<uint8_t>(value >> 16);
    ptr[3] = static_cast<uint8_t>(value >> 24);
}

void BinWriter::writeInt32BE(int32_t value)
{
    uint8_t *ptr = resize(4);

    ptr[0] = static_cast<uint8_t>(value >> 24);
    ptr[1] = static_cast<uint8_t>(value >> 16);
    ptr[2] = static_cast<uint8_t>(value >> 8);
    ptr[3] = static_cast<uint8_t>(value);
}

void BinWriter::writeInt64LE(int64_t value)
{
    uint8_t *ptr = resize(8);

    ptr[0] = static_cast<uint8_t>(value);
    ptr[1] = static_cast<uint8_t>(value >> 8);
    ptr[2] = static_cast<uint8_t>(value >> 16);
    ptr[3] = static_cast<uint8_t>(value >> 24);
    ptr[4] = static_cast<uint8_t>(value >> 32);
    ptr[5] = static_cast<uint8_t>(value >> 40);
    ptr[6] = static_cast<uint8_t>(value >> 48);
    ptr[7] = static_cast<uint8_t>(value >> 56);
}

void BinWriter::writeInt64BE(int64_t value)
{
    uint8_t *ptr = resize(8);

    ptr[0] = static_cast<uint8_t>(value >> 56);
    ptr[1] = static_cast<uint8_t>(value >> 48);
    ptr[2] = static_cast<uint8_t>(value >> 40);
    ptr[3] = static_cast<uint8_t>(value >> 32);
    ptr[4] = static_cast<uint8_t>(value >> 24);
    ptr[5] = static_cast<uint8_t>(value >> 16);
    ptr[6] = static_cast<uint8_t>(value >> 8);
    ptr[7] = static_cast<uint8_t>(value);
}

void BinWriter::writeString(const std::string &text)
{
    uint8_t *ptr = resize(text.size());
    std::memcpy(ptr, text.data(), text.size());
}

void BinWriter::writeString(const std::string &text, char lineEnd)
{
    uint8_t *ptr = resize(text.size() + 1);
    std::memcpy(ptr, text.data(), text.size());
    ptr[text.size()] = static_cast<uint8_t>(lineEnd);
}

void BinWriter::writeBytes(const std::vector<uint8_t> &bytes)
{
    uint8_t *ptr = resize(bytes.size());
    std::memcpy(ptr, bytes.data(), bytes.size());
}

void BinWriter::writeByte(uint8_t byte)
{
    buffer.push_back(byte);
}

void BinWriter::writeBytes(const BinWriter &other)
{
    writeBytes(other.buffer);
}

void BinWriter::saveToFile(const std::string &path)
{
    File::writeAllBytes(buffer, path);
}

void BinWriter::saveToBuffer(std::vector<uint8_t> &outBuffer)
{
    outBuffer.resize(buffer.size());
    std::memcpy(outBuffer.data(), buffer.data(), buffer.size());
}
