//
// Created by exozg on 06/05/2025.
//

#ifndef ISTREAM_H
#define ISTREAM_H
#include <cstdint>
#include <vector>

#include "Constants.h"

namespace EA::IO {
    class IStream {
        uint32_t refCount = 0;

    public:
        virtual ~IStream() = default;

        virtual uint32_t AddRef() {
            return ++refCount;
        }

        virtual uint32_t Release() {
            const uint32_t newCount = --refCount;

            if (newCount == 0) {
                delete this;
            }

            return newCount;
        }

        /// Returns the type of the stream, which is different for each Stream subclass.
        /// This function can be used for run-time type identification. A type of zero means
        /// the type is unknown or invalid.
        [[nodiscard]] virtual uint32_t GetType() const = 0;

        /// Returns one of enum AccessFlags.
        /// This function also tells you if the stream is open, as a return value
        /// of zero means the stream is not open. It is not allowed that a stream
        /// be open with no type of access.
        [[nodiscard]] virtual AccessFlags GetAccessFlags() const = 0;

        /// Returns the error state of the stream.
        /// Returns FileError::Success if OK, else an error code.
        /// This function is similar to 'errno' or a 'get last error' facility.
        [[nodiscard]] virtual FileError GetState() const = 0;

        /// Closes the stream and releases resources associated with it.
        /// Returns true upon success, else false.
        /// If the return value is false, GetState will give the error code.
        /// If an IStream encounters an error during operations on an open
        /// stream, it is guaranteed that you can safely call the Close function
        /// on the stream.
        virtual bool Close() = 0;

        /// Returns the size of the stream, which is not the same as the size
        /// of bytes remaining to be read from the stream. Returns (size_type)-2
        /// if the stream is of indeterminate size. Returns (size_type)-1
        /// (a.k.a. kSizeTypeError) upon error.
        [[nodiscard]] virtual size_t GetSize() const = 0;

        /// Sets the size of the stream, if possible. It is debatable whether this
        /// function should be present in IStream or only in subclasses of
        /// StreamBase which are writable. For consistency with GetSize, we put
        /// the function here. But also consider that a SetSize function is not
        /// necessarily a data writing function, depending on the stream implementation.
        /// @param size
        virtual bool SetSize(size_t size) = 0;

        /// Gets the current read/write position within the stream. The read and
        /// write positions of a stream must be the same value; you cannot have a
        /// read position that is different from a write position. However, a
        /// Stream subclass can provide such functionality if needed.
        /// Returns -1 upon error.
        /// @param positionType
        [[nodiscard]] virtual size_t GetPosition(PositionType positionType = PositionType::Begin) const = 0;

        /// Sets the read/write position of the stream. If the specified position is
        /// beyond the size of a fixed stream, the position is set to the end of the stream.
        /// A writable stream subclass may provide a policy whereby setting the position
        /// beyond the end of the stream results in an increase in the stream size.
        /// @param distance
        /// @param positionType
        virtual bool SetPosition(int distance, PositionType positionType = PositionType::Begin) = 0;

        /// Returns the number of bytes available for reading.
        /// Returns (size_type)-1 (a.k.a. kSizeTypeError) upon error.
        /// This function is non-blocking; it should return immediately.
        [[nodiscard]] virtual size_t GetAvailable() const = 0;

        /// Reads bytes from the stream given by the input count 'nSize'.
        /// If less than nSize bytes are available, then those bytes will be read.
        /// Returns the number of bytes read. A return value of zero means that there
        /// were no bytes to be read or no bytes were requested to be read.
        /// A return value of zero means the end of file was reached.
        /// A return value > 0 but < 'nSize' is possible, and it does not
        /// necessarily mean that the end of the file was reached.
        /// Returns (size_type)-1 (a.k.a. kSizeTypeError) if there was an error.
        /// You can use this return value or IStream::GetState to determine the error.
        /// Input size values equal to (size_type)-1 (a.k.a. kSizeTypeError) are not valid input.
        /// Upon error, the stream pointer is at the position it was upon the error occurrence.
        /// @param pData
        /// @param nSize
        virtual size_t Read(void *pData, size_t nSize) = 0;

        /// Flush any non-empty stream write buffers.
        /// If the return value is false, GetState will give the error code.
        /// This function implements the flushing as per the underlying file system.
        /// The behavior of the Flush function varies with the underlying platform.
        ///
        /// A common use of Flush is write a file to disk immediately in order to prevent
        /// the file from being corrupted if the application crashes before the file
        /// is closed. However, on desktop platforms such as Windows this strategy is
        /// unnecesary, as the Windows OS file flush doesn't write the file to disk as
        /// might be expected. This actually is not a problem, because the Windows OS
        /// manages files outside the process and if your process crashes the OS will
        /// take care of safely closing the files. Only if the machine power is lost or
        /// if certain kinds of kernel-level crashes occur may you lose file data.
        virtual bool Flush() = 0;

        /// Writes bytes to the stream.
        /// If false is returned, you can use IStream::GetState to determine the error.
        /// Upon error, the stream pointer is at the position it was upon the error occurrence.
        /// @param pData
        /// @param nSize
        virtual size_t Write(const void *pData, size_t nSize) = 0;

        // Only use when reading the full data at once is necessary, otherwise use Read
        std::vector<uint8_t> AsBuffer() {
            std::vector<uint8_t> buffer;
            SetPosition(0);
            buffer.resize(GetSize());
            Read(buffer.data(), buffer.size());
            return buffer;
        }
    };
}

#define READ(stream_name, variable) \
    stream_name->Read(&variable, sizeof(variable))

#define WRITE(stream_name, variable) \
    stream_name->Write(&variable, sizeof(variable))

#define WRITE_VALUE(stream_name, type, value)      \
    do {                                           \
        auto _tmp = static_cast<type>(value);      \
        stream_name->Write(&_tmp, sizeof(_tmp));   \
    } while (0)

#define WRITE_STRING(stream_name, str)                         \
    do {                                                       \
        stream_name->Write((str), sizeof(char) * strlen(str)); \
    } while (0)

#define READ_CSTRING(stream_name, str)                            \
    do {                                                          \
        std::vector<char> _tmp_str;                               \
        char _ch;                                                 \
        do {                                                      \
            stream_name->Read(&_ch, sizeof(char));                \
            _tmp_str.push_back(_ch);                              \
        } while (_ch != '\0');                                    \
        str = new char[_tmp_str.size()];                          \
        memcpy(str, _tmp_str.data(), _tmp_str.size());            \
    } while (0)

#define WRITE_LENGTH(stream_name, len)                      \
    do {                                                    \
        size_t __write_len = (len);                         \
        uint8_t *__zero_buf = new uint8_t[__write_len]();   \
        stream_name->Write(__zero_buf, __write_len);        \
        delete[] __zero_buf;                                \
    } while (0)


#define SKIP(stream_name, len) \
    stream_name->SetPosition(len, ::EA::IO::PositionType::Current)

#define SEEK(stream_name, pos, type) \
    stream_name->SetPosition(pos, type)

#define OFFSET(stream_name) \
    stream_name->GetPosition(::EA::IO::PositionType::Begin)

#define CREATE_HOLE(stream_name, name, type)            \
    using name##__type = type;                          \
    size_t name##__offset = stream_name->GetPosition(); \
    SKIP(stream_name, sizeof(type))

#define FILL_HOLE(stream_name, name, value)                                 \
    do {                                                                    \
        size_t before__##name = OFFSET(stream_name);                        \
        SEEK(stream_name, name##__offset, ::EA::IO::PositionType::Begin);   \
        WRITE_VALUE(stream_name, name##__type, value);                      \
        SEEK(stream_name, before__##name, ::EA::IO::PositionType::Begin);   \
    } while (0)

#endif //ISTREAM_H
