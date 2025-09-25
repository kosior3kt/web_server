#ifndef __HEAP__
#define __HEAP__

#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "../utils.h"
#include "../defines.h"
#include "../allocators/arena.h"
#include "dynamic_array.h"


////////////////////////// TYPEDEFS
typedef struct {
	dynamic_array_s data;
	uint current_size, max_size;
	bool (*predicate)(void*, void*);
}min_heap_s;

typedef struct {
	dynamic_array_s data;
	uint current_size, max_size;
	bool (*predicate)(void*, void*);
}max_heap_s;

typedef enum {
	UNDEFINED = 0,
	MAX,
	MIN,
}heap_e;

////////////////////////// DECLARATIONS
static void swap_elems_(void**, void**);

static void min_heap_hipify_(min_heap_s*);
static void max_heap_hipify_(max_heap_s*);

void  min_heap_init(min_heap_s*, const uint, arena_s*, bool(*)(void*, void*));
void  min_heap_add(min_heap_s*, void*);
void* min_heap_get(min_heap_s*, uint);
void* min_heap_pop(min_heap_s*);	/// get first element
void* min_heap_peak(min_heap_s*, uint);
void  min_heap_print(min_heap_s*);

void  max_heap_init(max_heap_s*, const uint, arena_s*, bool(*)(void*, void*));
void  max_heap_add(max_heap_s*, void*);
void* max_heap_get(max_heap_s*, uint);
void* max_heap_pop(max_heap_s*);	/// get first element
void* max_heap_peak(max_heap_s*, uint);
void  max_heap_print(min_heap_s*);

////////////////////////// DEFINITIONS
static void swap_elems_(void** _a, void** _b)
{
	void** temp = _a;
	*_a = *_b;
	*_b = *temp;
}


/// min
void min_heap_init(min_heap_s* _heap, const uint _size, arena_s* _arena, bool(*_predicate)(void*, void*))
{
	da_init(&_heap->data, _size, _arena);
	_heap->current_size = 0;
	_heap->max_size = _size;
}

void min_heap_add(min_heap_s* _heap, void* _value)
{
	//just add at the end of array and heapify later. yeah!
	if(_heap->current_size >= _heap->max_size) {
		perror("resize this fucker (min heap add)\n");
		assert(0);
	}
	da_add_at(&_heap->data, _heap->current_size, _value);
	++_heap->current_size;

	// now heapify
	int index = _heap->current_size;
	while(index > 0 && _heap->predicate(da_get_at(&_heap->data, index), da_get_at(&_heap->data, (index - 1)/2))) {
		void* temp = da_get_at(&_heap->data, index);
		da_add_at(&_heap->data, index, da_get_at(&_heap->data, (index - 1)/2));
		da_add_at(&_heap->data, (index - 1)/2, temp);
		index = (index - 1)/2;
	}
}

void* min_heap_get(min_heap_s* _heap, uint _index)
{
	//idk, check if it's zero or sth...

	void* ret_val = da_get_at(&_heap->data, _index);
	return ret_val;
}


void* min_heap_pop(min_heap_s* _heap)
{
	//idk, check if it's zero or sth...

	void* ret_val = min_heap_get(_heap, 0);
	da_add_at(&_heap->data, _index, da_get_at(&_heap->data, _heap->current_size));
	da_add_at(&_heap->data, _heap->current_size, 0);
	--_heap->current_size;
	//heapify later
	return ret_val;
}



void min_heap_print(min_heap_s* _heap)
{
	da_print(&_heap->data);
}


/// max
void max_heap_init(max_heap_s* _heap, const uint _size, arena_s* _arena, bool(*_predicate)(void*, void*))
{
	da_init(&_heap->data, _size, _arena);
	_heap->current_size = 0;
	_heap->max_size = _size;
}

void max_heap_add(max_heap_s* _heap, void* _value)
{
	//just add at the end of array and heapify later. yeah!
	if(_heap->current_size >= _heap->max_size) {
		perror("resize this fucker (max heap add)\n");
		assert(0);
	}
	da_add_at(&_heap->data, _heap->current_size, _value);
	++_heap->current_size;

	// now heapify
	int index = _heap->current_size;
	while(index > 0 && _heap->predicate(da_get_at(&_heap->data, index), da_get_at(&_heap->data, (index - 1)/2))) {
		void* temp = da_get_at(&_heap->data, index);
		da_add_at(&_heap->data, index, da_get_at(&_heap->data, (index - 1)/2));
		da_add_at(&_heap->data, (index - 1)/2, temp);
		index = (index - 1)/2;
	}
}


void max_heap_print(min_heap_s* _heap)
{
	da_print(&_heap->data);
}

#endif //__HEAP__
