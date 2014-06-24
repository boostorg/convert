// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP
#define BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <boost/convert/detail/coerce/coerce.hpp>
#include <boost/convert/detail/coerce/tag/base.hpp>

namespace boost { namespace cnv
{
    struct spirit;
}}

struct boost::cnv::spirit : public boost::cnv::detail::cnvbase<boost::cnv::spirit>
{
    typedef boost::cnv::spirit                     this_type;
    typedef boost::cnv::detail::cnvbase<this_type> base_type;

    using base_type::operator();

    template<typename TypeIn, typename TypeOut>
    void
    operator()(TypeIn const& value_in, boost::optional<TypeOut>& result_out) const
    {
        try
        {
            /**/ if (base_ == 10) result_out = coerce::as<TypeOut>(value_in);
            else if (base_ ==  8) result_out = coerce::as<TypeOut>(value_in, coerce::tag::oct());
            else if (base_ == 16) result_out = coerce::as<TypeOut>(value_in, coerce::tag::hex());
        }
        catch(boost::coerce::bad_cast const&)
        {
        }
    }
};

#ifdef asdasdasdasdasdasd

#include <boost/convert/detail/forward.hpp>
#include <boost/convert/detail/string_sfinae.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_string.hpp>

    template<typename StringIn>
    typename boost::enable_if<cnv::is_any_string<StringIn>, void>::type
    operator()(StringIn const& string_in, boost::optional<int>& result_out) const
    {
        typedef cnv::string_range<StringIn>   range_type;
        typedef typename range_type::char_type char_type;

        char_type const* beg = cnv::string_range<StringIn>::begin(string_in);
        char_type const* end = cnv::string_range<StringIn>::end(string_in);
        int           result;

        if (boost::spirit::qi::parse(beg, end, boost::spirit::int_, result))
            if (beg == end) // ensure the whole string was parsed
                result_out = result;
    }
#endif

#endif // BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP

