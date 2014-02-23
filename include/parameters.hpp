// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_PARAMETERS_HPP
#define BOOST_CONVERT_PARAMETERS_HPP

#include "./details/boost_parameter_ext.hpp"

namespace boost
{
    namespace conversion
    {
        struct base { enum type { dec =10, hex =16, oct =8 };};
        struct notation { enum type { fixed, scientific };};

        namespace parameter
        {
            BOOST_PARAMETER_KEYWORD(type,      base);
            BOOST_PARAMETER_KEYWORD(type,    locale);
            BOOST_PARAMETER_KEYWORD(type, precision);
            BOOST_PARAMETER_KEYWORD(type, uppercase);
            BOOST_PARAMETER_KEYWORD(type,  notation);
        }
    }
}

#endif // BOOST_CONVERT_PARAMETERS_HPP
