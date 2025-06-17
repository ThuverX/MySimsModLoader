//
// Created by exozg on 24/01/2025.
//

#include "Signatures.h"
#include <chrono>
#include <ranges>

#include "windows.h"
#include "sigdef.h"
#include "../../Version.h"
#include "../../EA/IO/FileStream.h"
#include "../../EA/IO/MemoryStream.h"
#include "../../include/hash_sha256.h"
#include "../system/Logger.h"
#include "../util/StreamUtil.h"

namespace msml::core {

    static constexpr uint32_t SIGNATURE_VERSION = 0;

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

    void Signatures::Append(const std::string& name, SigSearchBase* sig){
        signatures.emplace(name, sig);
    }

    std::array<uint8_t, 32U> Signatures::GetCheckSum() {
        hash_sha256 checksum;

        checksum.sha256_init();

        char path[MAX_PATH];
        GetModuleFileName(nullptr, path, MAX_PATH);

        const auto stream = new EA::IO::FileStream(path);
        const auto buffer = util::StreamUtil::ReadBytes(stream);
        stream->Close();

        checksum.sha256_update(buffer.data(), buffer.size());

        return checksum.sha256_final();
    }

    bool Signatures::SearchAll() {
        const auto start = std::chrono::high_resolution_clock::now();

        LoadDatabase();

        for(const auto &val: signatures | std::views::values) {
            if (val->GetAddress() == nullptr && !val->Search()) {
                return false;
            }
        }

        const auto elapsed = std::chrono::high_resolution_clock::now() - start;
        const double elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000.0;
        MSML_LOG_INFO("Search time: %.3f milliseconds", elapsed_ms);

        SaveDatabase();

        return true;
    }

    Signatures::Signatures() {
        const sigmatch::this_process_target target;
        context = target.in_module("MySims.exe");
    }

    bool Signatures::LoadDatabase() {
#ifndef NDEBUG
        return false;
#endif
        const auto stream = new EA::IO::FileStream("signatures.db");

        uint32_t sig_version = UINT32_MAX;
        READ(stream, sig_version);

        if (sig_version != SIGNATURE_VERSION) {
            return false;
        }

        std::string msml_version;
        READ_LEN_STRING(stream, msml_version);

        std::array<uint8_t, 32U> checksum{};
        for (auto i = 0; i < 32U; i++) {
            READ(stream, checksum[i]);
        }

        if (const std::array<uint8_t, 32U> currentChecksum = GetCheckSum(); checksum != currentChecksum) {
            MSML_LOG_ERROR("Checksum mismatch, searching for new signatures");
            return false;
        }

        uint32_t sigCount;
        READ(stream, sigCount);

        for (int i =0; i < sigCount; i++) {
            std::string sig_name;
            READ_LEN_STRING(stream, sig_name);
            uint64_t address;
            READ(stream, address);

            if (signatures.contains(sig_name)) {
                signatures[sig_name]->ApplyAddress(reinterpret_cast<void *>(address));
            } else {
                MSML_LOG_ERROR("Failed to find signatures with name %s", sig_name.c_str());
                return false;
            }
        }

        MSML_LOG_INFO("Read %d signatures from signatures.db", sigCount);

        stream->Close();

        return true;
    }

    void Signatures::SaveDatabase() const {
#ifndef NDEBUG
        return;
#endif
        const auto stream = new EA::IO::FileStream("signatures.db", EA::IO::AccessFlags::Write, EA::IO::CD::CreateAlways);

        WRITE(stream, SIGNATURE_VERSION);
        WRITE_LEN_STRING(stream, MSML_VERSION);

        const std::array<uint8_t, 32U> checksum = GetCheckSum();
        for (auto i = 0; i < 32U; i++) {
            WRITE(stream, checksum[i]);
        }

        WRITE_VALUE(stream, uint32_t, signatures.size());
        for (const auto& signature : signatures) {
            WRITE_LEN_STRING(stream, signature.first.c_str());
            WRITE_VALUE(stream, uint64_t, reinterpret_cast<uint64_t>(signature.second->GetAddress()));
        }

        MSML_LOG_INFO("Signature cache saved");

        stream->Close();
    }
}
