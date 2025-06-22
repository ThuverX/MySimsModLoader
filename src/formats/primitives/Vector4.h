#ifndef VECTOR4_H
#define VECTOR4_H
#include "../../EA/IO/IStream.h"

struct Vector4 {
    float x;
    float y;
    float z;
    float w;

    static void Read(Vector4 &instance, EA::IO::IStream *pStream) {
        READ(pStream, instance.x);
        READ(pStream, instance.y);
        READ(pStream, instance.z);
        READ(pStream, instance.w);
    }
};

#endif // VECTOR4_H
