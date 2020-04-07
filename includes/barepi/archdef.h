#ifndef __BAREPI_ARCHDEF_H
#define __BAREPI_ARCHDEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#ifndef PIBOARD
#define PIBOARD 2
#endif

#ifndef AARCH
#ifdef __aarch64__
#define AARCH 64
#else
#define AARCH 32
#endif
#endif

#if AARCH == 32
    typedef uint32_t uword_t;
#define MAX_UWORD_VALUE 4294967295UL
#else
typedef uint64_t uword_t;
#define MAX_UWORD_VALUE 18446744073709551615UL
#endif

#if PIBOARD == 1

#define m_data_memory_barrier() asm volatile("p15, 0, %0, c7, c10, 5" ::"r"(0) : "memory")
#define m_data_sync_barrier() asm volatile("p15, 0, %0, c7, c10, 4" ::"r"(0) : "memory")
#define m_instruction_barrier() asm volatile("isb" ::: "memory")

#else

#define m_data_memory_barrier() asm volatile("dmb" ::: "memory")
#define m_data_sync_barrier() asm volatile("dsb" ::: "memory")
#define m_instruction_barrier() asm volatile("isb" ::: "memory")

#endif



#ifdef __cplusplus
}
#endif

#endif