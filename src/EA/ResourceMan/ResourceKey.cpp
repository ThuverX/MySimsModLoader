//
// Created by exozg on 17/05/2025.
//

#include "ResourceKey.h"

void EA::ResourceMan::Key::Read(Key &instance, IO::IStream *stream) {
    READ(stream, instance.instance);
    READ(stream, instance.type);
    READ(stream, instance.group);
}

void EA::ResourceMan::Key::Write(IO::IStream *stream) const {
    WRITE(stream, instance);
    WRITE(stream, type);
    WRITE(stream, group);
}
