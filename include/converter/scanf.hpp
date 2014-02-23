// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_SCANF_CONVERTER_HPP
#define BOOST_CONVERT_SCANF_CONVERTER_HPP

#include "./printf.hpp"
#include <stdio.h>

namespace boost 
{
    struct scanf_converter;
}

struct boost::scanf_converter
{
    typedef boost::scanf_converter this_type;

	scanf_converter() : base_(10) {}
	
    template<typename TypeOut, typename StringIn>
    typename boost::enable_if_c<
		convert_detail::is_any_string<StringIn>::value && !convert_detail::is_any_string<TypeOut>::value, 
		bool>::type
    operator()(StringIn const& value_in, TypeOut& result_out) const
    {
        typedef typename boost::mpl::find<convert_detail::managed_types, TypeOut>::type type_iterator;
        typedef typename type_iterator::pos                                                  type_pos;
	    typedef typename convert_detail::corrected<StringIn>::type                            in_type;

        char const* format = base_ == 10 ? boost::convert_detail::d_format[type_pos::value]
                           : base_ == 16 ? boost::convert_detail::x_format[type_pos::value]
                           : base_ ==  8 ? boost::convert_detail::o_format[type_pos::value] : (BOOST_ASSERT(0), (char const*) 0);
        int       num_read = sscanf(in_type(value_in).c_str(), format, &result_out);
        
        return num_read == 1;
    }
    template<typename TypeOut>
    typename boost::enable_if_c<!convert_detail::is_any_string<TypeOut>::value, bool>::type
    operator()(char const* value_in, TypeOut& result_out) const
    {
        typedef typename boost::mpl::find<convert_detail::managed_types, TypeOut>::type type_iterator;
        typedef typename type_iterator::pos                                                  type_pos;

        char const* format = base_ == 10 ? boost::convert_detail::d_format[type_pos::value]
                           : base_ == 16 ? boost::convert_detail::x_format[type_pos::value]
                           : base_ ==  8 ? boost::convert_detail::o_format[type_pos::value] : (BOOST_ASSERT(0), (char const*) 0);
        int       num_read = sscanf(value_in, format, &result_out);
        
        return num_read == 1;
    }

    this_type const&
    operator()(parameter::aux::tag<conversion::parameter::type::base, conversion::base::type const>::type const& arg) const
    {
        boost::conversion::base::type base = arg[conversion::parameter::base];
        
        base_ = base == conversion::base::dec ? 10
              : base == conversion::base::hex ? 16
              : base == conversion::base::oct ? 8 : 0;

        return *this;
    }
    
    private:

    int mutable base_;
};

#endif // BOOST_CONVERT_SCANF_CONVERTER_HPP
