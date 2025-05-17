#ifndef VECTOR3_H
#define VECTOR3_H
#include "../../EA/IO/IStream.h"

struct Vector3 {
    float x;
    float y;
    float z;

    static void Read(Vector3 &instance, EA::IO::IStream* stream) {
        READ(stream, instance.x);
        READ(stream, instance.y);
        READ(stream, instance.z);
    }
};

#endif // VECTOR3_H
