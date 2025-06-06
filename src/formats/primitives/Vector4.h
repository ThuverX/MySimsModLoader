#ifndef VECTOR4_H
#define VECTOR4_H
#include "../../EA/IO/IStream.h"

struct Vector4 {
    float x;
    float y;
    float z;
    float w;

    static void Read(Vector4 &instance, EA::IO::IStream* stream) {
        READ(stream, instance.x);
        READ(stream, instance.y);
        READ(stream, instance.z);
        READ(stream, instance.w);
    }
};

#endif // VECTOR4_H
