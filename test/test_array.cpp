
#include <catch2/catch_test_macros.hpp>

#include <flux/collections/array.hpp>

namespace {

constexpr bool test_array_construction()
{
    flux::array<int, 3> arr({1, 2, 3});

    using A = decltype(arr);
    static_assert(std::is_trivially_default_constructible_v<A>);
    static_assert(std::is_trivially_copyable_v<A>);

    static_assert(flux::contiguous_sequence<A>);
    static_assert(flux::const_iterable_sequence<A>);
    static_assert(not flux::read_only_sequence<A>);
    static_assert(std::same_as<flux::element_t<A>, int&>);
    static_assert(std::same_as<flux::rvalue_element_t<A>, int&&>);
    static_assert(std::same_as<flux::const_element_t<A>, int const&>);
    static_assert(std::same_as<flux::common_element_t<A>, int&>);
    static_assert(std::same_as<flux::value_t<A>, int>);

    static_assert(arr.size() == 3);

    if (arr[0] != 1) {
        return false;
    }

    return true;
}
static_assert(test_array_construction());

}

TEST_CASE("flux.array")
{
    REQUIRE(test_array_construction());
}