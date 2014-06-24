//                 Copyright Adam Merz 2010.
//           Copyright Jeroen Habraken 2010 - 2011.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_DETAIL_RESERVE_HPP)
#define BOOST_COERCE_DETAIL_RESERVE_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

#include <cstddef>  // std::size_t

namespace boost { namespace coerce { namespace detail {

    template <typename Sequence>
    class has_reserve {
        template <typename U, void (U::*)(typename U::size_type) = &U::reserve>
        struct impl { };

        template <typename U>
        static type_traits::yes_type test(U*, impl<U>* = 0);

        template <typename U>
        static type_traits::no_type test(...);

    public:
        BOOST_STATIC_CONSTANT(bool, value =
            sizeof(test<Sequence>(0)) == sizeof(type_traits::yes_type));

        typedef mpl::bool_<value> type;
    };

    template <typename Sequence>
    inline void
    call_reserve_impl(
        Sequence & sequence,
        typename Sequence::size_type const size,
        mpl::true_ const
    ) {
        sequence.reserve(size);
    }

    template <typename Sequence>
    inline void
    call_reserve_impl(
        Sequence const &,
        typename Sequence::size_type const,
        mpl::false_ const
    ) {
        // Missing .reserve()
    }

    template <typename Sequence>
    inline void
    call_reserve(
        Sequence & sequence,
        typename Sequence::size_type const size
    ) {
        call_reserve_impl(
            sequence, size, typename has_reserve<Sequence>::type());
    }

    template <typename Sequence>
    inline void
    call_reserve(
        Sequence const &,
        std::size_t const
    ) {
        // Missing size_type
    }

} } }  // namespace boost::coerce::detail

#endif  // !defined(BOOST_COERCE_DETAIL_RESERVE_HPP)
