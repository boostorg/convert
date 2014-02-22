// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_SCANF_BASED_CONVERTER_HPP
#define BOOST_CONVERT_SCANF_BASED_CONVERTER_HPP

#include "./string_sfinae.hpp"
#include <stdio.h>

namespace boost 
{
    struct scanf_converter;

    namespace convert_detail
    {
        typedef boost::mpl::vector<
            double,
            int> managed_types;

        char const* format[] = // must be in sync with managed_types.
        {
            "%f",
            "%d"
        };
    }
}

struct boost::scanf_converter
{
    template<typename StringOut, typename TypeIn>
    typename boost::enable_if_c<
		!convert_detail::is_any_string<TypeIn>::value && convert_detail::is_any_string<StringOut>::value, 
		bool>::type
    operator()(TypeIn const& value_in, StringOut& result_out) const
    {
        BOOST_ASSERT(!"Not implemented");
        return false;
    }
    template<typename TypeOut, typename StringIn>
    typename boost::enable_if_c<
		convert_detail::is_any_string<StringIn>::value && !convert_detail::is_any_string<TypeOut>::value, 
		bool>::type
    operator()(StringIn const& value_in, TypeOut& result_out) const
    {
        typedef typename boost::mpl::find<convert_detail::managed_types, TypeOut>::type type_iterator;
        typedef typename type_iterator::pos                                                  type_pos;
	    typedef typename convert_detail::corrected<StringIn>::type                            in_type;

        char const* format = boost::convert_detail::format[type_pos::value];
        int       num_read = sscanf(in_type(value_in).c_str(), format, &result_out);
        
        return num_read == 1;
    }
    template<typename TypeOut>
    typename boost::enable_if_c<!convert_detail::is_any_string<TypeOut>::value, bool>::type
    operator()(char const* value_in, TypeOut& result_out) const
    {
        typedef typename boost::mpl::find<convert_detail::managed_types, TypeOut>::type type_iterator;
        typedef typename type_iterator::pos                                                  type_pos;

        char const* format = boost::convert_detail::format[type_pos::value];
        int       num_read = sscanf(value_in, format, &result_out);
        
        return num_read == 1;
    }
};

#endif // BOOST_CONVERT_SCANF_BASED_CONVERTER_HPP
