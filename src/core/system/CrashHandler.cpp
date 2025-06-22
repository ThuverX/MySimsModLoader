//
// Created by exozg on 13/04/2025.
//

#include "CrashHandler.h"
#include "../modloader/ModLoader.h"
#include "Logger.h"
#include <fstream>
#include <dbghelp.h>
#include <psapi.h>
#include <iomanip>
#include <sstream>

#pragma comment(lib, "dbghelp.lib")

namespace Msml::Core::System {
    std::string GetExceptionTypeName(const DWORD kExceptionCode) {
        switch (kExceptionCode) {
            case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION";
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
            case EXCEPTION_BREAKPOINT: return "EXCEPTION_BREAKPOINT";
            case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT";
            case EXCEPTION_FLT_DENORMAL_OPERAND: return "EXCEPTION_FLT_DENORMAL_OPERAND";
            case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
            case EXCEPTION_FLT_INEXACT_RESULT: return "EXCEPTION_FLT_INEXACT_RESULT";
            case EXCEPTION_FLT_INVALID_OPERATION: return "EXCEPTION_FLT_INVALID_OPERATION";
            case EXCEPTION_FLT_OVERFLOW: return "EXCEPTION_FLT_OVERFLOW";
            case EXCEPTION_FLT_STACK_CHECK: return "EXCEPTION_FLT_STACK_CHECK";
            case EXCEPTION_FLT_UNDERFLOW: return "EXCEPTION_FLT_UNDERFLOW";
            case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION";
            case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR";
            case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
            case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW";
            case EXCEPTION_INVALID_DISPOSITION: return "EXCEPTION_INVALID_DISPOSITION";
            case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
            case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION";
            case EXCEPTION_SINGLE_STEP: return "EXCEPTION_SINGLE_STEP";
            case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW";
            default: return "UNKNOWN_EXCEPTION";
        }
    }

    constexpr uintptr_t kBasePtr = 0x140000000;

    std::pair<std::string, uintptr_t> GetModuleLocation(uintptr_t addr) {
        HMODULE hMods[1024];
        DWORD cbNeeded = 0;
        HANDLE hProcess = GetCurrentProcess();

        if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
            for (DWORD i = 0; i < cbNeeded / sizeof(HMODULE); ++i) {
                MODULEINFO modInfo;
                if (GetModuleInformation(hProcess, hMods[i], &modInfo, sizeof(modInfo))) {
                    const auto kBase = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
                    const uintptr_t size = modInfo.SizeOfImage;

                    if (addr >= kBase && addr < kBase + size) {
                        char modName[MAX_PATH];
                        GetModuleBaseNameA(hProcess, hMods[i], modName, MAX_PATH);

                        uintptr_t offset = addr - kBase;

                        if (strcmp(modName, "MySims.exe") == 0) {
                            offset += kBasePtr;
                        }

                        return {modName, offset};
                    }
                }
            }
        }

        return {"", 0};
    }

    std::string GetStackTrace(PEXCEPTION_POINTERS pExceptionInfo) {
        auto *context = pExceptionInfo->ContextRecord;

        std::stringstream stackTrace;

        HANDLE process = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();

        SymInitialize(process, nullptr, TRUE);

        STACKFRAME64 stackFrame = {};
#ifdef _WIN64
        DWORD machineType = IMAGE_FILE_MACHINE_AMD64;

        stackFrame.AddrPC.Offset = context->Rip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context->Rbp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = context->Rsp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#else
        DWORD machineType = IMAGE_FILE_MACHINE_I386;
        stackFrame.AddrPC.Offset    = context->Eip;
        stackFrame.AddrPC.Mode      = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context->Ebp;
        stackFrame.AddrFrame.Mode   = AddrModeFlat;
        stackFrame.AddrStack.Offset = context->Esp;
        stackFrame.AddrStack.Mode   = AddrModeFlat;
#endif

        for (int i = 0; i < 32; ++i) {
            if (StackWalk64(machineType, process, thread, &stackFrame, context, nullptr,
                            SymFunctionTableAccess64, SymGetModuleBase64, nullptr) == 0) {
                break;
            }

            DWORD64 addr = stackFrame.AddrPC.Offset;
            if (addr == 0) break;

            auto ghidraPoint = GetModuleLocation(addr);

            char symbolBuffer[sizeof(SYMBOL_INFO) + 256] = {};

            auto *pSymbol = reinterpret_cast<PSYMBOL_INFO>(symbolBuffer);
            pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            pSymbol->MaxNameLen = 255;

            DWORD64 displacement = 0;
            if (SymFromAddr(process, addr, &displacement, pSymbol) != 0) {
                stackTrace << "  " << std::setw(2) << std::setfill('0') << i
                        << ": " << pSymbol->Name
                        << " + 0x" << std::hex << std::uppercase << displacement
                        << std::dec << std::nouppercase << " at " << ghidraPoint.first.c_str() << " 0x" << std::hex << (
                            ghidraPoint.second + displacement) << "\n";
            } else {
                stackTrace << "  " << std::setw(2) << std::setfill('0') << i
                        << ": 0x" << std::setw(16) << std::setfill('0') << std::hex << std::uppercase << addr
                        << std::dec << std::nouppercase << " at " << ghidraPoint.first.c_str() << " 0x" << std::hex << (
                            ghidraPoint.second + displacement) << "\n";
            }
        }

        SymCleanup(process);

        return stackTrace.str();
    }

    bool IsDebugException(const DWORD kCode) {
        switch (kCode) {
            case 0x40010006: // DBG_PRINTEXCEPTION_C (OutputDebugString)
            case 0x4001000A: // DBG_RIPEXCEPTION
            case 0x40010007: // DBG_RIPEXCEPTION (older/misc variant)
            case 0x40010005: // DBG_COMMAND_EXCEPTION (used internally)
            case 0x406D1388: // MS_VC_EXCEPTION (SetThreadName)
                return true;
            default:
                return false;
        }
    }

    LONG CALLBACK VectoredCrashHandler(PEXCEPTION_POINTERS pExceptionInfo) {
        if (IsDebugException(pExceptionInfo->ExceptionRecord->ExceptionCode)) {
            return EXCEPTION_CONTINUE_EXECUTION;
        }

        std::ofstream log(ModLoader::GetInstance().mModulePath / "crash.log", std::ios::app);

        if (!log.is_open()) {
            return EXCEPTION_CONTINUE_EXECUTION;
        }

        MSML_LOG_ERROR("Game crashed, check crash.log");

        log << "=== CRASH LOG ===\n";

        const auto kExpectionLocation = GetModuleLocation(
            reinterpret_cast<uintptr_t>(pExceptionInfo->ExceptionRecord->ExceptionAddress));

        MSML_LOG_ERROR("Exception occurred: %s 0x%p", kExpectionLocation.first.c_str(), kExpectionLocation.second);

        log << "\tException occurred: " << kExpectionLocation.first.c_str() << " 0x" << std::hex << kExpectionLocation.
                second
                << "\n";
        const DWORD kExceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;
        MSML_LOG_ERROR("\t%s (0x%p)", GetExceptionTypeName(kExceptionCode).c_str(), kExceptionCode);

        log << "\tException type: " << GetExceptionTypeName(kExceptionCode).c_str() << "(" << std::hex << kExceptionCode
                <<
                ")\n";

        log << "\n=== STACK TRACE ===\n";
        std::string stackTrace = GetStackTrace(pExceptionInfo);
        log << stackTrace;
        log << "\n\n\n";

        log.close();
        return EXCEPTION_CONTINUE_SEARCH;
    }
}
