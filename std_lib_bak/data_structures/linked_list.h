#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "../utils.h"
#include "../defines.h"
#include "../allocators/arena.h"


typedef struct node {
	void* data;
	struct node* next;
}node_s;



typedef struct {
	uint size_;
	node_s* head;
	node_s* tail;
	bool uses_arena_;
	arena_s* arena_;
}linked_list_s;

#define CREATE_NODE(_ll) \
	_ll->uses_arena_										\
	? (node_s*)get_raw_memory(_ll->arena_, sizeof(node_s))	\
	: (node_s*)malloc(sizeof(node_s))						\

///depercated - i prefer macro for this...
node_s* create_node_(linked_list_s* _ll)
{
	node_s* node;
	if(_ll->uses_arena_) {
		node = (node_s*)get_raw_memory(_ll->arena_, sizeof(node_s));
	} else {
		node = (node_s*)malloc(sizeof(node_s));
	}
	return node;
}


/////////////////////////////DECLARATIONS
void  ll_init(linked_list_s*, arena_s*);
void  ll_free(linked_list_s*);
void  ll_append(linked_list_s*, void*);
void  ll_insert(linked_list_s*, uint, void*);
void* ll_get_at(linked_list_s*, uint);
void* ll_remove(linked_list_s*, uint);


/////////////////////////////IMPLEMENTATIONS
void ll_init(linked_list_s* _ll, arena_s* _arena)
{
	assert(_ll != NULL);
	_ll->arena_ = _arena;
	_ll->uses_arena_ = false;
	if(_ll->arena_ != NULL) {
		_ll->uses_arena_ = true;
	}

	_ll->head = CREATE_NODE(_ll);
	_ll->tail = _ll->head;
	_ll->size_ = 0;


	_ll->head->next = NULL;
}

void ll_free(linked_list_s* _ll)
{
	assert(_ll != NULL);
	assert(_ll->head != NULL);
	if(_ll->uses_arena_) return;

	node_s* temp_node = _ll->head;
	while(temp_node != NULL) {
		node_s* to_be_cleaned = temp_node;
		temp_node = temp_node->next;
		free(to_be_cleaned);
	};
}

void ll_append(linked_list_s* _ll, void* _value)
{
	assert(_ll != NULL);
	assert(_ll->head != NULL);
	assert(_ll->tail != NULL);

	do {	///do once break capable loop
	if(_ll->size_ == 0) {
		_ll->head->data = _value;
		break;
	}

	node_s* temp_node = CREATE_NODE(_ll);
	temp_node->data = _value;
	temp_node->next = NULL;

	if(__builtin_expect(_ll->size_ == 1, 0)) {
		_ll->tail = temp_node;
		_ll->head->next = _ll->tail;
	} else {
		_ll->tail->next = temp_node;
		_ll->tail = temp_node;
	}
	} while(0);

	++(_ll->size_);
}

void* ll_get_at(linked_list_s* _ll, uint _index)
{
	assert(_ll != NULL);
	assert(_ll->head != NULL);
	assert(_ll->tail != NULL);

	node_s* temp_node = _ll->head;
	while(temp_node != NULL) {
		if(_index == 0) break;
		temp_node = temp_node->next;
		--_index;
	}
	assert(_index == 0);
	return temp_node->data;
}

void ll_insert(linked_list_s* _ll, uint _index, void* _value)
{
	assert(_ll != NULL);
	assert(_ll->head != NULL);
	assert(_ll->tail != NULL);

	node_s* to_insert = CREATE_NODE(_ll);
	assert(to_insert != NULL);

	do {
		if(_index == 0) {
			to_insert->next = _ll->head;
			to_insert->data = _value;
			_ll->head = to_insert;
			break;
		}

		node_s* temp_node = _ll->head;
		do {
			if(_index == 0) break;
			temp_node = temp_node->next;
			--_index;
		} while(temp_node != NULL);

		assert(_index == 0);

		to_insert->data = temp_node->data;
		temp_node->data = _value;
		to_insert->next = temp_node->next;
		temp_node->next = to_insert;
	} while(0);
}


void* ll_remove(linked_list_s* _ll, uint _index)
{
	assert(_ll != NULL);
	assert(_ll->head != NULL);
	assert(_ll->tail != NULL);

	node_s* previous_node = NULL;
	node_s* temp_node = _ll->head;
	void* ret_val = 0;


	do { ///one time break loop

	if(_index == 0) {
		ret_val = _ll->head->data;
		_ll->head = _ll->head->next;
		break;
	}

	while(temp_node != NULL) {
		if(_index == 0) break;
		previous_node = temp_node;
		temp_node = temp_node->next;
		--_index;
	}

	assert(_index == 0);
	previous_node->next = temp_node->next;
	ret_val = temp_node->data;
	if(!_ll->uses_arena_) free(temp_node);

	} while(0);

	return ret_val;
}


#endif//__LINKED_LIST__
