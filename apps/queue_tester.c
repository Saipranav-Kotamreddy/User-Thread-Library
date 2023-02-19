#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <queue.h>

typedef struct queue* queue_t;

typedef void (*queue_func_t)(queue_t queue, void *data);

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

static void iterator_inc(queue_t q, void* data){
	int* a = (int*)data;

	if(*a==2){
		*a=queue_length(q)-3;
	}
	else{
		*a += 1;
	}
}

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue & destroy simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	TEST_ASSERT(queue_length(q)==1);
	TEST_ASSERT(queue_destroy(q)==-1);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
	TEST_ASSERT(queue_length(q)==0);
	TEST_ASSERT(queue_destroy(q)==0);
}

void test_queue_delete(void){
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int* destPtr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete ***\n");

	q=queue_create();
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	TEST_ASSERT(queue_length(q)==3);
	TEST_ASSERT(queue_delete(q, &data2)==0);
	TEST_ASSERT(queue_length(q)==2);
	queue_dequeue(q, (void**)&destPtr);
	TEST_ASSERT(destPtr==&data1);
	queue_dequeue(q, (void**)&destPtr);
	TEST_ASSERT(destPtr==&data3);
	queue_destroy(q);
}

void test_queue_iterate(void){
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int* destPtr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_iterate ***\n");

	q=queue_create();
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	TEST_ASSERT(queue_length(q)==3);
	queue_iterate(q, iterator_inc);
	queue_dequeue(q, (void**)&destPtr);
	TEST_ASSERT(*destPtr==2);
	queue_dequeue(q, (void**)&destPtr);
	TEST_ASSERT(*destPtr==0);
	queue_dequeue(q, (void**)&destPtr);
	TEST_ASSERT(*destPtr==4);
	queue_destroy(q);
}
int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_delete();
	test_queue_iterate();
	return 0;
}