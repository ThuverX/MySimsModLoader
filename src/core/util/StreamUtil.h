//
// Created by exozg on 31/05/2025.
//

#ifndef STREAMUTIL_H
#define STREAMUTIL_H
#include <fstream>
#include <vector>

#include "../../EA/IO/IStream.h"


namespace msml::core::util::StreamUtil {
    std::vector<uint8_t> ReadBytes(EA::IO::IStream* stream);
    std::string ReadString(EA::IO::IStream* stream);
}


#endif //STREAMUTIL_H
