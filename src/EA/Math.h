//
// Created by exozg on 29/12/2025.
//

#ifndef MYSIMSMODLOADER_MATH_H
#define MYSIMSMODLOADER_MATH_H

namespace EA
{
    namespace Math
    {
        struct Vector3
        {
            float mX;
            float mY;
            float mZ;
        };

        struct Quaternion
        {
            float mX;
            float mY;
            float mZ;
            float mW;
        };

        struct Vector4
        {
            float mX;
            float mY;
            float mZ;
            float mW;
        };

        struct Transform
        {
            Vector4 mPosition;
            Quaternion mRotation;
        };
    };
}

#endif //MYSIMSMODLOADER_MATH_H