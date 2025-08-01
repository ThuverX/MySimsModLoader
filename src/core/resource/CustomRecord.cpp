//
// Created by exozg on 11/05/2025.
//

#include "CustomRecord.h"

namespace Msml::Core::Resource  {
    CustomRecord::CustomRecord(const EA::ResourceMan::Key key, EA::IO::IStream *stream,
                               EA::ResourceMan::IDatabase *database): mKey(key), mStream(stream), mDatabase(database) {
    }

    EA::ResourceMan::Key &CustomRecord::GetKey() {
        return mKey;
    }

    void CustomRecord::SetKey(const EA::ResourceMan::Key &key) {
        this->mKey = key;
    }

    EA::IO::IStream *CustomRecord::GetStream() {
        return mStream;
    }

    EA::ResourceMan::IDatabase *CustomRecord::GetDatabase() {
        return mDatabase;
    }

    void CustomRecord::Close() {
        // TODO: Figure out why this crashes
        // if (stream != nullptr)
        //     stream->Close();
    }

    void CustomRecord::SetStream(EA::IO::IStream *stream) {
        this->mStream = stream;
    }
}
