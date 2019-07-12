//
// Created by massimo on 29/07/18.
//

#include "ConsumerThread.hpp"

#include <utility>

namespace utils
{
	constexpr std::chrono::nanoseconds waitingTime(10);
	ConsumerThread::ConsumerThread(): running(false), terminate(false) {}

	ConsumerThread::~ConsumerThread()
	{
		if (std::this_thread::get_id() == workerThreadID.load())
			exit(EXIT_FAILURE);

		blockingStop();
	}

	/**
	 * Starts the thread.
	 */
	void ConsumerThread::start(
			std::function<void()> onStart, std::function<void()> onStop)
	{
		bool expected(false);
		if (!running.compare_exchange_weak(expected, true))
			return;

		while (!actionQueue.empty())
			actionQueue.poll();

		std::thread thread(std::bind(
				&ConsumerThread::run, this, std::move(onStart), std::move(onStop)));

		workerThreadID.store(thread.get_id());
		thread.detach();
	}

	void ConsumerThread::blockingStart(
			std::function<void()> onStart, std::function<void()> onStop)
	{
		bool expected(false);
		if (!running.compare_exchange_weak(expected, true))
			return;

		while (!actionQueue.empty())
			actionQueue.poll();

		run(std::move(onStart), std::move(onStop));
	}

	void ConsumerThread::stop()
	{
		runLater([this]() { terminate.store(true); });
	}

	void ConsumerThread::blockingStop()
	{
		if (std::this_thread::get_id() == workerThreadID.load())
			return;

		stop();

		while (running.load())
			std::this_thread::sleep_for(waitingTime);
	}

	void ConsumerThread::run(
			std::function<void()> onStart, std::function<void()> onStop)
	{
		terminate.store(false);

		if (onStart)
			onStart();

		while (!terminate.load())
		{
			processAll();
			std::this_thread::sleep_for(waitingTime);
		}

		if (onStop)
			onStop();

		running.store(false);
	}

	void ConsumerThread::processAll()
	{
		while (!actionQueue.empty() && !terminate.load())
			actionQueue.poll()();
	}

}	// namespace utils
