//
// Created by exozg on 24/01/2025.
//

#ifndef SIGNATURES_H
#define SIGNATURES_H

#include <sigmatch/sigmatch.hpp>
#include <unordered_map>


namespace msml::core {
    class SigSearchBase {
    public:
        virtual ~SigSearchBase() = default;

        [[nodiscard]] virtual bool Search() const = 0;

        [[nodiscard]] virtual std::string GetName() const = 0;

        [[nodiscard]] virtual void *GetAddress() const = 0;

        virtual void ApplyAddress(void *addr) const = 0;
    };

    class Signatures {
    public:
        static Signatures& GetInstance();
        bool Search(const sigmatch::signature &sig, void*& address, uint32_t offset, bool first) const;
        bool SearchAll();
        void Append(std::string name, SigSearchBase* sig);
        std::array<uint8_t, 32U> GetCheckSum();

    private:
        Signatures();
        std::unordered_map<std::string, SigSearchBase*> signatures;
        sigmatch::search_context context;

        bool LoadDatabase();
        void SaveDatabase();
    };
}

#endif //SIGNATURES_H