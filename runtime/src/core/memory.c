#include "memory.h"
#include "core/logger.h"
#include <stdlib.h>
#include <string.h>

void* mem_alloc(usize size) {
	void* chunk = malloc(size);
	memset(chunk, 0, size);

	LOG_TRACE("Allocated %zu bytes at address: %p", size, chunk);
	return chunk;
}

void mem_free(void* chunk, usize size) {
	free(chunk);
	LOG_TRACE("Freed %zu bytes from address: %p", size, chunk);
}

void* mem_calloc(u64 count, usize size) {
	u64 total = count * size;

	void* chunk = malloc(total);
	memset(chunk, 0, total);

	LOG_TRACE("Allocated %zu bytes at address: %p", total, chunk);
	return chunk;
}

void* mem_realloc(void* chunk, usize size, usize newsize) {
	return realloc(chunk, newsize);
}

void* mem_set(void* dest, i32 value, usize size) {
	return memset(dest, value, size);
}
