#ifndef BINARYREADER_HPP
#define BINARYREADER_HPP

#include <fstream>

class BinaryReader {
private:
    void *data;
    size_t size;
    size_t offset;

public:
    BinaryReader(void *data, size_t size);
    ~BinaryReader() = default;

    void Read(void* buffer, size_t size);

    template<typename T>
    T Read() {
        T value;
        Read(&value, sizeof(T));
        return value;
    }

    inline size_t GetSize() const {
        return size;
    }

    inline size_t GetOffset() const {
        return offset;
    }
};

#endif // BINARYREADER_HPP
