//
// Created by exozg on 17/05/2025.
//

#include "ResourceKey.h"

void EA::ResourceMan::Key::Read(Key &instance, IO::IStream *stream) {
    READ(stream, instance.mInstance);
    READ(stream, instance.mType);
    READ(stream, instance.mGroup);
}

void EA::ResourceMan::Key::Write(IO::IStream *stream) const {
    WRITE(stream, mInstance);
    WRITE(stream, mType);
    WRITE(stream, mGroup);
}
