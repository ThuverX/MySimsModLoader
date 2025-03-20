//
// Created by exozg on 13/03/2025.
//

#ifndef TWEAKER_H
#define TWEAKER_H
#include "../signatures/sigdef.h"


class Tweaker {
public:
    virtual void Apply(const EA::ResourceMan::Key& key) = 0;
    virtual ~Tweaker() = default;
};



#endif //TWEAKER_H
