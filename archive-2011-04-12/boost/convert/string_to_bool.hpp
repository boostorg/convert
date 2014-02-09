/// \file boost/convert/string_to_bool.hpp
/// Implements string-to-bool converter
//
// Copyright (c) 2009-2011 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.


#ifndef __BOOST_CONVERT_STRING_TO_BOOL_HPP__
#define __BOOST_CONVERT_STRING_TO_BOOL_HPP__

#include "./type_to_bool.hpp"
#include "./converter_base.hpp"
#include <boost/algorithm/string.hpp>

// A fairly half-hearted implementation of conversion to Boolean
// to demonstrate the idea of specializing/optimizing for a particular type
// with no underlying std::stream and, therefore, no manipulators.

namespace boost {

namespace convert_detail {

template<class Char> struct bool_constant;

/// Recognised string values
template<>
struct bool_constant<char>
{
    static char const* const* all_1s () { static char const* values[] = { "1", "yes", "true", 0 }; return values; }
    static char const* const* all_0s () { static char const* values[] = { "0", "no", "false", 0 }; return values; }
};

/// Recognised string values
template<>
struct bool_constant<wchar_t>
{
    static wchar_t const* const* all_1s () { static wchar_t const* values[] = { L"1", L"yes", L"true", 0 }; return values; }
    static wchar_t const* const* all_0s () { static wchar_t const* values[] = { L"0", L"no", L"false", 0 }; return values; }
};

}

/// Partial specialization for TypeOut = bool, TypeIn = char string
template<class StringIn>
struct convert<bool>::converter<
    StringIn,
    typename boost::enable_if<convert_detail::is_any_string<StringIn> >::type>
:
    public convert_detail::converter_base<convert<bool>::converter<StringIn, void>, StringIn, bool>
{
    typedef converter                                                 this_type;
    typedef convert_detail::converter_base<this_type, StringIn, bool> base_type;
    typedef typename base_type::corrected_in_type             corrected_in_type;
    typedef typename corrected_in_type::char_type                     char_type;
    typedef typename base_type::optional_out_type             optional_out_type;

    converter(StringIn const& from_str, bool default_value, bool dothrow)
    :
        base_type(from_str, default_value, dothrow)
    {}

    optional_out_type convert_(corrected_in_type const& value_in) // Called in converter_base.
    {
        corrected_in_type        value = boost::to_lower_copy(value_in);
        char_type const* const* all_0s = convert_detail::bool_constant<char_type>::all_0s();
        char_type const* const* all_1s = convert_detail::bool_constant<char_type>::all_1s();
        bool                     got_0 = false;
        bool                     got_1 = false;

        for (; *all_0s; ++all_0s) got_0 |= 0 == value.compare(*all_0s);
        for (; *all_1s; ++all_1s) got_1 |= 0 == value.compare(*all_1s);

        return (got_1 || got_0) ? optional_out_type(got_1) : optional_out_type();
    }
};

}

#endif // __BOOST_CONVERT_STRING_TO_BOOL_HPP__
