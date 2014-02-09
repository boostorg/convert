// Boost.Convert library
//
// Copyright (c) 2009-2011 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef __BOOST_CONVERT_TYPE_TO_BOOL_HPP__
#define __BOOST_CONVERT_TYPE_TO_BOOL_HPP__

#include "./api.hpp"

namespace boost {

// Have to have this specialization which repeats the generic.
// Otherwise, gcc 4.2.4 crashes.
template<>
struct convert<bool> // Conversion to Boolean
{
    typedef conversion::result<bool> result;
    template<class TypeIn, class EnableIn =void> struct converter;

    template<class TypeIn>
    static
    converter<TypeIn>
    from(TypeIn const& value_in =TypeIn())
    {
        return converter<TypeIn>(value_in, false, true);
    }

    template<class TypeIn>
    static
    converter<TypeIn>
    from(TypeIn const& value_in, bool fallback_value)
    {
        return converter<TypeIn>(value_in, fallback_value, false);
    }
};

}

#endif // __BOOST_CONVERT_TYPE_TO_BOOL_HPP__
