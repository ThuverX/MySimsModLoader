//
// Created by exozg on 20/03/2025.
//

#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN64
#define CATCH_EDX
#else
#define CATCH_EDX , void *_EDX
#endif

#endif //COMMON_H
