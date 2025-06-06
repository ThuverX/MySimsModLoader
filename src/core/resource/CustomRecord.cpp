//
// Created by exozg on 11/05/2025.
//

#include "CustomRecord.h"

namespace msml::core::resource {
    CustomRecord::CustomRecord(const EA::ResourceMan::Key key, EA::IO::IStream *stream,
                               EA::ResourceMan::IDatabase *database): key(key), stream(stream), database(database) {
    }

    EA::ResourceMan::Key &CustomRecord::GetKey() {
        return key;
    }

    void CustomRecord::SetKey(const EA::ResourceMan::Key &key) {
        this->key = key;
    }

    EA::IO::IStream *CustomRecord::GetStream() {
        return stream;
    }

    EA::ResourceMan::IDatabase *CustomRecord::GetDatabase() {
        return database;
    }

    void CustomRecord::Close() {
        // TODO: Figure out why this crashes
        // if (stream != nullptr)
        //     stream->Close();
    }

    void CustomRecord::SetStream(EA::IO::IStream *stream) {
        this->stream = stream;
    }
}
