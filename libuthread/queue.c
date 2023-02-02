#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node{
	void* dataPointer;
	struct node* nextNode;
}

struct queue {
	/* TODO Phase 1 */
	struct node* firstNode;
	struct node* lastNode;
	int length;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	struct queue* q = malloc(sizeof(struct queue));
	q->firstNode=NULL;
	q->lastNode=NULL;
	q->length=0;
	return q;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue==NULL || queue->length!=0){
		return -1;
	}
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(queue==NULL || data==NULL){
		return -1;
	}
	struct node* node = malloc(sizeof(struct node));
	if(node==NULL){
		return -1;
	}
	node->dataPointer=data;
	node->nextNode=NULL;
	if(queue->firstNode==NULL){
		queue->firstNode=node;
	}
	else{
		queue->lastNode->nextNode=node;
		queue->lastNode=node;
	}
	queue->length++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if(queue==NULL || data==NULL || queue->length==0){
		return -1;
	}
	struct node* oldestNode=queue->firstNode;
	struct node* secondNode = oldestNode->nextNode;
	*data=queue->firstNode->dataPointer;
	queue->firstNode=secondNode;
	free(oldestNode);
	queue->length--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(queue==NULL || data==NULL){
		return -1;
	}
	if(queue->length==0){
		return -1;
	}
	struct node* currentNode = queue->firstNode;
	struct node* prevNode;
	if(currentNode->dataPointer==data){
		queue->firstNode=currentNode->nextNode;
		free(currentNode);
		queue->length--;
		return 0;
	}
	while(currentNode->nextNode!=NULL){
		prevNode=currentNode;
		currentNode=currentNode->nextNode;
		if(currentNode==data){
			if(currentNode==queue->lastNode){
				queue->lastNode=prevNode;
				prevNode->nextNode=NULL;
				free(currentNode);
			}
			else{	
				prevNode->nextNode=currentNode->nextNode;
				free(currentNode);
			}
			queue->length--;
			return 0;
		}
	}
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	if(queue==NULL || func==NULL){
		return -1;
	}
	if(queue->length==0){
		return 0;
	}
	struct node* currentNode= queue->firstNode;
	func(currentNode->dataPointer);
	while(currentNode->nextNode!=NULL){
		currentNode=currentNode->nextNode;
		func(currentNode->dataPointer);
	}
	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	return queue.length;
}

