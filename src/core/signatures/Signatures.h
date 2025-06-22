//
// Created by exozg on 24/01/2025.
//

#ifndef SIGNATURES_H
#define SIGNATURES_H

#include <sigmatch/sigmatch.hpp>
#include <unordered_map>

namespace Msml::Core {
    class SigSearchBase {
    public:
        virtual ~SigSearchBase() = default;

        [[nodiscard]] virtual bool Search() const = 0;

        [[nodiscard]] virtual std::string GetName() const = 0;

        [[nodiscard]] virtual void *GetAddress() const = 0;

        virtual void ApplyAddress(void *pAddr) const = 0;
    };

    class Signatures {
    public:
        static Signatures& GetInstance();
        bool Search(const sigmatch::signature &sig, void*& pAddress, uint32_t kOffset, bool kbFirst) const;
        bool SearchAll();
        void Append(const std::string& name, SigSearchBase* pSig);
        static std::array<uint8_t, 32U> GetCheckSum();

    private:
        Signatures();
        std::unordered_map<std::string, SigSearchBase*> mSignatures;
        sigmatch::search_context mContext;

        bool LoadDatabase();
        void SaveDatabase() const;
    };
}

#endif //SIGNATURES_H