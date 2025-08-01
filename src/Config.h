//
// Created by exozg on 20/03/2025.
//

#ifndef CONFIG_H
#define CONFIG_H

#define MODS_PATH "mods/"
#define W_MODS_PATH L"mods/"

#ifdef _WIN64
#define PLATFORM_WIN64

#define DATA_PATH "data"
#define W_DATA_PATH L"data"

#define CATCH_EDX

#else
#define PLATFORM_WIN32

#define DATA_PATH "../SimsRevData"
#define W_DATA_PATH L"../SimsRevData"

#define CATCH_EDX , void *_EDX

#endif

#ifndef NDEBUG

#define BUILD_DEBUG

#else

#define BUILD_RELEASE

#endif

#endif //CONFIG_H
