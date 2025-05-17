//
// Created by exozg on 12/05/2025.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>

namespace EA::IO {
    enum class AccessFlags : uint32_t {
        /// No specified flags. Also used to indicate that a given IO stream is closed.
        None = 0x00,
        /// Used for identifying read access to an entity.
        Read = 0x01,
        /// Used for identifying write access to an entity.
        Write = 0x02,
        /// Used for identifying both read and write access to an entity.
        ReadWrite = 0x03
    };

    /// Creation disposition. Specifies aspects of how to create or not create a file during opening of it.
    enum class CD : uint32_t {
        /// Fails if file already exists.
        CreateNew = 1,
        /// Never fails, always opens or creates and truncates to 0.
        CreateAlways = 2,
        /// Fails if file doesn't exist, keeps contents.
        OpenExisting = 3,
        /// Never fails, creates if doesn't exist, keeps contents.
        OpenAlways = 4,
        /// Fails if file doesn't exist, but truncates to 0 if it does.
        TruncateExisting = 5,
        /// Default (implementation-specific) disposition
        Default = 6,
        /// Specific to MySims
        LoadAllFiles = 7,
    };

    /// Defines the positional basis for a user GetPosition or SetPosition action.
    enum class PositionType : uint32_t {
        /// For GetPosition refers to absolute index of next byte to read; always positive. For SetPosition, refers to absolute index of next byte to be written; always positive.
        Begin = 0,
        /// For GetPosition always returns zero. For SetPosition, refers to position relative to current position; can be positive or negative.
        Current = 1,
        /// For GetPosition returns to position relative to end (i.e. the negative of bytes left to read); always negative. For SetPosition, refers to position relative to end; can be positive or negative.
        End = 2
    };

    enum class FileError : uint32_t {
        Success = 0,
        InvalidHandle = 0xFFFFFFFE,
        OutOfMemory = 0xCFDE0002, // both OutOfMemory and NotEnoughMemory
        FileNotFound = 0xCFDE0004,
        PathNotFound = 0xCFDE0005, // also INVALID_DRIVE
        AccessDenied = 0xCFDE0006,
        CurrentDirectory = 0xCFDE0008,
        WriteProtect = 0xCFDE0007,
        NotReady = 0xCFDE000B,
        CRC = 0xCFDE000C,
        Other = 0xCFDE000D
    };
}

#endif //CONSTANTS_H
