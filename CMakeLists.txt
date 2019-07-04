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

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
	SET(G3_SHARED_LIB OFF CACHE INTERNAL "")
	message("Building static libs")
else()
	SET(BUILD_SHARED_LIBS ON CACHE INTERNAL "")
	SET(G3_SHARED_LIB ON CACHE INTERNAL "")
	message("Building shared libs")
endif()


include(compile_options.cmake)

project(example)

add_subdirectory(googletest)
add_subdirectory(core)
add_subdirectory(test)

message("COMPILING MAIN")
ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES(main PRIVATE utils pthread) 
TARGET_COMPILE_FEATURES(main PUBLIC cxx_std_17)

compile_options(main)

message("END COMPILING MAIN")

