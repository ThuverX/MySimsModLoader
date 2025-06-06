#ifndef VECTOR2_H
#define VECTOR2_H
#include "../../EA/io/IStream.h"

struct Vector2 {
    float x;
    float y;

    static void Read(Vector2 &instance, EA::IO::IStream* stream) {
        READ(stream, instance.x);
        READ(stream, instance.y);
    }
};

#endif // VECTOR2_H
