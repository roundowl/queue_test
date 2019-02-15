/*  queue.cpp
*   Implements the required functions.
*/

/*	I'll use first two bytes of the provided array to hold the "next available" index.
*	I'll use std::vector as it's a dynamic array. One vector will hold 
*/

#include "../headers/queue.h"

void prepare_array()
{
	//reinterpret_cast<char*>(&variable)
	Q** pointer;
	for (int i; i < 64; i++)
	{
		pointer = reinterpret_cast<Q**>(&data + i * sizeof(Q**));
		*pointer = nullptr;
	}
	pointer = reinterpret_cast<Q**>(&data + 64 * sizeof(Q*));
	Q* next_available_address = reinterpret_cast<Q*>(&data + 65 * sizeof(Q**));
	*pointer = next_available_address;
}


/*	The idea is to cut the array of chars into sections:
*	1. 64 pointers to queue beginnings (char* array),
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

/*	What points where:
	unsigned char data[2048];
	typedef unsigned char Q;
	Q queue_data = starting byte of queue, also popping value
	Q* queue = &queue_data = pointer to starting byte of queue
	Q** queue_pointer = &queue = pointer to queue variable for listing reasons

	Q* next_available = pointer to the next free byte not belonging to any queue
*/

/*	What is where:
	Q** queue_pointer[0..63] = from &data to &data + 63*sizeof(Q*)
	Q*	next_available = &data + 64*sizeof(Q*)
	Q actual data[] = &data + 65*sizeof(Q*) and onwards to &data[2048] inclusive
*/

/*	When the queue gets created, the function looks through the
*	first section, searching for the next std:nullptr.
*	It then writes the next_available address to the queue pointer,
*	which would be the location of the first char of new queue.
*	The pointer in the first section gets the location of the queue
*	pointer for easy reference (unsigned char **).
*/
Q* create_queue()
{
	Q* next_available_address = reinterpret_cast<Q*>(&data + 64 * sizeof(Q**));
	Q* queue = next_available_address;

	Q** pointer = reinterpret_cast<Q**>(&data);
	for (int i = 1; pointer != nullptr; i++)
	{
		Q** pointer = reinterpret_cast<Q**>(&data + i * sizeof(Q**));
	}
	pointer = &queue;

	return queue;
}

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

void enqueue_byte(Q* q, unsigned char b)
{

}