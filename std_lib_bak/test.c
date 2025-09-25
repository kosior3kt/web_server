#include <stdio.h>
#include <string.h>
#include <stdlib.h>

////////////////////////// OWN INCLUDES
#include "utils.h"
#include "data_structures/dynamic_array.h"
#include "data_structures/linked_list.h"
#include "data_structures/stack.h"
#include "data_structures/queue.h"
#include "data_structures/heap.h"


////////////////////////// GLOBALS
arena_s global_arena;
char*	test_results;
uint	total_tests_number;
uint	failed_tests_numer;

////////////////////////// MACROS
#define REPORT_FAILED_TEST(x) \
	test_results = arena_concat_string(&global_arena,	\
										test_results,	\
										x);				\
	++failed_tests_numer;								\

////////////////////////// DECLARATIONS
///dynamic array
void test_dynamic_array();
void da_test_1();			//adding and reading data
void da_test_2();			//dynamic resizing
void da_test_3();			//empty array edge cases

///linked list
void test_linked_list();
void ll_test_1();

///stack
void test_stack();
void stack_test_1();

///queue
void test_queue();
void queue_test_1();

///heap
void test_heap();
void heap_test_1();


int main()
{
	arena_init(&global_arena);

	test_results = (char*)get_raw_memory(&global_arena, sizeof(char) * strlen(" "));

	test_dynamic_array();
	test_linked_list();
	test_stack();
	test_queue();

	printf("test results:\n\tfailed %d out of %d\n", failed_tests_numer, total_tests_number);

	if(test_results != NULL) {
		printf("\tfailed tests:\n%s", test_results);
	}
	return 0;
}

////////////////////////// DEFINITIONS

void test_dynamic_array()
{
	da_test_1();
	total_tests_number += 2;
	da_test_2();
	total_tests_number += 2;
	da_test_3();
	total_tests_number += 2;
}

void test_linked_list()
{
	ll_test_1();
	total_tests_number += 2;
	/*ll_test_2();*/
	/*ll_test_3();*/
}

void test_stack()
{
	stack_test_1();
	total_tests_number += 2;
}

void test_queue()
{
	queue_test_1();
	total_tests_number += 2;
}

void test_heap()
{
	heap_test_1();
}


void da_test_1()
{
	dynamic_array_s da_arena;
	da_init(&da_arena, 10, &global_arena);
	//trivial test - create dynamic array with enough size to store everything. then test adding and reading of data

	for(int i = 0; i < 5; ++i) {
		da_append(&da_arena, (void*)i);
	}

	for(int i = 0; i < 5; ++i) {
		int item = (int)da_get_at(&da_arena, i);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" dynamic array test 1 with arena failed!\n");
			break;
		}
	}

	dynamic_array_s da_no_arena;
	da_init(&da_no_arena, 10, NULL);
	//trivial test - create dynamic array with enough size to store everything. then test adding and reading of data

	for(int i = 0; i < 5; ++i) {
		da_append(&da_no_arena, (void*)i);
	}

	for(int i = 0; i < 5; ++i) {
		int item = (int)da_get_at(&da_no_arena, i);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" dynamic array test 1 without arena failed!\n");
			break;
		}
	}
}

void da_test_2()
{
	///less trivial test - create dynamic array and actually resize something in there

	///with arena
	dynamic_array_s da_arena;
	da_init(&da_arena, 10, &global_arena);

	for(int i = 0; i < 5; ++i) {
		da_append(&da_arena, (void*)i);
	}

	for(int i = 0; i < 5; ++i) {
		int item = (int)da_get_at(&da_arena, i);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" dynamic array test 2 with arena failed!\n");
			break;
		}
	}

	/// wihtout my arena
	dynamic_array_s da_no_arena;
	da_init(&da_arena, 1, NULL);

	for(int i = 0; i < 5; ++i) {
		da_append(&da_no_arena, (void*)i);
	}

	for(int i = 0; i < 5; ++i) {
		int item = (int)da_get_at(&da_no_arena, i);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" dynamic array test 2 without arena failed!\n");
			break;
		}
	}
}

void da_test_3()
{

	///with arena
	dynamic_array_s da_arena;
	da_init(&da_arena, 10, &global_arena);

	printf("implement rest of tests later for da\n");
}


void ll_test_1()
{
	///trivial test, checking saving and restoring data
	//with arena
	linked_list_s ll_arena;

	ll_init(&ll_arena, &global_arena);

	for(int i = 0; i < 5; ++i){
		ll_append(&ll_arena, (void*)i);
	}

	for(int i = 0; i < 5; ++i){
		int item = (int)ll_get_at(&ll_arena, i);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" linked list test 1 with arena failed!\n");
			break;
		}
	}

	///trivial test, checking saving and restoring data
	//without arena
	linked_list_s ll_no_arena;

	ll_init(&ll_no_arena, &global_arena);

	for(int i = 0; i < 5; ++i){
		ll_append(&ll_no_arena, (void*)i);
	}

	for(int i = 0; i < 5; ++i){
		int item = (int)ll_get_at(&ll_no_arena, i);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" linked list test 1 without arena failed!\n");
			break;
		}
	}

	ll_free(&ll_arena);
	ll_free(&ll_no_arena);
}

void stack_test_1()
{
	///trivial test, checking saving and restoring data
	//with arena
	stack_s stack_arena;

	stack_init(&stack_arena, 10, &global_arena);

	for(int i = 0; i < 5; ++i){
		stack_add(&stack_arena, (void*)i);
	}

	int i = 4;
	while(!stack_is_empty(&stack_arena)){
		int item = (int)stack_pop(&stack_arena);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" stack test 1 with arena failed!\n");
			break;
		}
		--i;
	}


	//without arena
	stack_s stack_no_arena;
	stack_init(&stack_no_arena, 10, NULL);

	for(int i = 0; i < 5; ++i){
		stack_add(&stack_no_arena, (void*)i);
	}

	i = 4;
	while(!stack_is_empty(&stack_no_arena)){
		int item = (int)stack_pop(&stack_no_arena);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" stack test 1 without arena failed!\n");
			break;
		}
		--i;
	}
}

void queue_test_1()
{
	///trivial test, checking saving and restoring data
	//with arena
	queue_s queue_arena;

	queue_init(&queue_arena, 10, &global_arena);

	for(int i = 0; i < 5; ++i){
		queue_add(&queue_arena, (void*)i);
	}

	int i = 0;
	while(!queue_is_empty(&queue_arena)){
		int item = (int)queue_pop(&queue_arena);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" queue test 1 with arena failed!\n");
			break;
		}
		++i;
	}


	//without arena
	queue_s queue_no_arena;
	queue_init(&queue_no_arena, 10, NULL);

	for(int i = 0; i < 5; ++i){
		queue_add(&queue_no_arena, (void*)i);
	}

	i = 0;
	while(!queue_is_empty(&queue_no_arena)){
		int item = (int)queue_pop(&queue_no_arena);
		if(item != i) {
			//if reading failed let user know that by appening error list
			REPORT_FAILED_TEST(TERMINAL_RED"[ERROR]:"TERMINAL_RESET" queue test 1 without arena failed!\n");
			break;
		}
		++i;
	}
}

static bool do_nice_sort(void* _a, void* _b)
{
	return ((int)_a > (int)_b);
}

void heap_test_1()
{
	min_heap_s heap_arena;
	//min_heap_init(min_heap_s* _heap, const uint _size, arena_s* _arena, bool(*_predicate)(void*, void*))
	min_heap_init(&heap_arena, 10, &global_arena, &do_nice_sort);

	for(int i = 0; i < 5; ++i) {
		min_heap_add(&heap_arena, i);
	}

	printf("this is how the stack looks like now:\n");
	min_heap_print(&heap_arena);

	for(int i = 4; i < 0; --i) {
		int elem = min_heap_pop(&heap_arena); //????
	}

}
