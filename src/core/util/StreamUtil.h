//
// Created by exozg on 31/05/2025.
//

#ifndef STREAMUTIL_H
#define STREAMUTIL_H

#include <string>
#include <vector>

#include "../../EA/IO/IStream.h"


namespace Msml::Core::Util::StreamUtil {
    std::vector<uint8_t> ReadBytes(EA::IO::IStream* pStream);
    std::string ReadString(EA::IO::IStream* pStream);
}


#endif //STREAMUTIL_H
