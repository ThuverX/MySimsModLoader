//
// Created by exozg on 13/04/2025.
//

#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include <windows.h>

namespace msml::core::system {
    LONG CALLBACK VectoredCrashHandler(PEXCEPTION_POINTERS pExceptionInfo);
}

#endif //CRASHHANDLER_H
