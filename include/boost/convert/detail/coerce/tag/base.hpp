//              Copyright Jeroen Habraken 2011.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file ../../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_TAG_BASE_HPP)
#define BOOST_COERCE_TAG_BASE_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

#include <boost/config.hpp>
#include <boost/spirit/home/karma/numeric/int.hpp>
#include <boost/spirit/home/karma/numeric/uint.hpp>
#include <boost/spirit/home/qi/directive/no_case.hpp>
#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/qi/numeric/int.hpp>
#include <boost/spirit/home/qi/numeric/uint.hpp>
#include <boost/spirit/home/qi/operator/optional.hpp>
#include <boost/spirit/home/qi/operator/sequence.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/qi/string/lit.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_signed.hpp>

namespace boost { namespace coerce { namespace tag {

#if defined(BOOST_MSVC)
    #pragma warning(push) 
    #pragma warning(disable: 4348)  // redefinition of default parameter : parameter 2
#endif  // defined(BOOST_MSVC)
    template <unsigned Radix>
    struct base {
        template <typename Target, bool U = is_signed<Target>::value>
        struct parser_base;

        template <typename Target>
        struct parser_base<Target, true>
            : spirit::qi::int_parser<Target, Radix> { };

        template <typename Target>
        struct parser_base<Target, false>
            : spirit::qi::uint_parser<Target, Radix> { };

        template <typename Iterator, typename Target, typename Source>
        struct parser
            : parser_base<Target> {
            parser(tag::base<Radix> const &) {
                BOOST_STATIC_ASSERT(is_integral<Target>::value);
            }
        };

        template <typename Source, bool U = is_signed<Source>::value>
        struct generator_base;

        template <typename Source>
        struct generator_base<Source, true>
            : spirit::karma::int_generator<Source, Radix> { };

        template <typename Source>
        struct generator_base<Source, false>
            : spirit::karma::uint_generator<Source, Radix> { };
    
        template <typename Iterator, typename Target, typename Source>
        struct generator
            : generator_base<Source> {
            generator(tag::base<Radix> const &) {
                BOOST_STATIC_ASSERT(is_integral<Source>::value);
            }
        };
    };
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif  // defined(BOOST_MSVC)

    struct bin
        : base<2> { };

    struct oct
        : base<8> { };

    struct hex
        : base<16> {
        template <typename Iterator, typename Target, typename Source>
        struct parser
            : spirit::qi::parser<parser<Iterator, Target, Source> > {
            parser(tag::hex const &) {
                BOOST_STATIC_ASSERT(is_integral<Target>::value);
            }

            template <typename Context, typename Iterator_>
            struct attribute {
                typedef Target type;
            };
            
            template <typename Context, typename Skipper>
            inline bool
            parse(
                Iterator & first,
                Iterator const & last,
                Context & context,
                Skipper const & skipper,
                Target & target
            ) const {
                return spirit::compile<spirit::qi::domain>(
                    -spirit::standard::no_case_type()["0x"] >> parser_base<Target>()
                ).parse(first, last, context, skipper, target);
            }
        };
    };

} } }  // namespace boost::coerce::tag

#endif  // !defined(BOOST_COERCE_TAG_BASE_HPP)
