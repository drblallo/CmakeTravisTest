//
// Created by massimo on 29/07/18.
//
#include <gtest/gtest.h>

#include "SafeQueue.hpp"

#define SIZE 5

template class utils::SafeQueue<int>;

class SafeQueueTest: public testing::Test
{
	public:
	utils::SafeQueue<int> queue;
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(SafeQueueTest, constructorShouldCreateEmpty)
{
	EXPECT_TRUE(queue.empty());
}

TEST_F(SafeQueueTest, elementsShouldBeReturnedInOrder)
{
	for (int a = 0; a < SIZE; a++)
		queue.offer(a);

	for (int a = 0; a < SIZE; a++)
		EXPECT_EQ(a, queue.poll());
}

TEST_F(SafeQueueTest, elementShouldBeInFrontInOrder)
{
	for (int a = 0; a < SIZE; a++)
		queue.offer(a);

	for (int a = 0; a < SIZE; a++)
	{
		EXPECT_EQ(a, queue.front());
		queue.poll();
	}
}

TEST_F(SafeQueueTest, initialLenghShouldBeZero) { EXPECT_EQ(0, queue.size()); }

TEST_F(SafeQueueTest, offerShouldIncreaseSize)
{
	for (int a = 0; a < SIZE; a++)
	{
		queue.offer(a);
		EXPECT_EQ(a + 1, queue.size());
	}
}

TEST_F(SafeQueueTest, pollShouldDecreaseSize)
{
	for (int a = 0; a < SIZE; a++)
	{
		queue.offer(a);
	}

	for (int a = 0; a < 5; a++)
	{
		queue.poll();
		EXPECT_EQ(SIZE - 1 - a, queue.size());
	}
}

TEST_F(SafeQueueTest, copyConstructorObjectShouldBeEqual)
{
	for (int a = 0; a < SIZE; a++)
	{
		queue.offer(a);
	}

	utils::SafeQueue<int> newQueue(queue);
	EXPECT_EQ(queue.size(), newQueue.size());
	for (int a = 0; a < SIZE; a++)
	{
		EXPECT_EQ(queue.poll(), newQueue.poll());
	}
}

TEST_F(SafeQueueTest, copyAssignementObjectShouldBeEqual)
{
	for (int a = 0; a < SIZE; a++)
	{
		queue.offer(a);
	}

	utils::SafeQueue<int> newQueue;
	newQueue.offer(0);
	newQueue = queue;
	EXPECT_EQ(queue.size(), newQueue.size());
	for (int a = 0; a < SIZE; a++)
	{
		EXPECT_EQ(queue.poll(), newQueue.poll());
	}
}

TEST_F(SafeQueueTest, moveConstructorObjectShouldBeEqual)
{
	for (int a = 0; a < SIZE; a++)
	{
		queue.offer(a);
	}

	utils::SafeQueue<int> newQueue(std::move(queue));
	EXPECT_EQ(newQueue.size(), SIZE);
	for (int a = 0; a < SIZE; a++)
	{
		EXPECT_EQ(newQueue.poll(), a);
	}
}

TEST_F(SafeQueueTest, heapCreationAndDestruction)
{
	auto q = new utils::SafeQueue<int>();
	delete q;
}
