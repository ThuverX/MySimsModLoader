//
// Created by exozg on 24/01/2025.
//

#include "Console.h"

#include <cstdio>
#include <iostream>
#include <windows.h>

#include "../../Version.h"

class Outbuf : public std::streambuf {
public:
    Outbuf() {
        setp(nullptr, nullptr);
    }

    int_type overflow(int_type c = traits_type::eof()) override {
        return fputc(c, stdout) == EOF ? traits_type::eof() : c;
    }
};

namespace Msml::Core::System {
    static Outbuf sObuf;
    static std::streambuf *sStreamBuffer = nullptr;

    void Console::Enable() {
        if (mIsEnabled) {
            return;
        }

        if (AllocConsole() == 0) {
            return;
        }

        SetConsoleTitle("MSML " MSML_VERSION);

        FILE *tmp = nullptr;
        freopen_s(&tmp, "conout$", "w", stdout);
        freopen_s(&tmp, "conout$", "w", stderr);
        freopen_s(&tmp, "CONIN$", "r", stdin);

        sStreamBuffer = std::cout.rdbuf(&sObuf);

        mIsEnabled = true;
    }

    void Console::Disable() {
        if (!mIsEnabled) {
            return;
        }

        std::cout.rdbuf(sStreamBuffer);
        FreeConsole();

        mIsEnabled = false;
    }

    Console::~Console() {
        Disable();
    }
}
