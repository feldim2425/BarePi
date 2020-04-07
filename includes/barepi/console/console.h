#ifndef __BAREPI_CONSOLE_H
#define __BAREPI_CONSOLE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

    enum console_flagbits_enum
    {
        /// Set when the console handler is open and ready to use
        CONSFLAG_OPEN = 0x01,
        /// Set when the console has a readable input
        CONSFLAG_INPUT = 0x02,
        /// Set when the console has a writable output
        CONSFLAG_OUTPUT = 0x04,
        /// Used together with OUTPUT flag to indicate weather errors can be sent to a dedicated error stream
        CONSFLAG_DEDICATED_ERROR = 0x08
    };

    enum console_type_enum
    {
        CONSOLET_DUMMY = 0,
        CONSOLET_UART = 1,
        CONSOLET_SEMIHOST = 2
    };

    typedef struct console_struct console_t;
    typedef uint32_t console_type_t;
    typedef uint8_t console_flags_t;
    typedef void (*console_closehandle_t)(console_t *console);

    typedef struct
    {
        int (*puts)(char *s, console_t *console);
        int (*putc)(char c, console_t *console);
    } console_outhandle_t;

    typedef struct
    {
        int (*gets)(char *s, int num, console_t *console);
        int (*getc)(console_t *console);
    } console_inhandle_t;

    struct console_struct
    {
        console_type_t type;
        console_flags_t flags;
        console_outhandle_t output;
        console_outhandle_t error;
        console_inhandle_t input;
        console_closehandle_t _closeh;
        void *_handle_arg;
    };

    int console_open(console_type_t type, console_t *console);
    void console_close(console_t *console);

    int console_puts(char *s, console_t *console, bool error);
    int console_putc(int c, console_t *console, bool error);
    int console_gets(char *s, int num, console_t *console);
    int console_getc(console_t *console);

#ifdef __cplusplus
}
#endif

#endif