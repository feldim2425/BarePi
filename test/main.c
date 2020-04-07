#include <barepi/console/console.h>

int main(void)
{
    console_t con;
    console_open(CONSOLET_SEMIHOST, &con);
    console_puts("Hello World from main()\n", &con, false);
    console_close(&con);
    return 0;
}