#include "gtest/gtest.h"

#include "SoutSink.hpp"

class DefaultTest: public testing::Test
{
	public:
	void SetUp() final {}
	void TearDown() final {}
};

TEST_F(DefaultTest, simpleTest) { EXPECT_TRUE(true); }
