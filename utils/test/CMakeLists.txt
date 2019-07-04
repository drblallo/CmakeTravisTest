include(GoogleTest)

ADD_EXECUTABLE(utilsTest src/ConsumerThreadTest.cpp src/SafeQueueTest.cpp src/TestUtils.cpp src/TreeTest.cpp)
ADD_EXECUTABLE(example::utilsTest ALIAS utilsTest) 
TARGET_LINK_LIBRARIES(utilsTest PRIVATE gtest gtest_main example::utils)
TARGET_INCLUDE_DIRECTORIES(utilsTest PUBLIC include PRIVATE src)
TARGET_COMPILE_FEATURES(utilsTest PUBLIC cxx_std_17)

gtest_add_tests(TARGET      utilsTest
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)
