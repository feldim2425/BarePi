#include <barepi/semihost.h>
#include <stdlib.h>

typedef void (*initf_t)(void);

extern int main(void);

extern initf_t __preinit_array_start;
extern initf_t __preinit_array_end;

extern initf_t __init_array_start;
extern initf_t __init_array_end;

extern initf_t __fini_array_start;
extern initf_t __fini_array_end;

void __attribute__((noreturn)) _exit(int exitcode) 
{
    m_data_sync_barrier();
    semihost_exit(ADP_Stopped_ApplicationExit, exitcode);

    while (1)
        asm volatile("wfi");
}

void _sysinit(uword_t machineId, uword_t atags)
{
    initf_t *it;

    for (it = &__preinit_array_start; it < &__preinit_array_end; ++it)
        (**it)();

    for (it = &__init_array_start; it < &__init_array_end; ++it)
        (**it)();
    
    exit(main());
}

void _sysinit2()
{
    while (1)
        asm volatile("wfi");
}
