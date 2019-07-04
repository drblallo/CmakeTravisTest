cmake_minimum_required(VERSION 3.9)
include("CPack")

SET(BUILD_GMOCK ON CACHE INTERNAL "")
SET(BUILD_GTEST ON CACHE INTERNAL "")
SET(INSTALL_GMOCK OFF CACHE INTERNAL "")
SET(INSTALL_GTEST OFF CACHE INTERNAL "")
SET(gmock_build_tests OFF CACHE INTERNAL "")
SET(gtest_build_samples OFF CACHE INTERNAL "")
SET(gtest_build_tests OFF CACHE INTERNAL "")
SET(gtest_disable_pthreads OFF CACHE INTERNAL "")
SET(gtest_hide_internal_symbols OFF CACHE INTERNAL "")
SET(gtest_force_shared_crt OFF CACHE INTERNAL "")

project(example VERSION 1.0 LANGUAGES CXX)
enable_testing()
include("CTest")
set(CMAKE_DEBUG_POSTFIX "-d")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

add_subdirectory(googletest)
add_subdirectory(test)
add_subdirectory(utils)

ADD_EXECUTABLE(main main.cpp)
ADD_EXECUTABLE(example::main ALIAS main)
TARGET_LINK_LIBRARIES(main PRIVATE example::utils pthread) 
TARGET_COMPILE_FEATURES(main PUBLIC cxx_std_17)

INSTALL(TARGETS main
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include)

set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)
