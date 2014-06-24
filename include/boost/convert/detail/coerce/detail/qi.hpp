//           Copyright Jeroen Habraken 2010 - 2012.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_DETAIL_QI_HPP)
#define BOOST_COERCE_DETAIL_QI_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

#include <boost/convert/detail/coerce/reserve.hpp>
#include <boost/convert/detail/coerce/string.hpp>
#include <boost/convert/detail/coerce/tag.hpp>

#include <boost/spirit/home/qi/char.hpp>
#include <boost/spirit/home/qi/numeric.hpp>
#include <boost/spirit/home/qi/operator/optional.hpp>

#include <boost/spirit/version.hpp>
#if SPIRIT_VERSION < 0x3000
    #include <boost/spirit/home/qi/parse.hpp>
#else
    #include <boost/spirit/home/qi/core/parse.hpp>
#endif  // SPIRIT_VERSION < 0x3000

namespace boost { namespace coerce { namespace detail {

    struct qi {
        template <typename Target, typename Source, typename Tag>
        static inline bool
        call(Target & target, Source const & source, Tag const & tag) {
            typedef traits::string_traits<Source> string_traits;

            typename string_traits::const_iterator
                begin = string_traits::begin(source), iterator = begin;

            typename string_traits::const_iterator
                end = string_traits::end(source);

            typename Tag::template parser<
                typename string_traits::const_iterator, Target, Source
            > parser(tag);

            bool result = boost::spirit::qi::parse(
                iterator, end, parser, target);

            return result && (iterator == end);
        }
    };

} } }  // namespace boost::coerce::detail

#endif  // !defined(BOOST_COERCE_DETAIL_QI_HPP)
