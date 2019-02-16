#include "pch.h"
#include "../SCSTest/src/queue.cpp"

TEST(TestCaseName, TestName) {
	prepare_array();
	EXPECT_EQ(*(reinterpret_cast<Q*>(data) + 64), reinterpret_cast<Q>(data) + 65);

	Q *q0 = create_queue();
	ASSERT_NE(q0, nullptr);
	enqueue_byte(q0, 0);
	enqueue_byte(q0, 1);
	EXPECT_EQ(data[65 * sizeof(Q)], 1);
	Q *q1 = create_queue();
	enqueue_byte(q1, 3);
	enqueue_byte(q0, 2);
	enqueue_byte(q1, 4);
	printf("%d", dequeue_byte(q0));
	printf("%d\n", dequeue_byte(q0));
	enqueue_byte(q0, 5);
	enqueue_byte(q1, 6);
	printf("%d", dequeue_byte(q0));
	printf("%d\n", dequeue_byte(q0));
	destroy_queue(q0);
	printf("%d", dequeue_byte(q1));
	printf("%d", dequeue_byte(q1));
	printf("%d\n", dequeue_byte(q1));
	destroy_queue(q1);


}