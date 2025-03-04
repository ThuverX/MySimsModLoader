//
// Created by exozg on 24/01/2025.
//

#ifndef SIGNATURES_H
#define SIGNATURES_H

#include <sigmatch/sigmatch.hpp>
#include <unordered_map>

template <typename OriginalFuncPtr>
class SigSearch;

class SigSearchBase;

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

#endif //SIGNATURES_H