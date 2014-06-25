//           Copyright Jeroen Habraken 2010 - 2012.
//
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_RESERVE_HPP)
#define BOOST_COERCE_RESERVE_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

#include <boost/convert/detail/coerce/detail/precision.hpp>
#include <boost/convert/detail/coerce/detail/reserve.hpp>
#include <boost/convert/detail/coerce/tag_fwd.hpp>

#include <boost/config.hpp>
#include <boost/integer/static_log2.hpp>
#include <boost/limits.hpp>
#include <boost/optional.hpp>

#include <cstddef>  // std::size_t

namespace boost { namespace coerce { namespace traits {

    template <typename T, typename Tag>
    struct reserve_size_impl {
        BOOST_STATIC_CONSTANT(std::size_t, value = 0);
    };

    template <typename Tag>
    struct reserve_size_impl<char, Tag> {
        BOOST_STATIC_CONSTANT(std::size_t, value = 1);
    };

#if !defined(BOOST_NO_INTRINSIC_WCHAR_T)
    template <typename Tag>
    struct reserve_size_impl<wchar_t, Tag> {
        BOOST_STATIC_CONSTANT(std::size_t, value = 1);
    };
#endif  // !defined(BOOST_NO_INTRINSIC_WCHAR_T)

    template <typename T, typename Tag>
    struct reserve_size_impl_integral {
        BOOST_STATIC_CONSTANT(std::size_t, value =
            std::numeric_limits<T>::is_signed +
            1 +
            std::numeric_limits<T>::digits10);
    };

    template <typename T, unsigned Radix>
    struct reserve_size_impl_integral<T, tag::base<Radix> > {
        BOOST_STATIC_CONSTANT(std::size_t, value =
        1 +
        std::numeric_limits<T>::digits / static_log2<Radix>::value);
    };

    template <typename T>
    struct reserve_size_impl_integral<T, tag::bin> {
        BOOST_STATIC_CONSTANT(std::size_t, value =
            1 +
            std::numeric_limits<T>::digits);
    };

    template <typename T>
    struct reserve_size_impl_integral<T, tag::oct> {
        BOOST_STATIC_CONSTANT(std::size_t, value =
            1 +
            std::numeric_limits<T>::digits / 3);
    };

    template <typename T>
    struct reserve_size_impl_integral<T, tag::hex> {
        BOOST_STATIC_CONSTANT(std::size_t, value =
            1 +
            std::numeric_limits<T>::digits / 4);
    };

    template <typename Tag>
    struct reserve_size_impl<int, Tag>
        : reserve_size_impl_integral<int, Tag> { };

    template <typename Tag>
    struct reserve_size_impl<short, Tag>
        : reserve_size_impl_integral<short, Tag> { };

    template <typename Tag>
    struct reserve_size_impl<long, Tag>
        : reserve_size_impl_integral<long, Tag> { };

    template <typename Tag>
    struct reserve_size_impl<unsigned int, Tag>
        : reserve_size_impl_integral<unsigned int, Tag> { };

    template <typename Tag>
    struct reserve_size_impl<unsigned short, Tag>
        : reserve_size_impl_integral<unsigned short, Tag> { };

    template <typename Tag>
    struct reserve_size_impl<unsigned long, Tag>
        : reserve_size_impl_integral<unsigned long, Tag> { };

#if defined(BOOST_HAS_LONG_LONG)
    template <typename Tag>
    struct reserve_size_impl<boost::long_long_type, Tag>
        : reserve_size_impl_integral<boost::long_long_type, Tag> { };

    template <typename Tag>
    struct reserve_size_impl<boost::ulong_long_type, Tag>
        : reserve_size_impl_integral<boost::ulong_long_type, Tag> { };
#endif  // defined(BOOST_HAS_LONG_LONG)

    template <typename T, typename Tag>
    struct reserve_size_impl_floating_point {
        BOOST_STATIC_CONSTANT(std::size_t, value =
            std::numeric_limits<T>::is_signed +
            8 +
            detail::precision<T>::value);
    };

    template <typename Tag>
    struct reserve_size_impl<float, Tag>
        : reserve_size_impl_floating_point<float, Tag> { };

    template <typename Tag>
    struct reserve_size_impl<double, Tag>
        : reserve_size_impl_floating_point<double, Tag> { };

    template <typename Tag>
    struct reserve_size_impl<long double, Tag>
        : reserve_size_impl_floating_point<long double, Tag> { };

    template <typename Tag>
    struct reserve_size_impl<bool, Tag> {
        BOOST_STATIC_CONSTANT(std::size_t, value = 5);
    };

    template <typename T, typename Tag>
    struct reserve_size_impl<boost::optional<T>, Tag>
        : reserve_size_impl<T, Tag> { };

    template <typename T, typename Tag, typename Enable = void>
    struct reserve_size {
        typedef std::size_t type;
        
        static inline type
        call(T const &, Tag const &) {
            return reserve_size_impl<T, Tag>::value;
        }
    };

} } }  // namespace boost::coerce::traits

#endif  // !defined(BOOST_COERCE_RESERVE_HPP)
