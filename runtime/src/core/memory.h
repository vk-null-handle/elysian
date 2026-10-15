#pragma once
#include "defines.h"

void* mem_alloc(usize size);
void mem_free(void* chunk, usize size);

void* mem_calloc(u64 count, usize size);
void* mem_realloc(void* chunk, usize size, usize newsize);
void* mem_copy(void* dest, const void* source, u64 size);
void* mem_move(void* dest, const void* src, u64 count);
void* mem_set(void* dest, i32 value, usize size);
