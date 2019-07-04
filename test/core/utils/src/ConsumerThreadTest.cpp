//
// Created by massimo on 30/07/18.
//

#include <ConsumerThread.hpp>
#include <gtest/gtest.h>
#include <thread>

#include "TestUtils.hpp"

using namespace utils;

class ConsumerThreadTest: public testing::Test
{
	public:
	ConsumerThreadTest(): markCalled(false) {}
	ConsumerThread thread;
	std::atomic<bool> markCalled;

	virtual void SetUp() {}
	void mark()
	{
		thread.runLater([this]() { markCalled.store(true); });
	}
};

TEST_F(ConsumerThreadTest, atCreationShouldNotBeRunning)
{
	EXPECT_EQ(false, thread.isRunning());
}

TEST_F(ConsumerThreadTest, operationAppendedWillWait)
{
	mark();
	EXPECT_EQ(false, markCalled.load());
}

TEST_F(ConsumerThreadTest, operationAppendedWillBeProcessed)
{
	thread.blockingStart([this]() {
		mark();
		thread.stop();
	});
	EXPECT_EQ(true, markCalled.load());
}

TEST_F(ConsumerThreadTest, canBeStoppedWhenDetached)
{
	thread.start();
	mark();

	waitUntil([this]() { return markCalled.load(); }, false);

	thread.stop();

	waitUntil([this]() { return thread.isRunning(); }, false);
}

TEST_F(ConsumerThreadTest, cannotBeStartedTwice)
{
	thread.start([this]() { markCalled.store(true); });

	waitUntil([this]() { return markCalled.load(); });

	markCalled.store(false);

	thread.start([this]() { markCalled.store(true); });

	waitUntil([this]() { return markCalled.load(); }, false);
}

TEST_F(ConsumerThreadTest, canBeDestroyedWhileRunning)
{
	auto t = new ConsumerThread();
	t->start();
	delete t;
}

TEST_F(ConsumerThreadTest, canBeDestroyedWhileStopped)
{
	thread.start();
	thread.stop();

	waitUntil([this]() { return thread.isRunning(); }, false);
}

TEST_F(ConsumerThreadTest, canBeDestroyedOnStackWhileRunning)
{
	for (int a = 0; a < 2; a++)
	{
		ConsumerThread t;
		t.start();
	}
}

TEST_F(ConsumerThreadTest, canBeDestoryedOnHeapWhileRunning)
{
	auto t = new utils::ConsumerThread();
	t->start();
	delete t;
}

TEST_F(ConsumerThreadTest, cannotBeBlockStoppedFromHisOwnThread)
{
	thread.start();
	thread.runLater([this]() {
		thread.blockingStop();
		markCalled.store(true);
	});

	waitUntil([this]() { return markCalled.load(); }, true);
	EXPECT_EQ(thread.isRunning(), true);
}

TEST_F(ConsumerThreadTest, canBeBlockedFronHisOwnThread)
{
	thread.start();
	thread.runLater([this]() { thread.stop(); });

	waitUntil([this]() { return thread.isRunning(); }, false);
}

TEST_F(ConsumerThreadTest, startCleansQueue)
{
	mark();
	thread.start();
	thread.blockingStop();
	EXPECT_EQ(false, markCalled.load());
}

TEST_F(ConsumerThreadTest, canBeRestarted)
{
	thread.start();
	mark();

	waitUntil([this]() { return markCalled.load(); }, true);

	thread.blockingStop();
	EXPECT_EQ(false, thread.isRunning());
	markCalled.store(false);

	thread.start();
	mark();

	waitUntil([this]() { return markCalled.load(); }, true);
}

TEST_F(ConsumerThreadTest, testPreAndPostFunction)
{
	std::atomic<bool> a(false);
	std::atomic<bool> b(false);
	ConsumerThread consumer;
	consumer.start([&a]() { a.store(true); }, [&b] { b.store(true); });

	waitUntil([&a]() { return a.load(); }, true);
	EXPECT_EQ(b.load(), false);

	consumer.stop();

	waitUntil([&b]() { return b.load(); }, true);
}
