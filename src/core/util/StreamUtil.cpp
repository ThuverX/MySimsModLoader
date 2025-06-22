//
// Created by exozg on 31/05/2025.
//

#include "StreamUtil.h"
namespace Msml::Core::Util::StreamUtil {
    std::vector<uint8_t> ReadBytes(EA::IO::IStream* pStream) {
        std::vector<uint8_t> buffer;
        pStream->AddRef();
        pStream->SetPosition(0);
        buffer.resize(pStream->GetSize());
        pStream->Read(buffer.data(), buffer.size());
        pStream->Release();
        return buffer;
    }

    std::string ReadString(EA::IO::IStream* pStream) {
        pStream->SetPosition(0);
        pStream->AddRef();
        const auto kNSize = pStream->GetSize();
        std::string outString(kNSize, '\0');
        pStream->Read(outString.data(), kNSize);
        pStream->Release();

        return outString;
    }
}
