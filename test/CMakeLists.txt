
include(FetchContent)

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG f981c9cbcac07a2690e5a86767eba490b5465463 # v3.5.1
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(Catch2)

add_executable(test-flux-collections
    test_array.cpp
)

target_link_libraries(test-flux-collections PUBLIC flux::collections Catch2::Catch2WithMain)
target_compile_definitions(test-flux-collections PUBLIC
    FLUX_UNWIND_ON_ERROR
    FLUX_ERROR_ON_OVERFLOW
    FLUX_DISABLE_STATIC_BOUNDS_CHECKING
)

target_compile_options(test-flux-collections PUBLIC
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU,Intel>:
    -Wall -Wextra -Wconversion -pedantic
    -fno-omit-frame-pointer
    -ftemplate-backtrace-limit=0
    >

    $<$<CXX_COMPILER_ID:GNU>: -fconcepts-diagnostics-depth=2>

    $<$<CXX_COMPILER_ID:Clang,AppleClang>: -fconstexpr-backtrace-limit=0>

    $<$<CXX_COMPILER_ID:MSVC>:
    # Various options for closer standard conformance
    /utf-8 /Zc:__cplusplus /Zc:throwingNew /Zc:inline /Zc:externConstexpr
    /Zc:templateScope /Zc:checkGwOdr /Zc:enumTypes
    /W4
    /wd4459 # local variable name hides global variable
    /wd4702 # unreachable code
    >
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(test-flux-collections)
