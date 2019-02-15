#pragma once

typedef unsigned char* Q;

void prepare_array();

Q *create_queue();

void destroy_queue(Q *q);

void enqueue_byte(Q *q, unsigned char b);

unsigned char dequeue_byte(Q *q);

void on_out_of_memory();

unsigned char data[2048];

/*	The idea is to cut the array of chars into sections:
*	1. 64 pointers to queues (char** array),
*	2. A pointer to the next free char in the array (char*),
*	3. Actual data values belonging to different queues (char array).
*/
/*	Queues are sequential, data belonging to first queue is before
*	the data belonging to second queue. Thus, by jumping right in
*	first section, we can be sure we'll be moving right in the data.
*/
/*	When the queue gets created, the function looks through the
*	first section, searching for the next std:nullptr.
*	It then writes the next_available address to the pointer,
*	which would be the location of the first char of new queue.
*	The queue pointer itself (Q *q) gets the location of the address
*	in the first section in the array (char **).
*/
/*	When the new byte is pushed to a queue, we get the queue pointer
*	from the queue variable, then we jump to the next queue pointer,
*	and shift everything from that address right, starting from
*	next_available char, ending at char pointed to by next queue.
*	This is also where we check if we're out of memory.
*	If next_available address is equal to data[2048] address,
*	then we can't shift without landing outside.
*	After shifting everything right, we update the first section
*	by adding 1 to every address right of ours,
*	and insert our new pushed byte.
*/
/*	When the byte is popped from the queue, we get the queue pointer
*	from the queue variable, then we pop the byte at which it is
*	pointing, then we move everything left, starting at the same
*	address, ending with next_available char. Also updating pointers.
*	If the next queue pointer is equal to this queue pointer,
*	then the queue is empty, and we shouldn't pop anything.
*/
/*	When the queue gets removed completely, we get it's pointer,
*	get the next pointer, then subtract the first from second to find
*	the size, and shift everything into the place of deleted queue.
*	We then update first section, and the last queue pointer becomes
*	std::nullptr.
*/
