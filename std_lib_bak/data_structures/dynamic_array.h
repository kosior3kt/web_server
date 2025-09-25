#ifndef __DYNAMIC_ARRAY__
#define __DYNAMIC_ARRAY__

#include <stdbool.h>
#include <string.h>
#include "../utils.h"
#include "../defines.h"
#include "../allocators/arena.h"

#define DEFAULT_SIZE 128

typedef struct {
	uint max_size;
	uint current_size;
	void** data;
	bool uses_arena_;
	arena_s* arena_;
}dynamic_array_s;

///////////////////////////// DECLARATIONS
void  da_init(dynamic_array_s*, const uint, arena_s*);
void  da_free(dynamic_array_s*);
void  da_resize(dynamic_array_s*, const int);
void  da_append(dynamic_array_s*, void*);
void* da_get_at(dynamic_array_s*, const uint);
void  da_add_at(dynamic_array_s*, const uint, void*);
void  da_print(dynamic_array_s*);

///////////////////////////// IMPLEMENTATIONS
void da_init(dynamic_array_s* _array, const uint _size, arena_s* _arena)
{
	uint real_size = (_size != 0) ? _size : DEFAULT_SIZE;
	assert(real_size > 0);
	assert(real_size != 0);

	_array->arena_ = _arena;	//it's fine if it's NULL
	_array->uses_arena_ = (_array->arena_ != NULL) ? true : false;

	_array->max_size = real_size;
	_array->current_size = 0;

	if(_array->uses_arena_) {
		assert(_array->arena_ != NULL);	//shouldn't be possible
		_array->data = (void*)get_raw_memory(_array->arena_, sizeof(void*) * real_size);
	} else {
		_array->data = (void*)malloc(sizeof(void*) * real_size);
	}
	assert(_array->data != NULL);
}

void da_free(dynamic_array_s* _array)
{
	assert(_array != NULL);
	assert(_array->data != NULL);

	if(!_array->uses_arena_) {
		free(_array->data);
	}
}

void da_resize(dynamic_array_s* _array, const int _new_size)
{
	assert(_array != NULL);
	assert(_array->data != NULL);

	if(_new_size <= _array->max_size) return;

	if(_array->uses_arena_) {
		_array->data = (void**)arena_resize(_array->arena_, _array->data, _array->max_size, _new_size, sizeof(void*));
		//printf("array uses arena\n");
	} else {
		//printf("array doesnt use arena\n");
		void* to_be_cleaned = _array->data;
		_array->data = (void**)malloc(sizeof(void) * _new_size);
		memmove(_array->data, to_be_cleaned, sizeof(void) * _array->current_size);	//should not be overlapping;
		free(to_be_cleaned);
	}

	if(_array->current_size >= _new_size) {
		_array->current_size = _new_size;	//only relevant when decresing size
	}
	_array->max_size = _new_size;
	assert(_array->data != NULL);
}

void da_append(dynamic_array_s* _array, void* _val)
{
	assert(_array != NULL);
	assert(_array->data != NULL);

	while(_array->current_size >= _array->max_size) {
		da_resize(_array, _array->max_size * 2);
	}	//idk how fucked up it would need to be to require while instead of if...

	_array->data[_array->current_size] = _val;
	++(_array->current_size);
}

void da_add_at(dynamic_array_s* _array, const uint _index, void* _val)
{
	assert(_array != NULL);
	assert(_array->data != NULL);
	assert(_array->max_size > _index);

	if(_index >= _array->current_size) _array->current_size = _index + 1;
	_array->data[_index] = _val;
}

void* da_get_at(dynamic_array_s* _array, const uint _index)
{
	assert(_array != NULL);
	assert(_array->data != NULL);
	assert(_array->max_size > _index);

	return _array->data[_index];
}

void da_print(dynamic_array_s* _array)
{
	for(int i = 0; i < _array->current_size; ++i) {
		printf("%d, ", da_get_at(_array, i));
	}
	printf("\n");
	return;
}

#endif//__DYNAMIC_ARRAY__
