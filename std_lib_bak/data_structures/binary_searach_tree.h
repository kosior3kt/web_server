#ifndef __BINARY_SEARCH_TREE__
#define __BINARY_SEARCH_TREE__

#include <stdbool.h>
#include <string.h>
#include "../utils.h"
#include "../defines.h"
#include "../allocators/arena.h"

typedef struct node {
	struct node* left;
	struct node* right;
	int data;
}node_s_;

typedef struct {
	node_s_* root;
	bool uses_arena_;
	arena_s* arena_;
	uint size;
}bst_s;


/////////////////////////////DECLARATIONS
void bst_init(bst_s*, arena_s*);
void bst_free(bst_s*);
void bst_add(bst_s*, const int);
void bst_remove(bst_s*, const int);

/////////////////////////////IMPLEMENTATIONS
void bst_init(bst_s* _tree, arena_s* _arena)
{
	assert(_tree != NULL);

	_tree->size = 0;
	_tree->arena_ = _arena;
	_tree->uses_arena_ = false;
	if(_tree->arena_ != NULL) {
		_tree->uses_arena_ = true;
	}

	_tree->root = CREATE_NODE(_tree);
	_tree->root->data = 0;
}

void bst_free(bst_s* _tree)
{
	assert(_tree != NULL);
	assert(_tree->root != NULL);
	assert(_tree->size > 0);

	if(_tree->uses_arena_) return;
	error_out("bst free");
}

void bst_node_add_(node_s_* _node, const int _value)
{
	if(_node == NULL) {
		_node = CREATE_NODE();
	}
}

void bst_add(bst_s* _tree, const int _value)
{
	assert(_tree != NULL);
	assert(_tree->root != NULL);

	if(_tree->size == 0) {
		_root->data = _value;
	} else {
		bst_node_add_(_tree->root, _value);
	}
	++(_tree->size);
}

void bst_remove(bst_s*, const int);


#endif//__BINARY_SEARCH_TREE__
