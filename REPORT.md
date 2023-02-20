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

### Semaphores 

Semaphore data is managed by the Semaphore data type, which consists of the 
following two attributes:
1. `curr_count`: an integer representing current count value
2. `waiting_queue`: a queue holding all of the threads currently 
waiting on the semaphore  

The logic for creating and deleting semaphores is simple. Create just 
allocates a new instance of the semaphore data type, with an initial count 
and empty queue. Delete first deallocates the waiting queue 
(using the queue_destroy function) and then deallocates the semaphore 
instance using `free`. 

Before implementing the `sem_up()`, and `sem_down()` functions, functionality 
needed to be added to the `uthread api` to allow for blocking and unblocking 
threads. This is done through the `uthread_block()` and `uthread_unblock()` 
functions, as well as the introduction of a queue for holding blocked threads (
named `blocked_queue`). 

To block a thread, the current threads state is set to be BLOCKED, the current 
thread is added to the blocked queue, and then the current threads context 
switches to the next "ready" thread. To unblock a given thread, the given 
thread is removed from the blocked queue and its state is set to READY. It is 
then added to the thread queue holding threads that are ready to run on the 
CPU. 

When a thread calls `sem_down()`, the semaphore will first disable preemption 
in order to prevent getting interrupted. Then it will check if the current 
count is above 0. If so, it will decrement its count, reenable preemption, and 
let the thread continue on unblocked. If the current count is 0, the semaphore 
will add the thread to its waiting queue and block it, making sure to reenable 
preemption before switching to a new thread. When a thread calls `sem_up()`, 
the semaphore will first disable preemption. It will then check if there are 
any threads in the waiting queue and if so, it will unblock the thread at the 
front of the queue and remove it from the queue. If the waiting queue is empty, 
the semaphore will just increase its count. 

This design of down and up ensures correct functionality in all possible cases 
while also prevented starvation. In the corner case, when thread A calls down 
on a semaphore with count 0, it will be added to the semaphores waiting queue 
and get blocked. Then, when thread B calls up on that semaphore, thread A will 
be unblocked, but the semaphores count will remain 0. Thus, when thread C then 
calls `sem_down()`, rather than getting the resource, it will itself be 
blocked, as the semaphore has already "reserved" the free spot opened by thread 
B for thread A. Thus, when thread A runs, it will be able to move on without 
getting blocked. Thus, no thread ever starves because even if it gets blocked 
on the semaphore, it will be prioritized over all other future threads that 
also try to grab the resource. No future thread will get the semaphore resource 
before the currently blocked thread. 

### Preemption

If preemption is requested, then three of the following needs to be set up:
1. A virtual timer that triggers 100 times every second
2. A signal set containing the signal to block when preemption is disabled
3. A handler for when the virtual timer expires 

For the first step, a new timer is created that triggers once every 10,000 
microseconds. Because there are 1,000,000 microseconds in one second, this 
ensures that the timer goes off 100 times every second. In addition, the 
previous virtual timer configuration is saved. For the second step, the only 
signal that would need to be blocked when preemption is disabled is the 
'SIGVTALRM' signal. This is the signal that the virtual alarm sends when it 
goes off. For the third step, a handler for the 'SIGVTALRM' is set up so that 
the uthread_yield() function is called. Thus, when the timer goes off in the 
current thread, it will have to yield to the next available thread. 

When preemption is stopped, the old signal handler for 'SIGVTALRM' is 
reestablished, as well as the previous timer configuration. 

For disabling preemption, the SIGVTALRM signal is set to be blocked using the 
'sigprocmask' function provided by the signal library. When preemption is 
enabled again, the timer is reset and then the SIGVTALRM signal is unblocked. 
It is very important to reset the timer, or else a newly started thread may 
quickly get preempted unfairly. 

Within the threading library, the current thread, threading queue, and blocked 
queue are shared among threads. Thus, in the following areas that access one or 
more of these, preemption is disabled: 
1. `uthread_yield()`.
2. `uthread_create()`, when the new thread is added to the thread queue.
3. `uthread_run()`, when switching to next available thread and delete a thread.
5. `uthread_block()` and `uthread_unblock()`.

To test preemption, two threads are set up. The first thread that is triggered 
runs an infinite loop. The second thread will do a simple print and then cause 
the program to exit. If preemption is not enabled or if the preemption logic 
does not work, then the first thread would just be able to run forever, and the 
program would never terminate. But if preemption occurs, then the first thread 
will eventually get interrupted, causing the second thread to run and stop the 
program. 

### Sources
* https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Signal-Actions
* https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Setting-an-Alarm 
* https://stackoverflow.com/questions/4019121/run-an-infinite-loop-for-a-while-in-c 