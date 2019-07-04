message("COMPILING TESTS")
ADD_EXECUTABLE(runTest src/DefaultTest.cpp)

TARGET_LINK_LIBRARIES(runTest PRIVATE gtest gtest_main utils)
TARGET_INCLUDE_DIRECTORIES(runTest PUBLIC include PRIVATE src)
TARGET_COMPILE_FEATURES(runTest PUBLIC cxx_std_17)

add_subdirectory(core)

compile_options(runTest)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT CULT_TSAN)
	message("--enabling death test")
	TARGET_COMPILE_DEFINITIONS(runTest PUBLIC CULT_DEATH_TEST)	
elseif()
	message("--death test disabled")
endif()

message("END COMPILING TEST")
