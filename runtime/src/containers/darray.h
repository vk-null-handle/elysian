#pragma once
#include "defines.h"

typedef struct darray {
  u64 count;
  u64 capacity;
  usize size;
  void* data;
} DArray;

DArray darray_create(u64 count, usize size);
b8 darray_insert(DArray* array, const void* value);
b8 darray_insert_at(DArray* array, const void* value, u64 index);

void* darray_get(DArray* array, u64 index);
