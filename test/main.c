#include <barepi/console/console.h>
#include <stdlib.h>

int main(void)
{
    console_t* con = calloc(sizeof(console_t), 1);
    console_open(CONSOLET_SEMIHOST, con);
    console_puts("Hello World from main()\n", con, false);
    console_close(con);
    free(con);
    return 0;
}