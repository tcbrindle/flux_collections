
cmake_minimum_required(VERSION 3.23)

project(flux-collections CXX)

set(CMAKE_CXX_EXTENSIONS Off)

include(FetchContent)

FetchContent_Declare(
    flux
    GIT_REPOSITORY https://github.com/tcbrindle/flux.git
    GIT_TAG main
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(flux)

add_library(flux_collections INTERFACE)
add_library(flux::collections ALIAS flux_collections)

target_sources(flux_collections INTERFACE
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    FILES
        collections/array.hpp
)

target_link_libraries(flux_collections INTERFACE flux::flux)

enable_testing()
add_subdirectory(test)
