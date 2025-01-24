//
// Created by exozg on 24/01/2025.
//

#include "Signatures.h"
#include "../util/Logger.h"

Signatures & Signatures::GetInstance() {
    static Signatures signatures;
    return signatures;
}

bool Signatures::Search(const sigmatch::signature &sig, void *&address, const uint32_t offset) {
    sigmatch::search_result result = context.search(sig);

    auto matches = result.matches();

    const size_t matchCount = std::distance(matches.begin(), matches.end());

    if(matchCount == 0) {
        MSML_LOG_ERROR("Failed to find any matches");
        return false;
    }

    if(matchCount > 1) {
        MSML_LOG_ERROR("Too many matches: %d", matchCount);
        for (const std::byte *foundAddress : matches) {
            MSML_LOG_ERROR(" Matched: %p", foundAddress);
        }
        return false;
    }

    for (const std::byte *foundAddress : matches) {
        address = reinterpret_cast<void *>(const_cast<std::byte*>(foundAddress));
        address = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(address) + offset);
        return true;
    }

    return false;
}

Signatures::Signatures() {
    sigmatch::this_process_target target;
    context = target.in_module("MySims.exe");
}
