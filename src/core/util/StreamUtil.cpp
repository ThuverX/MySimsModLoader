//
// Created by exozg on 31/05/2025.
//

#include "StreamUtil.h"
namespace msml::core::util::StreamUtil {
    std::vector<uint8_t> ReadBytes(EA::IO::IStream* stream) {
        std::vector<uint8_t> buffer;
        stream->AddRef();
        stream->SetPosition(0);
        buffer.resize(stream->GetSize());
        stream->Read(buffer.data(), buffer.size());
        stream->Release();
        return buffer;
    }

    std::string ReadString(EA::IO::IStream* stream) {
        stream->SetPosition(0);
        stream->AddRef();
        const auto n_size = stream->GetSize();
        std::string out_string(n_size, '\0');
        stream->Read(out_string.data(), n_size);
        stream->Release();

        return out_string;
    }
}
