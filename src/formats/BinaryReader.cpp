#include "BinaryReader.h"

BinaryReader::BinaryReader(void *data, size_t size)
    : data(data), size(size), offset(0) {}

void BinaryReader::Read(void* buffer, size_t size) {
    if (offset + size > this->size) {
        throw std::runtime_error("BinaryReader::Read: Attempted to read past end of buffer");
    }

    if (buffer) {
        memcpy(buffer, static_cast<uint8_t*>(data) + offset, size);
    }

    offset += size;
}
