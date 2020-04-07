#include <barepi/archdef.h>
#include <barepi/multicore.h>

uint8_t get_coreid()
{
    uint8_t coreid;
#if AARCH == 32
    asm volatile (
        "mrc p15, 0, r0, c0, c0, 5\n"
        "and %0, r0, #3\n"
        : "=r" (coreid)
        :
        : "r0"
    );
#else
    asm volatile (
        "mrs x0, MPIDR_EL1\n"
        "and %0, x0, #0x7F\n"
        : "=r" (coreid)
        : 
        : "x0"
    );
#endif
    return coreid;
}