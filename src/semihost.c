
#include <semihost.h>

const char SEMIHOST_STDIO_FILE[] = ":ff";
const char SEMIHOST_FEATURE_FILE[] = ":semihosting-features";

semihost_value_t semihost_syscall(semihost_operator_t op, semihost_value_t val)
{
    semihost_value_t result;
#if ARM_SEMIH_ENABLE == 1
#if AARCH == 32
#ifdef __thumb__
    asm volatile("mov r0, %1\n"
                 "mov r1, %2\n"
                 "hlt 0x3C\n"
                 "mov %0, r0\n"

                 : "=r"(result)
                 : "r"(op), "r"(val)
                 : "r0", "r1");
#else
    asm volatile("mov r0, %1\n"
                 "mov r1, %2\n"
                 "hlt 0xF000\n"
                 "mov %0, r0\n"

                 : "=r"(result)
                 : "r"(op), "r"(val)
                 : "r0", "r1");
#endif // ARMPROFILE
#else  // AARCH
    asm volatile("mov w0, %1\n"
                 "mov x1, %2\n"
                 "hlt 0xF000\n"
                 "mov %0, x0\n"

                 : "=r"(result)
                 : "r"(op), "r"(val)
                 : "x0", "x1", "w0");
    // W0 actually maps to the lower 16-Bit of X0, but just to be safe it's
    // added the the cobbled registers
#endif // AARCH
#else
    result = 0;
#endif // ARM_SEMIH_ENABLE
    return result;
}

int8_t semihost_check_featurebit(uint8_t featrueBit, int featureByte)
{
#if ARM_SEMIH_ENABLE == 1
    char magicArr[] = {0, 0, 0, 0};
    char buffer;
    semihost_value_t param[] = {(semihost_value_t)SEMIHOST_FEATURE_FILE, SEMIMODE_READ,
                                (semihost_value_t)sizeof(SEMIHOST_FEATURE_FILE) - 1};
    semihost_value_t fd;
    size_t flen;
    int8_t result = 0;

    if (featrueBit >= 8 || featureByte < 0)
    {
        return -1;
    }

    fd = semihost_syscall(SEMISYS_OPEN, (semihost_value_t)param);
    if (fd == -1)
    {
        return -1;
    }

    param[0] = fd;
    param[1] = (semihost_value_t)magicArr;
    param[2] = 4;
    if (semihost_syscall(SEMISYS_READ, (semihost_value_t)param) != 0 || magicArr[0] != SEMIFB_MAGIC_0 ||
        magicArr[1] != SEMIFB_MAGIC_1 || magicArr[2] != SEMIFB_MAGIC_2 || magicArr[3] != SEMIFB_MAGIC_3)
    {
        result = -1;
    }

    if (result == 0)
    {
        flen = semihost_syscall(SEMISYS_FLEN, (semihost_value_t)&fd);
        if (flen < 0)
        {
            result = -1;
        }
        else if (flen <= (featureByte + 4))
        {
            result = 1;
        }
    }

    if (result == 0 && featureByte != 0)
    {
        param[0] = fd;
        param[1] = 4 + featureByte;
        if (semihost_syscall(SEMISYS_SEEK, (semihost_value_t)param) != 0)
        {
            result = -1;
        }
    }

    if (result == 0)
    {
        param[0] = fd;
        param[1] = (semihost_value_t)&buffer;
        param[2] = 1;
        if (semihost_syscall(SEMISYS_READ, (semihost_value_t)param) == 0)
        {
            result = (buffer & (1 << featrueBit)) ? 0 : 1;
        }
        else
        {
            result = 1;
        }
    }

    semihost_syscall(SEMISYS_CLOSE, (semihost_value_t)&fd);
    return result;
#else
    return -1;
#endif
}

void semihost_exit(int32_t exc, int32_t subc)
{
#if ARM_SEMIH_ENABLE == 1
    semihost_value_t param[] = {exc, subc};

    if (semihost_check_featurebit(0, 0) == 0)
    {
        semihost_syscall(SEMISYS_EXIT_EXTENDED, (semihost_value_t)param);
    }
    else
    {
        semihost_syscall(SEMISYS_EXIT, (semihost_value_t)param);
    }
#endif
}