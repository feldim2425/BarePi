#include <barepi/console/semiconsole.h>
#include <stdbool.h>
#include <string.h>

static bool _console_isopen = false;
static semiconsole_handleargs_t _console_fds = {0, 0};

static int _semiconsole_puts(char *s, semihost_value_t out_fd)
{
    semihost_value_t param[] = {out_fd, (semihost_value_t)s, strlen(s)};
    return semihost_syscall(SEMISYS_WRITE, (semihost_value_t)param);
}

static int _semiconsole_putc(char c, semihost_value_t out_fd)
{
    semihost_value_t param[] = {out_fd, (semihost_value_t)&c, 1};
    return semihost_syscall(SEMISYS_WRITE, (semihost_value_t)param);
}

static int _semiconsole_puts_out(char *s, console_t *out_fd)
{
    return _semiconsole_puts(s, _console_fds.out_fd);
}

static int _semiconsole_putc_out(char c, console_t *out_fd)
{
    return _semiconsole_putc(c, _console_fds.out_fd);
}

static int _semiconsole_puts_err(char *s, console_t *out_fd)
{
    return _semiconsole_puts(s, _console_fds.err_fd);
}

static int _semiconsole_putc_err(char c, console_t *out_fd)
{
    return _semiconsole_putc(c, _console_fds.err_fd);
}

bool semiconsole_init()
{
    semihost_value_t param[3] = {0};
    if (_console_isopen)
        return true;

    param[0] = (semihost_value_t)SEMIHOST_STDIO_FILE;
    param[2] = SEMIHOST_STDIO_FILELEN;

    param[1] = SEMIMODE_WRITE;
    _console_fds.out_fd = semihost_syscall(SEMISYS_OPEN, (semihost_value_t)param);
    if (_console_fds.out_fd <= 0)
        return false;

    if (semihost_check_featurebit(SEMIFEAT_STDOUT_STDERR, 0) == 0)
    {
        param[1] = SEMIMODE_APPEND;
        _console_fds.err_fd = semihost_syscall(SEMISYS_OPEN, (semihost_value_t)param);
        if (_console_fds.err_fd <= 0)
            return false;
    }
    else
        _console_fds.err_fd = _console_fds.out_fd;

    _console_isopen = true;
    return true;
}
void semiconsole_deinit()
{
    if (!_console_isopen)
        return;

    if (_console_fds.err_fd != _console_fds.out_fd)
        semihost_syscall(SEMISYS_CLOSE, _console_fds.err_fd);
    semihost_syscall(SEMISYS_CLOSE, _console_fds.out_fd);

    _console_isopen = false;
}

int semiconsole_handle(console_t *console)
{
    if (!_console_isopen && !semiconsole_init())
        return -1;
    console->flags = CONSFLAG_OPEN | CONSFLAG_OUTPUT;
    console->type = CONSOLET_SEMIHOST;

    if (_console_fds.err_fd != _console_fds.out_fd)
    {
        console->flags |= CONSFLAG_DEDICATED_ERROR;
    }

    console->_closeh = NULL;
    console->_handle_arg = NULL;

    console->output.puts = _semiconsole_puts_out;
    console->output.putc = _semiconsole_putc_out;
    console->error.puts = _semiconsole_puts_err;
    console->error.putc = _semiconsole_putc_err;
    console->input.gets = NULL;
    console->input.getc = NULL;
    return 0;
}