#ifndef __BAREPI_ARCHDEF_H
#define __BAREPI_ARCHDEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdlib.h>

void* malloc(size_t size);
void* calloc(size_t num, size_t size);

#ifdef __cplusplus
}
#endif

#endif