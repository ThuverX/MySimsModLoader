//
// Created by exozg on 11/05/2025.
//

#ifndef CUSTOMRECORD_H
#define CUSTOMRECORD_H

#include "../../EA/ResourceMan/IRecord.h"

namespace Msml::Core::Resource {
    class CustomRecord : public EA::ResourceMan::IRecord {
    public:
        EA::ResourceMan::Key mKey;

        EA::IO::IStream *mStream;
        EA::ResourceMan::IDatabase *mDatabase;

        CustomRecord(EA::ResourceMan::Key key, EA::IO::IStream *pStream, EA::ResourceMan::IDatabase *pDatabase);

        EA::ResourceMan::Key &GetKey() override;

        void SetKey(const EA::ResourceMan::Key &key) override;

        EA::IO::IStream *GetStream() override;

        EA::ResourceMan::IDatabase *GetDatabase() override;

        void Close() override;

        void SetStream(EA::IO::IStream *stream) override;
    };
}

#endif //CUSTOMRE
