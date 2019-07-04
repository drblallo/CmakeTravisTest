cmake_minimum_required(VERSION 3.9)

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

project(example)
enable_testing()


add_subdirectory(googletest)
add_subdirectory(test)
add_subdirectory(utils)

ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES(main PRIVATE utils pthread) 
TARGET_COMPILE_FEATURES(main PUBLIC cxx_std_17)

set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)