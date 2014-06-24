//              Copyright Jeroen Habraken 2011.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_TAG_PRECISION_HPP)
#define BOOST_COERCE_TAG_PRECISION_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

#include <boost/spirit/home/karma/numeric/real.hpp>
#include <boost/spirit/home/karma/numeric/real_policies.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace coerce { namespace tag {

    struct precision {
        precision(unsigned const & value)
            : value(value) { }

        template <typename Iterator, typename Target, typename Source>
        struct parser {
            BOOST_STATIC_ASSERT(sizeof(Iterator) == 0);
        };

        template <typename Source>
        struct real_policies
            : spirit::karma::real_policies<
                typename remove_const<Source>::type
            > {
            real_policies(unsigned const & value)
                : value(value) { }

            inline unsigned
            precision(Source const &) const {
                return value;
            }

            unsigned value;
        };

        template <typename Iterator, typename Target, typename Source>
        struct generator
            : spirit::karma::real_generator<Source, real_policies<Source> > {
            generator(tag::precision const & tag)
                : spirit::karma::real_generator<
                    Source, real_policies<Source>
                >(real_policies<Source>(tag.value))
            {
                BOOST_STATIC_ASSERT(is_floating_point<Source>::value);
            }
        };

        unsigned value;
    };

} } }  // namespace boost::coerce::tag

#endif  // !defined(BOOST_COERCE_TAG_PRECISION_HPP)
