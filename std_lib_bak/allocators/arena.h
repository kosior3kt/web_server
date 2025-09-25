#ifndef __ARENA__
#define __ARENA__

#define DEFAULT_CAPACITY 1024

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct region {
    struct region* next;
    uint8_t *data;
    unsigned max_size;
    unsigned current_size;
} region_s_;

typedef struct arena {
    region_s_* start;
    region_s_* end;
} arena_s;

static void
init_region_(region_s_* _region, const unsigned _capacity)
{
    _region->data = (uint8_t*)malloc(_capacity * sizeof(uint8_t));
    memset(_region->data, '\0', _capacity);
    _region->current_size = 0;
    _region->max_size     = _capacity;
    _region->next         = NULL;
}

static void
free_region_(region_s_** _region)
{
    if (*_region != NULL) {
        free((*_region)->data);
        free(*_region);
        *_region = NULL;
    }
}

static void
arena_init(arena_s* _arena)
{
    region_s_* starting_region = (region_s_*)malloc(sizeof(region_s_));
    init_region_(starting_region, DEFAULT_CAPACITY);
    _arena->start = starting_region;
    _arena->end   = starting_region;
}

static void
arena_free(arena_s* _arena)
{
    region_s_* current_region = _arena->start;
    while(current_region) {
        region_s_* to_be_removed = current_region;
        current_region = current_region->next;
		if(to_be_removed->current_size)
        free_region_(&to_be_removed);
    }
    _arena->start = _arena->end = NULL;
}

static void*
get_raw_memory(arena_s* _arena, const unsigned _amount)
{
    region_s_* current_region = _arena->start;
    while (current_region->current_size + _amount > current_region->max_size) {
        if (current_region->next != NULL) {
            current_region = current_region->next;
			continue;
        }
		region_s_* new_region = (region_s_*)malloc(sizeof(region_s_));
		{
			if (_amount > DEFAULT_CAPACITY) init_region_(new_region, _amount);
			else							init_region_(new_region, DEFAULT_CAPACITY);
		}
		current_region->next = new_region;
		_arena->end = new_region;
		current_region = new_region;
    }
    unsigned temp_size = current_region->current_size;
    current_region->current_size += _amount;
    return (void*)(current_region->data + temp_size);
}


static void*
arena_resize(arena_s* _arena,
			void* _old_pointer,
			const unsigned _old_size,
			const unsigned _new_size,
			const unsigned _data_size)
{
	assert(_old_pointer != NULL);
	assert(_arena != NULL);

	if(_old_size >= _new_size) return _old_pointer;

	void* new_pointer = get_raw_memory(_arena, _new_size * _data_size);
	assert(new_pointer != NULL);

	//region guarantees that they do not overlap
	memmove(new_pointer, _old_pointer, _old_size * _data_size);
	return new_pointer;
}


static char*
arena_concat_string(arena_s* _arena, const char* _first, const char* _second)
{
	///if these are not null terminated then unlucky
	unsigned alloc_size = strlen(_first) + strlen(_second) + 1;
	char* new_string = get_raw_memory(_arena, alloc_size * sizeof(char));

	unsigned i = 0;
	while(_first[i] != '\0') {
	//for(i = 0; _first[i] != '\0'; ++i) {
		new_string[i] = _first[i];
		++i;
	}

	unsigned j = 0;
	for(j = 0; _second[j] != '\0'; ++j) {
		new_string[i + j] = _second[j];
	}
	//new_string[i + j + 1] = '\0';
	return new_string;
}

#undef DEFAULT_CAPACITY
#endif// __ARENA__
