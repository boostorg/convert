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
            int,
            unsigned int,
            short int,
            unsigned short int,
            long int,
            unsigned long int> managed_types;

        char const* d_format[] = // must be in sync with managed_types.
        {
            "%f",
            "%d",   
            "%u",   
            "%hd", 
            "%hu",
            "%ld",
            "%lu"
        };
        char const* x_format[] = // must be in sync with managed_types.
        {
            "%f",
            "0x%x", 
            "0x%x", 
            "0x%hx",
            "0x%hx",
            "0x%lx",
            "0x%lx"
        };
        char const* o_format[] = // must be in sync with managed_types.
        {
            "%f",
            "0%o",
            "0%o",
            "0%ho",
            "0%ho",
            "0%lo",
            "0%lo"
        };
    }
}

struct boost::scanf_converter
{
    typedef boost::scanf_converter this_type;

    scanf_converter()
    :
        base_(boost::conversion::base::dec)
    {}

    template<typename StringOut, typename TypeIn>
    typename boost::enable_if_c<
		!convert_detail::is_any_string<TypeIn>::value && convert_detail::is_any_string<StringOut>::value, 
		bool>::type
    operator()(TypeIn const& value_in, StringOut& result_out) const
    {
        typedef typename boost::mpl::find<convert_detail::managed_types, TypeIn>::type type_iterator;
        typedef typename type_iterator::pos                                                 type_pos;

        char const* format = base_ == conversion::base::dec ? boost::convert_detail::d_format[type_pos::value]
                           : base_ == conversion::base::hex ? boost::convert_detail::x_format[type_pos::value]
                           : base_ == conversion::base::oct ? boost::convert_detail::o_format[type_pos::value] : (BOOST_ASSERT(0), (char const*) 0);
        int const    bufsz = 256;
        char     buf[bufsz];
        int       num_chars = snprintf(buf, bufsz, format, value_in);
        bool        success = num_chars < bufsz;

        if (success) result_out = buf;
        
        return success;
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

        char const* format = boost::convert_detail::d_format[type_pos::value];
        int       num_read = sscanf(in_type(value_in).c_str(), format, &result_out);
        
        return num_read == 1;
    }
    template<typename TypeOut>
    typename boost::enable_if_c<!convert_detail::is_any_string<TypeOut>::value, bool>::type
    operator()(char const* value_in, TypeOut& result_out) const
    {
        typedef typename boost::mpl::find<convert_detail::managed_types, TypeOut>::type type_iterator;
        typedef typename type_iterator::pos                                                  type_pos;

        char const* format = boost::convert_detail::d_format[type_pos::value];
        int       num_read = sscanf(value_in, format, &result_out);
        
        return num_read == 1;
    }

    this_type&
    operator()(parameter::aux::tag<conversion::parameter::type::base, conversion::base::type const>::type const& arg)
    {
        base_ = arg[conversion::parameter::base];
        return *this;
    }
    
    private:

    boost::conversion::base::type base_;
};

#endif // BOOST_CONVERT_SCANF_BASED_CONVERTER_HPP
