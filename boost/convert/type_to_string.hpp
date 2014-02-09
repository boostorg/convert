// Boost.Convert library
//
// Copyright (c) 2009-2011 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef __BOOST_CONVERT_TYPE_TO_STRING_HPP__
#define __BOOST_CONVERT_TYPE_TO_STRING_HPP__

#include "./stream_based_string_convertor.hpp"

namespace boost {

/// Partial specialization for conversions to std::string
template<class StringOut>
struct convert<
    StringOut,
    typename boost::enable_if<convert_detail::is_std_string<StringOut> >::type>
{
    typedef conversion::result<StringOut> result;

    template<class TypeIn>
    static
    convert_detail::stream_based_string_convertor<TypeIn, StringOut>
    from(TypeIn const& value_in =TypeIn())
    {
        return convert_detail::stream_based_string_convertor<TypeIn, StringOut>(value_in, StringOut(), true);
    }

    template<class TypeIn, class DefaultStringOut>
    static
    convert_detail::stream_based_string_convertor<TypeIn, StringOut>
    from(TypeIn const& value_in, DefaultStringOut const& default_str)
    {
        return convert_detail::stream_based_string_convertor<TypeIn, StringOut>(value_in, default_str, false);
    }
};

} // End of the boost namespace

#endif // __BOOST_CONVERT_TYPE_TO_STRING_HPP__
