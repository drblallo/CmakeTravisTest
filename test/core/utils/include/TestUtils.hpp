//
// Created by massimo on 02/08/18.
//

#ifndef CULT_TESTUTILS_HPP
#define CULT_TESTUTILS_HPP
#include <chrono>
#include <functional>

void waitUntil(
		std::function<bool()> fun,
		bool expected = true,
		int maxTries = 100,
		std::chrono::milliseconds waitTime = std::chrono::milliseconds(10));

#endif	// CULT_TESTUTILS_HPP
