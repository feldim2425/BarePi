#include <barepi/console/console.h>
#include <barepi/console/semiconsole.h>

int console_open(console_type_t type, console_t *console)
{
    if (type == CONSOLET_SEMIHOST)
        return semiconsole_handle(console);

    return -1;
}
void console_close(console_t *console)
{
    if (console->_closeh)
        console->_closeh(console);
    console->flags = 0;
}

int console_puts(char *s, console_t *console, bool error)
{
    if (error && console->error.puts)
        return console->error.puts(s, console);
    else if (console->output.puts)
        return console->output.puts(s, console);
    return -1;
}

int console_putc(int c, console_t *console, bool error)
{
    if (error && console->error.putc)
        return console->error.putc(c, console);
    else if (console->output.putc)
        return console->output.putc(c, console);
    return -1;
}

int console_gets(char *s, int num, console_t *console)
{
    if (console->input.gets)
        return console->input.gets(s, num, console);
    return -1;
}

int console_getc(console_t *console)
{
    if (console->input.getc)
        return console->input.getc(console);
    return -1;
}