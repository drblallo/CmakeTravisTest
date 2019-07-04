//
// Created by massimo on 02/08/18.
//

#include "TestUtils.hpp"

#include <gtest/gtest.h>
#include <thread>

void waitUntil(
		std::function<bool()> fun,
		bool expected,
		int maxTries,
		std::chrono::milliseconds waitTime)
{
	int a(0);
	while (fun() != expected && a++ < maxTries)
		std::this_thread::sleep_for(waitTime);

	EXPECT_EQ(fun(), expected);
}
