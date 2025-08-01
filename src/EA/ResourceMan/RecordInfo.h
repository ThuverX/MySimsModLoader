//
// Created by exozg on 10/05/2025.
//

#ifndef RECORDINFO_H
#define RECORDINFO_H

#pragma pack(push, 2)

namespace EA::ResourceMan {
    struct RecordInfo {
        /// The offset, in bytes, of the file data inside the package file.
        size_t mChunkOffset;
        /// The size, in bytes, that this file occupies inside the package file.
        size_t mCompressedSize;
        /// The size, in bytes, that this file occupies in memory, once uncompressed.
        size_t mMemorySize;
        /// 0 -> no compression, -1 -> compressed
        uint16_t mFlags; // 0xFFFF for compressed
        /// Whether this item has been saved and written into the package.
        bool mIsSaved;
    };
}

#pragma pack(pop)

#endif //RECORDINFO_H
