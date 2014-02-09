/// \file boost/convert/string_to_type.hpp
/// Implements string-to-type converter
//
// Copyright (c) 2009-2011 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef __BOOST_CONVERT_STRING_TO_TYPE_HPP__
#define __BOOST_CONVERT_STRING_TO_TYPE_HPP__

#include "./stream_based_string_convertor.hpp"

namespace boost {

/// String-to-type Converter
template<class TypeOut, class EnableOut>
template<class StringIn>
struct convert<TypeOut, EnableOut>::converter<
    StringIn,
    typename boost::enable_if<convert_detail::is_any_string<StringIn> >::type>
:
    public convert_detail::stream_based_string_convertor<StringIn, TypeOut>
{
    typedef convert_detail::stream_based_string_convertor<StringIn, TypeOut> base_type;
    typedef converter                                                        this_type;

    converter(StringIn const& from_str, TypeOut const& default_value, bool dothrow)
    :
        base_type(from_str, default_value, dothrow)
    {}

    /// Specify formatting behavior using std::stream-based manipulator.
    /// For example,
    /// @code
    ///     int value = convert<int>::from("FF", -1) >> std::hex;
    /// @endcode
    template<typename Manipulator>
    this_type& operator>> (Manipulator m) { return (base_type::operator>>(m), *this); }

    template<typename T>
    this_type& operator()(T const& v) { return (base_type::operator()(v), *this); }
};

}

#endif // __BOOST_CONVERT_STRING_TO_TYPE_HPP__
