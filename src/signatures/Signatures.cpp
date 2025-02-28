//
// Created by exozg on 24/01/2025.
//

#include "Signatures.h"
#include "../util/Logger.h"
#include <chrono>
#include <ranges>

#include "windows.h"
#include "sigdef.h"
#include "../include/hash_sha256.h"
#include "../modloader/ModLoader.h"
#include "../util/File.h"

Signatures & Signatures::GetInstance() {
    static Signatures signatures;
    return signatures;
}

bool Signatures::Search(const sigmatch::signature &sig, void *&address, const uint32_t offset, bool first) const {
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

void Signatures::Append(std::string name, SigSearchBase* sig){
    signatures.emplace(name, sig);
}

std::array<uint8_t, 32U> Signatures::GetCheckSum() {
    hash_sha256 checksum;

    checksum.sha256_init();

    char path[MAX_PATH];
    GetModuleFileName(nullptr, path, MAX_PATH);

    const auto exeBytes = File::ReadAsBytes(path);

    checksum.sha256_update(exeBytes.data(), exeBytes.size());

    return checksum.sha256_final();
}

void Signatures::SearchAll() {
    const auto start = std::chrono::high_resolution_clock::now();

    if (!LoadDatabase()) {
        for(const auto &val: signatures | std::views::values) {
            val->Search();
        }
    }

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    double elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000.0;
    MSML_LOG_INFO("Search time: %.3f milliseconds", elapsed_ms);

    SaveDatabase();
}

Signatures::Signatures() {
    const sigmatch::this_process_target target;
    context = target.in_module("MySims.exe");
}

#define READ_BIN(var) infile.read((char*)&var, sizeof(var));
#define WRITE_BIN(var) outfile.write((char*)&var, sizeof(var))

#define SIGCACHE_DB_NAME "sigcache.db"

bool Signatures::LoadDatabase() {
#ifndef NDEBUG
    return false;
#endif

    std::ifstream infile(SIGCACHE_DB_NAME, std::ios::in | std::ios::binary);
    if (!infile.good()) {
        MSML_LOG_ERROR("Failed to open sigcache");
        return false;
    }

    std::array<uint8_t, 32U> checksum{};
    for (auto i = 0; i < 32U; i++) {
        READ_BIN(checksum[i]);
    }

    if (std::array<uint8_t, 32U> currentChecksum = GetCheckSum(); checksum != currentChecksum) {
        MSML_LOG_ERROR("Checksum mismatch, searching for new signatures");
        return false;
    }

    uint32_t sigCount;
    READ_BIN(sigCount);
    for (uint32_t i = 0; i < sigCount; i++) {
        uint32_t length;
        READ_BIN(length);

        char name[1024];
        infile.read(name, length);

        auto name_str = std::string(name, length);

        uint32_t addr;
        READ_BIN(addr);

        if (signatures.contains(name_str)) {
            signatures[name_str]->ApplyAddress(reinterpret_cast<void *>(addr));
        } else {
            MSML_LOG_ERROR("Failed to find signatures with name %s", name_str);
        }
    }

    return true;
}

void Signatures::SaveDatabase() {
#ifndef NDEBUG
    return;
#endif

    std::ofstream outfile(SIGCACHE_DB_NAME, std::ios::out | std::ios::binary);

    std::array<uint8_t, 32U> checksum = GetCheckSum();
    uint32_t count = signatures.size();

    for (auto i = 0; i < 32U; i++) {
        WRITE_BIN(checksum[i]);
    }
    WRITE_BIN(count);

    for (const auto &sig: signatures) {
        std::string name = sig.first;
        uint32_t length = name.length();
        auto addr = reinterpret_cast<uint32_t>(sig.second->GetAddress());

        WRITE_BIN(length);
        outfile.write(name.c_str(), length);
        WRITE_BIN(addr);
    }

    MSML_LOG_INFO("Signature cache saved");

    outfile.close();
}
