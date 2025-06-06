//
// Created by exozg on 11/05/2025.
//

#ifndef IRECORD_H
#define IRECORD_H
#include "../io/IStream.h"
#include "ResourceKey.h"

namespace EA::ResourceMan {
    class IDatabase;

    class IRecord {
    public:
        virtual ~IRecord() = default;

        virtual Key &GetKey() = 0;

        virtual void SetKey(const Key &key) = 0;

        virtual IO::IStream *GetStream() = 0;

        virtual IDatabase *GetDatabase() = 0;

        virtual void Close() = 0;

        virtual void SetStream(IO::IStream *stream) = 0;
    };
}
#endif //IRECORD_H
