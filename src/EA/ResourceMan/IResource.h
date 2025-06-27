//
// Created by exozg on 31/05/2025.
//

#ifndef IRESOURCE_H
#define IRESOURCE_H

#include "ResourceKey.h"

namespace EA::ResourceMan {
    class IResource {
    public:
        virtual int AddRef();

        virtual int Release();

        virtual void *Cast(uint32_t typeID) const;

        virtual int GetReferenceCount();

        Key GetResourceKey() const;

        void SetResourceKey(const Key &kKey);

    protected:
        int mRefCount;
        Key mNameKey;
    };
}

#endif //IRESOURCE_H
