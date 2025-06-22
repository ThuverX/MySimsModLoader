#ifndef VECTOR3_H
#define VECTOR3_H
#include "../../EA/IO/IStream.h"

struct Vector3 {
    float x;
    float y;
    float z;

    static void Read(Vector3 &instance, EA::IO::IStream *pStream) {
        READ(pStream, instance.x);
        READ(pStream, instance.y);
        READ(pStream, instance.z);
    }
};

#endif // VECTOR3_H
