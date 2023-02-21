#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

#define ERR -1
#define OK 0

struct node{
 	void* dataPointer;
 	struct node* nextNode;
};

struct queue {
 	struct node* firstNode;
 	struct node* lastNode;
 	int length;
};

queue_t queue_create(void)
{
 	struct queue* q = malloc(sizeof(struct queue));
 	q->firstNode = NULL;
 	q->lastNode = NULL;
 	q->length = 0;
 	return q;
}

int queue_destroy(queue_t queue)
{
 	if(queue == NULL || queue->length != 0){
 		return ERR;
 	}
 	free(queue);
 	return OK;
}

int queue_enqueue(queue_t queue, void *data)
{
 	if(queue == NULL || data==NULL){
 		return ERR;
 	}

 	struct node* node = malloc(sizeof(struct node));
 	if(node == NULL){
 		return ERR;
 	}

 	node->dataPointer = data;
 	node->nextNode = NULL;

 	if(queue->firstNode == NULL){
 		queue->firstNode = node;
		queue->lastNode = node;
 	}
 	else{
 		queue->lastNode->nextNode = node;
 		queue->lastNode = node;
 	}

 	queue->length++;
 	return OK;
}

int queue_dequeue(queue_t queue, void **data)
{
 	if(queue == NULL || data == NULL || queue->length == 0){
 		return ERR;
 	}

 	struct node* oldestNode = queue->firstNode;
 	struct node* secondNode = oldestNode->nextNode;
 	*data = queue->firstNode->dataPointer;
 	queue->firstNode = secondNode;
 	free(oldestNode);
 	queue->length--;

 	return OK;
}

int queue_delete(queue_t queue, void *data)
{
 	if(queue == NULL || data == NULL){
 		return ERR;
 	}
 	if(queue->length == 0){
 		return ERR;
 	}

 	struct node* currentNode = queue->firstNode;
 	struct node* prevNode;
 	if(currentNode->dataPointer == data){
 		queue->firstNode = currentNode->nextNode;
 		free(currentNode);
 		queue->length--;
 		return OK;
 	}

 	while(currentNode->nextNode != NULL){
 		prevNode = currentNode;
 		currentNode = currentNode->nextNode;
 		
		if(currentNode->dataPointer == data){
 			
			if(currentNode == queue->lastNode){
 				queue->lastNode = prevNode;
 				prevNode->nextNode = NULL;
 				free(currentNode);
 			}
 			else{	
 				prevNode->nextNode = currentNode->nextNode;
 				free(currentNode);
 			}
 			
			queue->length--;
 			return OK;
 		}
 	}

 	return ERR;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
 	if(queue == NULL || func == NULL){
 		return ERR;
 	}
 	if(queue->length == 0){
 		return OK;
 	}

 	struct node* currentNode = queue->firstNode;
 	func(queue, currentNode->dataPointer);
 	while(currentNode->nextNode != NULL){
 		currentNode = currentNode->nextNode;
 		func(queue, currentNode->dataPointer);
 	}

 	return OK;
}

int queue_length(queue_t queue)
{
 	return queue->length;
}

