# Threading: User-level thread library

## Summary

This library consists of threading api to concurrently run threads during a
process, from thread creation to switching between threads to destroying
threads. This library also includes features such as semaphores and preemption.

## Implementation

The implementation of this program includes 4 aspects:

1. Creating, using, and deleting queues for thread storage and choosing
2. Creating, switching, and destroying threads
3. Using semaphores for synchronization between threads
4. Adding preemption to prevent a thread hogging the processor

### Queue

The queue is made up of nodes, each of which holds a generic type of data.
Nodes are represented using a node data structure, which contains a void pointer
to the data stored in the node and a pointer to the next node. A void pointer is
utilized to make the node generic in what it can store, while a next node
pointer makes the queue single directional and easier to manage.

The data structure for the queue is made up of a pointer to the first node in
the queue, a pointer to the last node in the queue, and an integer for queue
length. The pointer to the first node is used to dequeue the next item, while
the pointer to the last node is used to enqueue new objects. Queue traversal is
done by going to the next node indicated by the nextNode pointer in the current
node. A length variable is maintained for simplicity and speed, so the queue
does not need to be traversed each time the length needs to be calculated. These
pointers are utilized to ensure every operation is O(1), since it does not need
to traverse the queue to get the length or to the last node.

Queue creation is done by using malloc to reserve memory space for the queue, so
that the qqueue can be used outside of its creation function. The firstNode and
lastNode pointers are initialized to NULL and length to 0. This queue is then
returned. Destroying the queue checks if the queue is empty first before
deallocating the memory allotted for the queue. If the queue is not empty or
NULL, a -1 is returned to signify an error.

For enqueueing, the function first checks if the queue and data exist, returning
a -1 if either do not exist. Memory space for the new node is then allocated and
has its dataPointer variable set to the address of the data passed. The queue
then checks if this is the first node, in which case both the first and last
node pointers in the queue are set to this node. Otherwise, the nextNode pointer
is mapped to the new node's address and the last node pointer is updated to the
new node. The length is incremented in either case.

For dequeueing, a check is performed to make sure the queue and data exist and
that the queue is not empty. The data is then removed from the first node, the
first node pointer is moved to the second pointer or NULL if it is the only
node. This node is then deallocated and the length is decremented.

Queue delete first checks if the queue and data are valid and the queue is not
empty. The queue then iterates through each of its elements and compares the
dataPointer to the data provided until a match is found. When a match is found,
the node is freed and the previousNode's pointer is mapped to the node after
the matching node. The length is then decremented.

Queue iterate performs a check to make sure the queue and function are valid,
and that the queue is not empty. The function uses a while loop to run the
function on each node, using the nextNode pointer to traverse the queue.

### Threads

Thread data is managed with a Thread Control Block data structure, which has the
thread's context, a pointer to the thread's stack, and the state of the thread.
The thread library uses 4 global variables: a waiting thread queue; a blocked
thread queue; a context for the thread manager thread; a pointer to the
thread control block of the current thread.

Threading starts when `uthread_run` is ran, where it takes in a preemption flag,
the thread function, and arguments. First the thread queue is created and the
memory for the conext of the manager thread is allocated. The first thread is
created using the thread creation function and the thread manager then enters a
loop, which continues until the queue is empty. The manager dequeues the first
thread in the queue, switches its state to running, and switches the context to
that thread. If the thread manager is returned to via thread exit, the thread's
context is deallocated, the thread's stack is destroyed, and the thread being
deallocated. Once all the threads are completed, the thread manager's memory and
context are deallocated and the threading is complete.

Thread creation is performed by first allocating memory for the new thread, the
context of the new thread, the thread's stack pointer, and the state is set to
READY. The thread's context is initialized so it can be switched to and the
thread is added to the thread queue. When the function finishes, the thread is
at the back of the queue thread and able to be context switched to at any time.

Thread yield is performed by first saving the current thread's context to the
current thread's TCB and switches the current thread's state to READY. The
current thread is then enqueued and the next thread is dequeued. This next
thread's state is set to running and the context is switched to this new thread.
Thread exit is similar to yield, except it does not save the current thread's
context and always switches to the thread manager's context stored in the
loopContext global variable.
