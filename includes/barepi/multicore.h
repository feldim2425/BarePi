#ifndef __BAREPI_MULTICORE_H
#define __BAREPI_MULTICORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

uint8_t get_coreid();

void mutex_initializeStatic();
void mutex_initialize();

bool mutex_is_locked();
void mutex_lock();
bool mutex_try_lock();
void mutex_unlock();

#ifdef __cplusplus
}
#endif

#endif