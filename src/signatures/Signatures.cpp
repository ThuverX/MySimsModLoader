//
// Created by exozg on 24/01/2025.
//

#include "Signatures.h"
#include "../util/Logger.h"
#include <chrono>
#include <ranges>

#include "sigdef.h"

Signatures & Signatures::GetInstance() {
    static Signatures signatures;
    return signatures;
}

bool Signatures::Search(const sigmatch::signature &sig, void *&address, const uint32_t offset, bool first) {
    sigmatch::search_result result = context.search(sig);

    auto matches = result.matches();

    const size_t matchCount = std::distance(matches.begin(), matches.end());

    if(matchCount == 0) {
        MSML_LOG_ERROR("Failed to find any matches");
        return false;
    }

    if(!first && matchCount > 1) {
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

void Signatures::Append(SigSearchBase* sig){
    signatures.emplace_back(sig);
}

void Signatures::SearchAll() const {
    auto start = std::chrono::high_resolution_clock::now();
    for(const auto &sig: signatures) {
        sig->Search();
    }
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    double elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000.0;
    MSML_LOG_INFO("Search time: %.3f milliseconds", elapsed_ms);
}

Signatures::Signatures() {
    sigmatch::this_process_target target;
    context = target.in_module("MySims.exe");
}
