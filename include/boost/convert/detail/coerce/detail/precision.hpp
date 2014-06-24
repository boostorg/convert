//           Copyright Jeroen Habraken 2011 - 2012.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_DETAIL_PRECISION_HPP)
#define BOOST_COERCE_DETAIL_PRECISION_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <boost/spirit/home/karma/numeric/real.hpp>
#include <boost/spirit/home/karma/numeric/real_policies.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace coerce { namespace detail {

    template <typename T>
    struct precision {
        typedef std::numeric_limits<T> limits;

        BOOST_STATIC_CONSTANT(bool, is_specialized =
            limits::is_specialized && !limits::is_exact);

        BOOST_STATIC_CONSTANT(bool, is_specialized_binary =
            is_specialized && limits::radix == 2 && limits::digits > 0);

        BOOST_STATIC_CONSTANT(unsigned, precision_binary =
            2UL + limits::digits * 30103UL / 100000UL);

        BOOST_STATIC_CONSTANT(bool, is_specialized_decimal =
            is_specialized && limits::radix == 10 && limits::digits10 > 0);

        BOOST_STATIC_CONSTANT(unsigned, precision_decimal =
            limits::digits10 + 1U);

        BOOST_STATIC_CONSTANT(unsigned, value =
            is_specialized_binary ? precision_binary
                : is_specialized_decimal ? precision_decimal : 6);
    };

    template <typename Source>
    struct real_policies
        : spirit::karma::real_policies<
            typename remove_const<Source>::type
        > {
        static inline unsigned
        precision(Source const &) {
            return detail::precision<Source>::value;
        }
    };

    template <typename Source>
    struct real_generator
        : spirit::karma::real_generator<
            Source, detail::real_policies<Source>
        > { };

} } }  // namespace boost::coerce::detail

#endif  // !defined(BOOST_COERCE_DETAIL_PRECISION_HPP)
