#ifndef __BAREPI_ARCHDEF_H
#define __BAREPI_ARCHDEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef __aarch64__
#define AARCH 64
#else
#define AARCH 32
#endif

#if AARCH == 32
typedef uint32_t uword_t;
#define MAX_UWORD_VALUE 4294967295UL
#else
typedef uint64_t uword_t;
#define MAX_UWORD_VALUE 18446744073709551615UL
#endif

#ifdef __cplusplus
}
#endif

#endif