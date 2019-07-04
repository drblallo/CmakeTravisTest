set(default_build_type "Release")

include(debug_compile_options.cmake)

MACRO(compile_options target)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        message("--clang detected")
        set(CLANG true)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        message("--gnu detected")
        set(GNU true)
    else ()
        message("--unrecognized compiler:${CMAKE_CXX_COMPILER_ID}")
    endif()

    if (CLANG)
        message("--compiling with LLD")
        TARGET_LINK_LIBRARIES(${target} PRIVATE -fuse-ld=lld)
    endif()

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        message("--building for release")
        TARGET_COMPILE_OPTIONS(${target} PRIVATE -O3)
    endif()


    debug_compile_options(${target})
ENDMACRO()
