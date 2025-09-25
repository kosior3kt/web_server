#include "utils.h"
#include "data_structures/dynamic_array.h"
#include "data_structures/linked_list.h"
#include "data_structures/stack.h"
#include "data_structures/queue.h"
#include <stdio.h>

void test_linked_list(arena_s* arena)
{
	linked_list_s ll, ll2;
	linked_list_s ll_list[] = {ll, ll2};

	for(int ll_index = 0; ll_index < sizeof(ll_list) / sizeof(ll_list[0]); ++ll_index){
		if(ll_index == 0)
			ll_init(&ll_list[ll_index], arena);
		else if(ll_index == 1)
			ll_init(&ll_list[ll_index], NULL);

		for(int i = 0; i < 10; ++i){
			ll_append(&ll_list[ll_index], i);
		}

		printf("==== %d ========================\n", ll_index);

		ll_insert(&ll_list[ll_index], 0, 69);
		ll_remove(&ll_list[ll_index], 2);
		printf("results:\n");
		for(int i = 0; i < 10; ++i){
			int res = ll_get(&ll_list[ll_index], i);
			printf("%d, ", res);
		}
		puts("");
	}
	printf("\n\n");
}


void test_dynamic_array(arena_s* arena)
{
	dynamic_array_s array, array2;
	dynamic_array_s array_list[] = {array, array2};

	for(int array_index = 0; array_index < sizeof(array_list) / sizeof(array_list[0]); ++array_index) {
		if(array_index == 0)
			da_init(&array_list[array_index], 1, arena);
		else if(array_index == 1)
			da_init(&array_list[array_index], 1, NULL);
		printf("==== %d =======================\n", array_index);
		printf("array current size: %d\n", array_list[array_index].current_size);
		printf("array max size: %d\n", array_list[array_index].max_size);

		int i;
		for(i = 0; i < 16; ++i) {
			da_append(&array_list[array_index], i);
		}

		for(i = 0; i < 16; ++i) {
			int temp = da_get_at(&array_list[array_index], i);
			printf("%d, ", temp);
		}
		printf("\n");

		printf("array current size: %d\n", array_list[array_index].current_size);
		printf("array max size: %d\n", array_list[array_index].max_size);
	}
	printf("\n\n");
}

void test_stack(arena_s* arena)
{
	stack_s stack, stack2;
	stack_s stack_list[] = {stack, stack2};

	for(int array_index = 0; array_index < sizeof(stack_list) / sizeof(stack_list[0]); ++array_index) {
		if(array_index == 0)
			stack_init(&stack_list[array_index], 1, arena);
		else if(array_index == 1)
			stack_init(&stack_list[array_index], 1, NULL);

		printf("==== %d =======================\n", array_index);
		printf("current stack size: %d\n", stack_list[array_index].current_size);
		printf("stack max size: %d\n", stack_list[array_index].max_size);

		uint i;
		for(i = 0; i < 16; ++i) {
			stack_add(&stack_list[array_index], i);
		}

		for(i = 0; i < 16; ++i) {
			int temp = stack_pop(&stack_list[array_index]);
			printf("%d, ", temp);
		}
		printf("\n");

		printf("current stack size: %d\n", stack_list[array_index].current_size);
		printf("stack max size: %d\n", stack_list[array_index].max_size);
	}
	printf("\n\n");
}


void test_queue(arena_s* arena)
{
	queue_s queue, queue2;
	queue_s queue_list[] = {queue, queue2};

	for(int array_index = 0; array_index < sizeof(queue_list) / sizeof(queue_list[0]); ++array_index) {
		if(array_index == 0)
			queue_init(&queue_list[array_index], 1, arena);
		else if(array_index == 1)
			queue_init(&queue_list[array_index], 1, NULL);

		printf("==== %d =======================\n", array_index);
		printf("current queue size: %d\n", queue_list[array_index].current_size);
		printf("current head: %d\n", queue_list[array_index].head);
		printf("queue max size: %d\n", queue_list[array_index].max_size);

		uint i;
		for(i = 0; i < 16; ++i) {
			queue_add(&queue_list[array_index], i);
		}

		for(i = 0; i < 16; ++i) {
			int temp = queue_pop(&queue_list[array_index]);
			printf("%d, ", temp);
		}
		printf("\n");

		printf("current queue size: %d\n", queue_list[array_index].current_size);
		printf("current head: %d\n", queue_list[array_index].head);
		printf("queue max size: %d\n", queue_list[array_index].max_size);
	}
	printf("\n\n");
}


int main()
{
	//sanity check
	sanity_check();

	//make arena stuff
	arena_s arena;
	arena_init(&arena);

	test_dynamic_array(&arena);
	test_linked_list(&arena);
	test_stack(&arena);
	test_queue(&arena);

	arena_free(&arena);
	return 0;
}
