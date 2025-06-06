//
// Created by exozg on 08/05/2025.
//

#ifndef KEYFILTER_H
#define KEYFILTER_H
#include "ResourceKey.h"

namespace EA::ResourceMan {
    class IKeyFilter
    {
    public:
        virtual ~IKeyFilter() = default;

        /// A method used to determine whether a file with this ResourceKey name is accepted.
        /// @param name
        /// @returns True if the file is accepted, false otherwise.
        virtual bool IsValid(const Key& name) = 0;
    };
}

#endif //KEYFILTER_H
