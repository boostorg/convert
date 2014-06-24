//           Copyright Jeroen Habraken 2011 - 2012.
//
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_TAG_HPP)
#define BOOST_COERCE_TAG_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

#include <boost/convert/detail/coerce/detail/precision.hpp>

#include <boost/spirit/home/karma/auto.hpp>
#include <boost/spirit/home/qi/auto.hpp>

namespace boost { namespace coerce { namespace tag {

    struct none {
        template <typename Iterator, typename Target, typename Source>
        struct parser
            : spirit::traits::create_parser<Target>::type {
            parser(tag::none const &) { }
        };

        template <typename Iterator, typename Target, typename Source>
        struct generator
            : spirit::traits::create_generator<Source>::type {
            generator(tag::none const &) { }
        };

        template <typename Iterator, typename Target>
        struct generator<Iterator, Target, float>
            : detail::real_generator<float> {
            generator(tag::none const &) { }
        };

        template <typename Iterator, typename Target>
        struct generator<Iterator, Target, double>
            : detail::real_generator<double> {
            generator(tag::none const &) { }
        };

        template <typename Iterator, typename Target>
        struct generator<Iterator, Target, long double>
            : detail::real_generator<long double> {
            generator(tag::none const &) { }
        };
    };

} } }  // namespace boost::coerce::tag

#endif  // !defined(BOOST_COERCE_TAG_HPP)
