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

namespace Msml::Core {

    static constexpr uint32_t kSignatureVersion = 0;

    Signatures & Signatures::GetInstance() {
        static Signatures signatures;
        return signatures;
    }

    bool Signatures::Search(const sigmatch::signature &sig, void *&pAddress, const uint32_t kOffset, const bool kbFirst) const {
        sigmatch::search_result result = mContext.search(sig);

        auto matches = result.matches();

        const size_t kMatchCount = std::distance(matches.begin(), matches.end());

        if(kMatchCount == 0) {
            MSML_LOG_ERROR("Failed to find any matches");
            return false;
        }

        if(!kbFirst && kMatchCount > 1) {
            MSML_LOG_ERROR("Too many matches: %d", kMatchCount);
            for (const std::byte *foundAddress : matches) {
                MSML_LOG_ERROR(" Matched: %p", foundAddress);
            }
            return false;
        }

        for (const std::byte *foundAddress : matches) {
            pAddress = reinterpret_cast<void *>(const_cast<std::byte*>(foundAddress));
            pAddress = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(pAddress) + kOffset);
            return true;
        }

        return false;
    }

    void Signatures::Append(const std::string& name, SigSearchBase* sig){
        mSignatures.emplace(name, sig);
    }

    std::array<uint8_t, 32U> Signatures::GetCheckSum() {
        hash_sha256 checksum;

        checksum.sha256_init();

        char path[MAX_PATH];
        GetModuleFileName(nullptr, path, MAX_PATH);

        auto *const kStream = new EA::IO::FileStream(path);
        const auto kBuffer = Util::StreamUtil::ReadBytes(kStream);
        kStream->Close();

        checksum.sha256_update(kBuffer.data(), kBuffer.size());

        return checksum.sha256_final();
    }

    bool Signatures::SearchAll() {
        const auto kStart = std::chrono::high_resolution_clock::now();

        LoadDatabase();

        for(const auto &val: mSignatures | std::views::values) {
            if (val->GetAddress() == nullptr && !val->Search()) {
                return false;
            }
        }

        const auto kElapsed = std::chrono::high_resolution_clock::now() - kStart;
        const double kElapsedMs = std::chrono::duration_cast<std::chrono::microseconds>(kElapsed).count() / 1000.0;
        MSML_LOG_INFO("Search time: %.3f milliseconds", kElapsedMs);

        SaveDatabase();

        return true;
    }

    Signatures::Signatures() {
        const sigmatch::this_process_target kTarget;
        mContext = kTarget.in_module("MySims.exe");
    }

    bool Signatures::LoadDatabase() {
#ifndef NDEBUG
        return false;
#endif
        auto *const kStream = new EA::IO::FileStream("signatures.db");

        uint32_t sigVersion = UINT32_MAX;
        READ(kStream, sigVersion);

        if (sigVersion != kSignatureVersion) {
            return false;
        }

        std::string msmlVersion;
        READ_LEN_STRING(kStream, msmlVersion);

        std::array<uint8_t, 32U> checksum{};
        for (auto i = 0; i < 32U; i++) {
            READ(kStream, checksum[i]);
        }

        if (const std::array<uint8_t, 32U> kCurrentChecksum = GetCheckSum(); checksum != kCurrentChecksum) {
            MSML_LOG_ERROR("Checksum mismatch, searching for new signatures");
            return false;
        }

        uint32_t sigCount = 0;
        READ(kStream, sigCount);

        for (uint32_t i = 0; i < sigCount; i++) {
            std::string sigName;
            READ_LEN_STRING(kStream, sigName);
            uint64_t address = 0;
            READ(kStream, address);

            if (mSignatures.contains(sigName)) {
                mSignatures[sigName]->ApplyAddress(reinterpret_cast<void *>(address));
            } else {
                MSML_LOG_ERROR("Failed to find signatures with name %s", sigName.c_str());
                return false;
            }
        }

        MSML_LOG_INFO("Read %d signatures from signatures.db", sigCount);

        kStream->Close();

        return true;
    }

    void Signatures::SaveDatabase() const {
#ifndef NDEBUG
        return;
#endif
        auto *const kStream = new EA::IO::FileStream("signatures.db", EA::IO::AccessFlags::kWrite, EA::IO::CD::kCreateAlways);

        WRITE(kStream, kSignatureVersion);
        WRITE_LEN_STRING(kStream, MSML_VERSION);

        const std::array<uint8_t, 32U> kChecksum = GetCheckSum();
        for (auto i = 0; i < 32U; i++) {
            WRITE(kStream, kChecksum[i]);
        }

        WRITE_VALUE(kStream, uint32_t, mSignatures.size());
        for (const auto& signature : mSignatures) {
            WRITE_LEN_STRING(kStream, signature.first.c_str());
            WRITE_VALUE(kStream, uint64_t, reinterpret_cast<uint64_t>(signature.second->GetAddress()));
        }

        MSML_LOG_INFO("Signature cache saved");

        kStream->Close();
    }
}
