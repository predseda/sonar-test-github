#ifndef COMMON_H
#define COMMON_H

// common headers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Windows headers
#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#endif


#ifdef _WIN32
// Windows functions
HANDLE SHMEM;

void create_shared_memory();
void remove_shared_memory();
#else
// POSIX functions
int32_t* open_shared_memory();
void clean_shared_memory();
void close_shared_memory(int* memory);
#endif

// common functions
int32_t init_shared_memory();
int32_t modify_shared_memory(int32_t value);
void print_shared_memory_value();

#endif
