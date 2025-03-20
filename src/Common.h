//
// Created by exozg on 20/03/2025.
//

#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN64
#define CATCH_ECX
#else
#define CATCH_ECX ,void *_ECX
#endif

#endif //COMMON_H
