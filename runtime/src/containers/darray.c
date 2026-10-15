#include "darray.h"
#include "core/logger.h"
#include "core/memory.h"

DArray darray_create(u64 count, usize size) {
	DArray array = {0};

	array.count = count;
	array.capacity = count;
	array.size = size;
	array.data = mem_calloc(count, size);

	return array;
}

void darray_destroy(DArray* array) {
	array->count = 0;
	array->capacity = 0;
	array->size = 0;
	mem_free(array->data, sizeof(array->data));
}

b8 darray_insert(DArray* array, const void* value) {
	if (array->count >= array->capacity) {
		u64 new_capacity = array->capacity ? array->capacity * 2 : 2;
		void* new_data = mem_realloc(array->data, array->capacity, new_capacity * array->size);
		array->data = new_data;
		array->capacity = new_capacity;
	}

	mem_copy((char*)array->data + array->count * array->size, value, array->size);
	array->count++;
	return TRUE;
}

b8 darray_insert_at(DArray* array, const void* value, u64 index) {
	if (index >= array->count) {
		LOG_ERROR("Index outside the bounds of this array, length: %i, index: %i", array->count, index);
		return FALSE;
	}
	if (array->count >= array->capacity) {
		u64 new_capacity = array->capacity ? array->capacity * 2 : 2;
		void* new_data = mem_realloc(array->data, array->capacity, new_capacity * array->size);
		array->data = new_data;
		array->capacity = new_capacity;
	}
	mem_move((char*)array->data + (index + 1) * array->size, (char*)array->data + index * array->size, (array->count - index) * array->size);

	mem_copy((char*)array->data + index * array->size, value, array->size);
	array->count++;
	return TRUE;
}

void* darray_get(DArray* array, u64 index) {
	if (index >= array->count) {
		LOG_ERROR("Index outside the bounds of this array, length: %i, index: %i", array->count, index);
		return array;
	}
	return (char*)array->data + index * array->size;
}
