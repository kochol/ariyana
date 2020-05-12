#ifndef ARI_MACROS_H
#define ARI_MACROS_H

#include <stdio.h>
#include <stdint.h>
#if defined _WIN32 || defined __CYGWIN__
    #ifdef CARI_NO_EXPORT
        #define API
    #else
        #define API __declspec(dllexport)
    #endif
#else
    #ifdef __GNUC__
        #define API  __attribute__((__visibility__("default")))
    #else
        #define API
    #endif
#endif

#if defined __cplusplus
    #define EXTERN extern "C"
#else
    #include <stdarg.h>
    #include <stdbool.h>
    #define EXTERN extern
#endif

#define CARI_API EXTERN API
#define CONST const

#define CARI_HANDLE(_name) \
struct _name \
{ \
    uint32_t Handle; \
	uint32_t Index; \
};

#define CARI_COMPONENT_HANDLE(_name) \
struct _name \
{ \
    uint32_t Handle; \
	uint32_t Index; \
    void* Component; \
};

#endif // ARI_MACROS_H
