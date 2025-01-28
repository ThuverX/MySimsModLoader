//
// Created by exozg on 24/01/2025.
//

#ifndef SIGNATURES_H
#define SIGNATURES_H

#include <string>
#include <sigmatch/sigmatch.hpp>

template <typename OriginalFuncPtr>
class SigSearch;

class SigSearchBase;

class Signatures {
public:
    static Signatures& GetInstance();
    bool Search(const sigmatch::signature &sig, void*& address, uint32_t offset, bool first);
    void SearchAll() const;
    void Append(SigSearchBase* sig);

private:
    Signatures();
    std::vector<SigSearchBase*> signatures;
    sigmatch::search_context context;
};

#endif //SIGNATURES_H