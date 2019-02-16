/*  queue.cpp
*   Implements the required functions.
*/

#include "../headers/queue.h"
#include <cstring>
#include <iostream>
unsigned char data[2048];

void prepare_array()
{
	Q* pointer;
	for (int i = 0; i < 64; i++)
	{
		pointer = &reinterpret_cast<Q*>(data)[i];
		*pointer = nullptr;
	}
	pointer = &reinterpret_cast<Q*>(data)[64];
	Q next_available_address = reinterpret_cast<Q>(data) + 65 * sizeof(Q);
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
*	first section, searching for the next nullptr.
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
*	nullptr.
*	---AMENDED---
*	As I can't think of a way to update queue variables, and this is
*	inefficient to update amount_of_queues*2 pointers, I'll leave a nullptr
*	hole where the queue gets deleted. Next created queue will find this hole
*	and will live there. It's data will be pushed in-between prev and next queues,
*	even though it was created later.
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
	Q actual data[] = &data + 65*sizeof(Q*) and onwards to &data[2047] inclusive
*/

/*	When the queue gets created, the function looks through the
*	first section, searching for the next nullptr.
*	It then writes the next_available address to the queue pointer,
*	which would be the location of the first char of new queue.
*	The pointer in the first section gets the location of the queue
*	pointer for easy reference (unsigned char **).
*/
Q* create_queue()
{
	Q* next_available_address_pointer = &reinterpret_cast<Q*>(data)[64];
	//*next_available_address_pointer = reinterpret_cast<Q>(data) + 65*sizeof(Q);
	Q* queue = nullptr;

	Q* pointer = reinterpret_cast<Q*>(data);
	for (int i = 1; *pointer != nullptr; i++)
	{
		pointer = &reinterpret_cast<Q*>(data)[i];
	}

	queue = pointer;
	*pointer = *next_available_address_pointer;

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
	if (q == nullptr)
	{
		on_illegal_operation();
		return;
	}
	Q* next_available_address_pointer = &reinterpret_cast<Q*>(data)[64];
	Q next_available_address = *next_available_address_pointer;
	Q queue_pointer = *q;
	Q next_queue_pointer = *(q + 1);
	if (next_queue_pointer == nullptr) {
		next_queue_pointer = next_available_address;
	}

	if (next_available_address == &data[2048])
	{
		on_out_of_memory();
		return;
	}
	else {
		memmove(next_queue_pointer + 1, next_queue_pointer, next_available_address - next_queue_pointer);
		*next_queue_pointer = b;
		for (int i = 1; (q + i) < next_available_address_pointer; i++)
		{
			if (*(q + i) != nullptr) { *(q + i) += 1; }
		}
		/*for (Q queue = next_queue_pointer; queue < next_available_address; queue = queue + 1)
		{
			if (queue != nullptr) { queue = queue + 1; };
		}*/
		next_available_address++;
		*next_available_address_pointer = next_available_address;
	}
}

/*	When the byte is popped from the queue, we get the queue pointer
*	from the queue variable, then we pop the byte at which it is
*	pointing, then we move everything left, starting at the same
*	address, ending with next_available char. Also updating pointers.
*	If the next queue pointer is equal to this queue pointer,
*	then the queue is empty, and we shouldn't pop anything.
*/

unsigned char dequeue_byte(Q* q)
{
	if (q == nullptr)
	{
		on_illegal_operation();
		return 0;
	}
	Q* next_available_address_pointer = &reinterpret_cast<Q*>(data)[64];
	Q next_available_address = *next_available_address_pointer;
	Q queue_pointer = *q;
	Q next_queue_pointer = *(q + 1);
	if (next_queue_pointer == nullptr) {
		next_queue_pointer = next_available_address;
	}

	if (next_queue_pointer == queue_pointer)
	{
		on_illegal_operation();
		return 0;
	}
	else {
		unsigned char dequeued_byte = *queue_pointer;
		memmove(queue_pointer, queue_pointer + 1, next_available_address - queue_pointer + 1);

		for (int i = 1; (q + i) < next_available_address_pointer; i++)
		{
			if (*(q + i) != nullptr) { *(q + i) -= 1; }
		}
		next_available_address--;
		*next_available_address_pointer = next_available_address;

		return dequeued_byte;
	}
}

/*	When the queue gets removed completely, we get it's pointer,
*	get the next pointer, then subtract the first from second to find
*	the size, and shift everything into the place of deleted queue.
*	We then update first section, and the last queue pointer becomes
*	nullptr.
*	---AMENDED---
*	As I can't think of a way to update queue variables, and this is
*	inefficient to update amount_of_queues*2 pointers, I'll leave a nullptr
*	hole where the queue gets deleted. Next created queue will find this hole
*	and will live there. It's data will be pushed in-between prev and next queues,
*	even though it was created later.
*	---FIXME---
*	This behaviour will create an error if two holes are sequential.
*	Current solution will think it's the end of pointers.
*	I will move nullptr check from condition to statement.
*/

void destroy_queue(Q *q)
{
	if (q == nullptr)
	{
		on_illegal_operation();
		return;
	}
	Q* next_available_address_pointer = &reinterpret_cast<Q*>(data)[64];
	Q next_available_address = *next_available_address_pointer;
	Q queue_pointer = *q;
	Q next_queue_pointer = *(q + 1);
	if (next_queue_pointer == nullptr) {
		next_queue_pointer = next_available_address;
	}
	int size_of_deleted_queue = next_queue_pointer - queue_pointer;

	memmove(queue_pointer, next_queue_pointer, next_available_address - next_queue_pointer);

	for (int i = 1; (q + i) < next_available_address_pointer; i++)
	{
		if (*(q + i) != nullptr) { *(q + i) -= size_of_deleted_queue; }
	}
	next_available_address -= size_of_deleted_queue;
	*next_available_address_pointer = next_available_address;

	*q = nullptr;
}

void on_out_of_memory()
{
	std::cout << "Ran out of memory.\n";
}

void on_illegal_operation()
{
	std::cout << "Did something illegal. No such queue or it's empty.\n";
}