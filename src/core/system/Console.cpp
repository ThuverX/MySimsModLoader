//
// Created by exozg on 24/01/2025.
//

#include "Console.h"

#include <cstdio>
#include <iostream>
#include <windows.h>

#include "../../Version.h"

class outbuf : public std::streambuf {
public:
    outbuf() {
        setp(nullptr, nullptr);
    }

    int_type overflow(int_type c = traits_type::eof()) override {
        return fputc(c, stdout) == EOF ? traits_type::eof() : c;
    }
};

outbuf obuf;
std::streambuf *sb = nullptr;

namespace msml::core::system {
    void Console::Enable() {
        if (isEnabled) return;

        if (!AllocConsole())
            return;

        SetConsoleTitle("MSML " MSML_VERSION);

        FILE *tmp;
        freopen_s(&tmp, "conout$", "w", stdout);
        freopen_s(&tmp, "conout$", "w", stderr);
        freopen_s(&tmp, "CONIN$", "r", stdin);

        sb = std::cout.rdbuf(&obuf);

        isEnabled = true;
    }

    void Console::Disable() {
        if (!isEnabled) return;

        std::cout.rdbuf(sb);
        FreeConsole();

        isEnabled = false;
    }

    Console::~Console() {
        Disable();
    }
}
