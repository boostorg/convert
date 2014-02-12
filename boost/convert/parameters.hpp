// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_PARAMETERS_HPP
#define BOOST_CONVERT_PARAMETERS_HPP

#include "./workarounds.hpp"
#include "./boost_parameter_ext.hpp"

namespace boost
{
    namespace conversion
    {
        namespace detail { struct throw_on_failure_helper {}; }

        typedef int detail::throw_on_failure_helper::* throw_on_failure_t;

        throw_on_failure_t const throw_on_failure = (static_cast<throw_on_failure_t>(0)) ;

        BOOST_PARAMETER_KEYWORD(type, fallback); ///< Provide fallback value.
    }
}

#endif // BOOST_CONVERT_PARAMETERS_HPP
