// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP
#define BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <boost/convert/detail/forward.hpp>
#include <boost/convert/detail/string.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_string.hpp>

#include <boost/convert/detail/coerce/detail/karma.hpp>

namespace boost { namespace cnv
{
    struct spirit;

    namespace detail
    {
        template <typename Source>
        struct generator : boost::spirit::traits::create_generator<Source>::type {};

        template <> struct generator<      float> : coerce::detail::real_generator<float> {};
        template <> struct generator<     double> : coerce::detail::real_generator<double> {};
        template <> struct generator<long double> : coerce::detail::real_generator<long double> {};
    }
}}

struct boost::cnv::spirit : public boost::cnv::detail::cnvbase<boost::cnv::spirit>
{
    typedef boost::cnv::spirit                     this_type;
    typedef boost::cnv::detail::cnvbase<this_type> base_type;

    using base_type::operator();

    template<typename string_type> void operator()(        int v, optional<string_type>& r) const { to_str(v, r); }
    template<typename string_type> void operator()(   long int v, optional<string_type>& r) const { to_str(v, r); }
    template<typename string_type> void operator()(     double v, optional<string_type>& r) const { to_str(v, r); }
    template<typename string_type> void operator()(long double v, optional<string_type>& r) const { to_str(v, r); }

    template<typename string_type> void operator()(string_type const& s, optional<        int>& r) const { str_to(s, r); }
    template<typename string_type> void operator()(string_type const& s, optional<   long int>& r) const { str_to(s, r); }
    template<typename string_type> void operator()(string_type const& s, optional<     double>& r) const { str_to(s, r); }
    template<typename string_type> void operator()(string_type const& s, optional<long double>& r) const { str_to(s, r); }

    template<typename string_type, typename out_type>
    void
    str_to(string_type const& string_in, optional<out_type>& result_out) const
    {
        typedef typename boost::range_iterator<string_type const>::type     iterator;
        typedef typename boost::iterator_range<iterator>::type        iterator_range;
        typedef typename boost::spirit::traits::create_parser<out_type>::type parser;

        iterator_range range = boost::as_literal(string_in);
        iterator         beg = range.begin();
        iterator         end = range.end();
        out_type      result;

        if (boost::spirit::qi::parse(beg, end, parser(), result))
            if (beg == end) // ensure the whole string has been parsed
                result_out = result;
    }
    template<typename in_type, typename string_type>
    void
    to_str(in_type const& value_in, optional<string_type>& result_out) const
    {
        typedef char const* iterator;
        typedef cnv::detail::generator<in_type> generator;

        char buf[bufsize_];
        char*          beg = buf + bufsize_ / 2;
        char*          end = beg;
        bool          good = boost::spirit::karma::generate(end, generator(), value_in);

        if (good)
            result_out = base_type::format<string_type>(beg, end, buf);
//            result_out = coerce::as<TypeOut>(value_in, coerce::tag::none());

            // TODO: Just adding 'if' costs about 15-20% performance
            //       compared to that same but DIRECT call to coerce::as or qi::parse.
            //  /**/ if (base_ == 10) result_out = coerce::as<TypeOut>(value_in, coerce::tag::none());
            //  else if (base_ ==  8) result_out = coerce::as<TypeOut>(value_in, coerce::tag::oct());
            //  else if (base_ == 16) result_out = coerce::as<TypeOut>(value_in, coerce::tag::hex());
    }
};

#endif // BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP

