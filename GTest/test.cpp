#include "pch.h"
#include <iostream>
#include "../SCSTest/src/queue.cpp"

TEST(QueueTest, SCSEmailTest) {
	prepare_array();

	Q *q0 = create_queue();
	ASSERT_NE(q0, nullptr);

	enqueue_byte(q0, 0);
	EXPECT_EQ(*(*q0), 0);

	enqueue_byte(q0, 1);
	EXPECT_EQ(*(*q0 + 1), 1);

	Q *q1 = create_queue();
	ASSERT_NE(q1, nullptr);

	enqueue_byte(q1, 3);
	EXPECT_EQ(*(*q1), 3);

	enqueue_byte(q0, 2);
	EXPECT_EQ(*(*q0 + 2), 2);

	enqueue_byte(q1, 4);
	EXPECT_EQ(*(*q1 + 1), 4);
	
	EXPECT_EQ(*(*q0), 0);
	printf("%d", dequeue_byte(q0));
	EXPECT_EQ(*(*q0), 1);
	printf("%d\n", dequeue_byte(q0));
	EXPECT_EQ(*(*q0), 2);

	enqueue_byte(q0, 5);
	EXPECT_EQ(*(*q0 + 1), 5);

	enqueue_byte(q1, 6);
	EXPECT_EQ(*(*q1 + 2), 6);

	EXPECT_EQ(*(*q0), 2);
	printf("%d", dequeue_byte(q0));
	EXPECT_EQ(*(*q0), 5);
	printf("%d\n", dequeue_byte(q0));

	destroy_queue(q0);
	EXPECT_EQ(*q0, nullptr);
	
	EXPECT_EQ(*(*q1), 3);
	printf("%d", dequeue_byte(q1));
	EXPECT_EQ(*(*q1), 4);
	printf("%d", dequeue_byte(q1));
	EXPECT_EQ(*(*q1), 6);
	printf("%d\n", dequeue_byte(q1));

	destroy_queue(q1);
	EXPECT_EQ(*q1, nullptr);
}

TEST(QueueTest, DequeueEmptyTest)
{
	prepare_array();

	Q* q0 = create_queue();

	enqueue_byte(q0, 1);
	dequeue_byte(q0);
	EXPECT_THROW(dequeue_byte(q0), std::invalid_argument);
	destroy_queue(q0);
}

TEST(QueueTest, DeleteNonEmptyQueueTest)
{
	prepare_array();

	Q* q0 = create_queue();
	for (short unsigned i = 0; i < (15 * 80); i++)
	{
		ASSERT_NO_THROW(enqueue_byte(q0, 85));
		ASSERT_EQ(*(*(reinterpret_cast<Q*>(data) + 64) - 1), 85);
	}

	Q* q1 = create_queue();
	enqueue_byte(q1, 42);

	//Ensuring that the queue really moved to the beginning
	Q q0a = *q0;
	destroy_queue(q0);
	ASSERT_EQ(*q0, nullptr);
	EXPECT_EQ(*q0a, 42);

	EXPECT_EQ(dequeue_byte(q1), 42);
	EXPECT_ANY_THROW(dequeue_byte(q1));
	destroy_queue(q1);
}

TEST(QueueTest, StringTest) {
	prepare_array();

	char s0[] = "abcdefg";
	Q* q0 = create_queue();

	for (short int i = 0; i < sizeof(s0); i++)
	{
		enqueue_byte(q0, s0[i]);
	}

	char s1[8];
	for (short int i = 0; i < sizeof(s0); i++)
	{
		s1[i] = dequeue_byte(q0);
	}
	EXPECT_EQ(std::string(s0), std::string(s1));
	destroy_queue(q0);
}

TEST(QueueTest, Average15x80) {
	prepare_array();

	Q* q[64];
	for (short unsigned i = 0; i < 15; i++)
	{
		q[i] = create_queue();
		EXPECT_NE(q[i], nullptr);
		for (short unsigned j = 1; j <= 80; j++)
		{
			EXPECT_NO_THROW(enqueue_byte(q[i], 42));
		}
	}
	EXPECT_NE(q[42], nullptr);
	for (short unsigned i = 0; i < 15; i++)
	{
		EXPECT_NO_THROW(dequeue_byte(q[i]));
		EXPECT_NO_THROW(destroy_queue(q[i]));
	}
}

TEST(QueueTest, The64QueuesTest) {
	prepare_array();

	Q* q[64];
	for (short unsigned i = 0; i < 64; i++)
	{
		q[i] = create_queue();
		for (short unsigned j = 1; j <= 2; j++)
		{
			EXPECT_NO_THROW(enqueue_byte(q[i], i*j));
		}
	}
	for (short unsigned i = 0; i < 64; i++)
	{
		EXPECT_NO_THROW(dequeue_byte(q[i]));
		EXPECT_NO_THROW(destroy_queue(q[i]));
	}
}

TEST(QueueTest, ManyQueuesTest) {
	prepare_array();

	Q* q[64];
	for (short unsigned i = 0; i < 64; i++)
	{
		q[i] = create_queue();
		for (short unsigned j = 1; j <= 8; j++)
		{
			EXPECT_NO_THROW(enqueue_byte(q[i], 42));
		}
	}
	for (short unsigned i = 15; i < 32; i++)
	{
		EXPECT_NO_THROW(dequeue_byte(q[i]));
		EXPECT_NO_THROW(destroy_queue(q[i]));
	}
	for (short unsigned i = 15; i < 32; i++)
	{
		q[i] = create_queue();
		EXPECT_NO_THROW(enqueue_byte(q[i], 42));
	}
	for (short unsigned i = 0; i < 64; i++)
	{
		EXPECT_NO_THROW(dequeue_byte(q[i]));
		EXPECT_NO_THROW(destroy_queue(q[i]));
	}
}

TEST(QueueTest, OverflowTest)
{
	prepare_array();

	Q* q0 = create_queue();
	for (short unsigned i = 0; i < 1788; i++)
	{
		ASSERT_NO_THROW(enqueue_byte(q0, 85));
		ASSERT_EQ(*(*(reinterpret_cast<Q*>(data) + 64) - 1), 85);
	}
	EXPECT_THROW(enqueue_byte(q0, 42), std::out_of_range);
	destroy_queue(q0);
}