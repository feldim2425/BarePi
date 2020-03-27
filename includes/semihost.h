#ifndef __BAREPI_SEMIHOST_H
#define __BAREPI_SEMIHOST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef ARM_SEMIH_ENABLE
#define ARM_SEMIH_ENABLE 1
#endif

#include "archdef.h"

    enum semihost_magic_enum
    {
        SEMIFB_MAGIC_0 = 0x53,
        SEMIFB_MAGIC_1 = 0x48,
        SEMIFB_MAGIC_2 = 0x46,
        SEMIFB_MAGIC_3 = 0x42
    };

    enum semihost_feature_enum
    {
        SEMIFEAT_EXIT_EXTENDET = 0,
        SEMIFEAT_STDOUT_STDERR = 1
    };

    enum semihost_filemode_enum
    {
        SEMIMODE_READ = 0,
        SEMIMODE_READBIN = 1,
        SEMIMODE_READEXT = 2,
        SEMIMODE_READEXTBIN = 3,
        SEMIMODE_WRITE = 4,
        SEMIMODE_WRITEBIN = 5,
        SEMIMODE_WRITEEXT = 6,
        SEMIMODE_WRITEEXTBIN = 7,
        SEMIMODE_APPEND = 8,
        SEMIMODE_APPENDBIN = 9,
        SEMIMODE_APPENDEXT = 10,
        SEMIMODE_APPENDEXTBIN = 11,
    };

    enum semihost_operations_enum
    {
        SEMISYS_CLOCK = 0x10,
        SEMISYS_CLOSE = 0x02,
        SEMISYS_ELAPSED = 0x30,
        SEMISYS_ERRNO = 0x13,
        SEMISYS_EXIT = 0x18,
        SEMISYS_EXIT_EXTENDED = 0x20,
        SEMISYS_FLEN = 0x0C,
        SEMISYS_GET_CMDLINE = 0x15,
        SEMISYS_HEAPINFO = 0x16,
        SEMISYS_ISERROR = 0x08,
        SEMISYS_ISTTY = 0x09,
        SEMISYS_OPEN = 0x01,
        SEMISYS_READ = 0x06,
        SEMISYS_READC = 0x07,
        SEMISYS_REMOVE = 0x0E,
        SEMISYS_RENAME = 0x0F,
        SEMISYS_SEEK = 0x0A,
        SEMISYS_SYSTEM = 0x12,
        SEMISYS_TICKFREQ = 0x31,
        SEMISYS_TIME = 0x11,
        SEMISYS_TMPNAM = 0x0D,
        SEMISYS_WRITE = 0x05,
        SEMISYS_WRITEC = 0x03,
        SEMISYS_WRITE0 = 0x04
    };

    enum semihost_exceptions_enum
    {
        ADP_Stopped_BranchThroughZero = 0x20000,
        ADP_Stopped_UndefinedInstr = 0x20001,
        ADP_Stopped_SoftwareInterrupt = 0x20002,
        ADP_Stopped_PrefetchAbort = 0x20003,
        ADP_Stopped_DataAbort = 0x20004,
        ADP_Stopped_AddressException = 0x20005,
        ADP_Stopped_IRQ = 0x20006,
        ADP_Stopped_FIQ = 0x20007,

        ADP_Stopped_BreakPoint = 0x20020,
        ADP_Stopped_WatchPoint = 0x20021,
        ADP_Stopped_StepComplete = 0x20022,
        ADP_Stopped_RunTimeErrorUnknown = 0x20023,
        ADP_Stopped_InternalError = 0x20024,
        ADP_Stopped_UserInterruption = 0x20025,
        ADP_Stopped_ApplicationExit = 0x20026,
        ADP_Stopped_StackOverflow = 0x20027,
        ADP_Stopped_DivisionByZero = 0x20028,
        ADP_Stopped_OSSpecific = 0x20029
    };

    typedef uword_t semihost_value_t;
    typedef uint32_t semihost_operator_t;

    /**
     * This constant contains a string (0-terminated char array) that represents the Stdin, Stdout and Stderr files/streams
     * in the SEMISYS_OPEN call. \n
     * Opening this file with SEMIMODE_READ opens stdin \n
     * Opening this file with SEMIMODE_WRITE opens stdout \n
     * If the stdout_stderr extention is supported (bit: 1 / byte:0) the file can be opend with SEMIMODE_APPEND to get stderr
     */
    extern const char const SEMIHOST_STDIO_FILE[];
    /**
     * This constant contains a string (0-terminated char array) that represents feature file
     * in the SEMISYS_OPEN call
     */
    extern const char const SEMIHOST_FEATURE_FILE[];

    /**
     * Call a semihost syscall function. \n
     * This function does NOT do any checks weather a semihost is present, the return value might be completly random if the call did nothing. \n
     * This function only supports one argument. Multiple arguments are passed by setting the val parameter to a pointer to an array. \n
     * Please refer to the "Semihosting for AArch32 and AArch64" Document V3.0 for more information:
     * https://developer.arm.com/docs/100863/0300/the-semihosting-interface
     * 
     * \param op semihost operator to call
     * \param val argument to the semihost operator.
     */
    semihost_value_t semihost_syscall(semihost_operator_t op, semihost_value_t val);

    /**
     * Checks weather the Semihost supports a Extention sepecified by the bit and byte index. \n
     * The feature is supported if the function returns 0. A value below 0 indicates an error,
     * the error can be ignored and treated as unsupported by the calling function.
     * 
     * \param featureBit index of the featrue Bit. The function errors if the index is out of range
     * \param featureByte index of the byte containing the feature bit. The function returns 1 if the byte is out of range 
     * \return -1 if an error occured, 1 if the feature is NOT supported, 0 if it is supported
     */
    int8_t semihost_check_featurebit(uint8_t featureBit, int featureByte);

    /**
     * Call the semihost exit command. Automatically uses the extended exit if available
     * \param exc The exception code. Look into semihost_exceptions_enum for officially supported values
     * \param subc Sub code. Meaning depends on exc, for ADP_Stopped_ApplicationExit this is the return from the main function
     */
    void semihost_exit(int32_t exc, int32_t subc);

#ifdef __cplusplus
}
#endif

#endif