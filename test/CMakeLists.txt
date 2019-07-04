include(GoogleTest)

ADD_EXECUTABLE(defaultTest src/DefaultTest.cpp)
ADD_EXECUTABLE(example::defaultTest ALIAS defaultTest)

TARGET_LINK_LIBRARIES(defaultTest PRIVATE gtest gtest_main example::utils)
TARGET_INCLUDE_DIRECTORIES(defaultTest PUBLIC include PRIVATE src)
TARGET_COMPILE_FEATURES(defaultTest PUBLIC cxx_std_17)

gtest_add_tests(TARGET      defaultTest
                TEST_SUFFIX .noArgs
                TEST_LIST   noArgsTests
)


