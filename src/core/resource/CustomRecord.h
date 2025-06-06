//
// Created by exozg on 11/05/2025.
//

#ifndef CUSTOMRECORD_H
#define CUSTOMRECORD_H

#include "../../EA/ResourceMan/IRecord.h"

namespace msml::core::resource {
    class CustomRecord : public EA::ResourceMan::IRecord {
    public:
        EA::ResourceMan::Key key;

        EA::IO::IStream *stream;
        EA::ResourceMan::IDatabase *database;

        CustomRecord(EA::ResourceMan::Key key, EA::IO::IStream *stream, EA::ResourceMan::IDatabase *database);

        EA::ResourceMan::Key &GetKey() override;

        void SetKey(const EA::ResourceMan::Key &key) override;

        EA::IO::IStream *GetStream() override;

        EA::ResourceMan::IDatabase *GetDatabase() override;

        void Close() override;

        void SetStream(EA::IO::IStream *stream) override;
    };
}

#endif //CUSTOMRE
