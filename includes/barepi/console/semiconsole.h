#ifndef __BAREPI_SEMICONSOLE_H
#define __BAREPI_SEMICONSOLE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <barepi/console/console.h>
#include <barepi/semihost.h>

    typedef struct
    {
        semihost_value_t out_fd;
        semihost_value_t err_fd;
    } semiconsole_handleargs_t;

    bool semiconsole_init();
    void semiconsole_deinit();
    int semiconsole_handle(console_t *console);

#ifdef __cplusplus
}
#endif

#endif