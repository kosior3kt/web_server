#ifndef __QUEUE__
#define __QUEUE__

#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "../utils.h"
#include "../defines.h"
#include "../allocators/arena.h"
#include "dynamic_array.h"


///////////////////////////// EXAMPLE
/*

arena_s arena;
arena_init(&arena);

queue_s queue_arena, queue_malloc;
queue_init(&queue_malloc, <size>, NULL);
queue_init(&queue_arena, <size>, arena);

queue_add(&queue_malloc, <value>);
int returned_value = queue_pop(&queue_malloc);
*/

typedef struct {
	dynamic_array_s data_;
	uint head, current_size, max_size;
}queue_s;


/////////////////////////////DECLARATIONS
void  queue_init(queue_s*, const uint, arena_s*);
void  queue_add(queue_s*, void*);
void* queue_pop(queue_s*);
bool  queue_is_empty(queue_s*);

/////////////////////////////DEFINITIONS
void queue_init(queue_s* _queue, const uint _size, arena_s* _arena)
{
	da_init(&_queue->data_, _size, _arena);
	_queue->current_size = 0;
	_queue->head = 0;
	_queue->max_size = _size;
}

void queue_add(queue_s* _queue, void* _value)
{
	if(_queue->current_size >= _queue->max_size) {
		_queue->max_size *= 2;
		da_resize(&_queue->data_, _queue->max_size);
	}

	da_add_at(&_queue->data_, _queue->current_size, _value);
	++_queue->current_size;
}

void* queue_pop(queue_s* _queue)
{
	if(_queue->head >= _queue->max_size) {
		perror("queue is empty\n");
		assert(0);
	}
	void* ret_val = da_get_at(&_queue->data_, _queue->head);
	++_queue->head;
	return ret_val;
}

bool queue_is_empty(queue_s* _queue)
{
	//printf("test me! (queue_is_empty)\n");
	return (_queue->head == _queue->current_size);
}

#endif//__QUEUE__
