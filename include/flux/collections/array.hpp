
#ifndef FLUX_COLLECTIONS_ARRAY_HPP_INCLUDED
#define FLUX_COLLECTIONS_ARRAY_HPP_INCLUDED

#include <flux.hpp>

namespace flux::inline coll {

template <typename T, distance_t N>
using c_array = T[N];

template <typename T, distance_t N>
    requires std::is_object_v<T> && (N > 0)
struct array : inline_sequence_base<array<T, N>> {
private:
    c_array<T, N> data_;

    template <typename U, std::size_t... I>
    constexpr array(c_array<U, N>&& arr, std::index_sequence<I...>)
        : data_(std::move(arr[I])...)
    {}

public:
    using value_type = T;
    using cursor_type = index_t;

    array() = default;

    template <typename Arg0, typename... Args>
        requires (!decays_to<Arg0, array>) &&
                 std::constructible_from<T[N], Arg0, Args...>
    constexpr array(Arg0&& arg0, Args&&... args)
        : data_{FLUX_FWD(arg0), FLUX_FWD(args)...}
    {}

    template <typename U>
    constexpr array(c_array<U, N>&& arr)
        : array(std::move(arr), std::make_index_sequence<N>{})
    {}

    friend bool constexpr operator==(array const& lhs, array const& rhs)
    {
        for (distance_t idx = 0; idx < N; ++idx) {
            if (lhs.data_[idx] != rhs.data_[idx]) {
                return false;
            }
        }
        return true;
    }

    friend auto constexpr operator<=>(array const& lhs, array const& rhs)
        -> std::compare_three_way_result_t<T, T>
        requires std::three_way_comparable<T>
    {
        for (distance_t idx = 0; idx < N; ++idx) {
            if (auto r = lhs.data_[idx] <=> rhs.data_[idx]; r != 0) {
                return r;
            }
        }
        return std::strong_ordering::equal;
    }

    struct flux_sequence_traits {
        static constexpr auto first(const array&) -> index_t { return 0; }

        static constexpr auto last(const array&) -> index_t { return N; }

        static constexpr auto is_last(const array&, index_t idx) -> bool { return idx >= N; }

        static constexpr auto inc(const array&, index_t& idx, distance_t offset = 1) {
            idx = num::checked_add(idx, offset);
        }

        static constexpr auto dec(const array& self, index_t& idx) { inc(self, idx, -1); }

        static constexpr auto read_at(auto& self, index_t idx) -> decltype(auto)
        {
            indexed_bounds_check(idx, N);
            return self.data_[idx];
        }

        static constexpr auto size(const array&) -> distance_t { return N; }

        static constexpr auto distance(const array&, index_t from, index_t to) -> distance_t
        {
            return num::checked_sub(to, from);
        }

        static constexpr auto data(auto& self) -> auto* { return self.data_; }

        static constexpr auto for_each_while(auto& self, auto&& pred) -> index_t
        {
            index_t idx = 0;
            for (; idx < N; ++idx) {
                if (!std::invoke(pred, self.data_[idx])) {
                    break;
                }
            }
            return idx;
        }
    };
};

template <typename T, typename... Us>
    requires (std::same_as<T, Us> && ...)
array(T, Us...) -> array<T, 1 + sizeof...(Us)>;

template <typename T, distance_t N>
array(c_array<T, N>&&) -> array<T, N>;

}

#endif // FLUX_COLLECTIONS_ARRAY_HPP_INCLUDED
