//
// Created by exozg on 12/05/2025.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>

namespace EA::IO {
    enum class AccessFlags : uint32_t {
        /// No specified flags. Also used to indicate that a given IO stream is closed.
        kNone = 0x00,
        /// Used for identifying read access to an entity.
        kRead = 0x01,
        /// Used for identifying write access to an entity.
        kWrite = 0x02,
        /// Used for identifying both read and write access to an entity.
        kReadWrite = 0x03
    };

    /// Creation disposition. Specifies aspects of how to create or not create a file during opening of it.
    enum class CD : uint32_t {
        /// Fails if file already exists.
        kCreateNew = 1,
        /// Never fails, always opens or creates and truncates to 0.
        kCreateAlways = 2,
        /// Fails if file doesn't exist, keeps contents.
        kOpenExisting = 3,
        /// Never fails, creates if doesn't exist, keeps contents.
        kOpenAlways = 4,
        /// Fails if file doesn't exist, but truncates to 0 if it does.
        kTruncateExisting = 5,
        /// Default (implementation-specific) disposition
        kDefault = 6,
        /// Specific to MySims
        kLoadAllFiles = 7,
    };

    /// Defines the positional basis for a user GetPosition or SetPosition action.
    enum class PositionType : uint32_t {
        /// For GetPosition refers to absolute index of next byte to read; always positive. For SetPosition, refers to absolute index of next byte to be written; always positive.
        kBegin = 0,
        /// For GetPosition always returns zero. For SetPosition, refers to position relative to current position; can be positive or negative.
        kCurrent = 1,
        /// For GetPosition returns to position relative to end (i.e. the negative of bytes left to read); always negative. For SetPosition, refers to position relative to end; can be positive or negative.
        kEnd = 2
    };

    enum class FileError : uint32_t {
        kSuccess = 0,
        kInvalidHandle = 0xFFFFFFFE,
        kOutOfMemory = 0xCFDE0002, // both OutOfMemory and NotEnoughMemory
        kFileNotFound = 0xCFDE0004,
        kPathNotFound = 0xCFDE0005, // also INVALID_DRIVE
        kAccessDenied = 0xCFDE0006,
        kCurrentDirectory = 0xCFDE0008,
        kWriteProtect = 0xCFDE0007,
        kNotReady = 0xCFDE000B,
        kCrc = 0xCFDE000C,
        kOther = 0xCFDE000D
    };
}

#endif //CONSTANTS_H
