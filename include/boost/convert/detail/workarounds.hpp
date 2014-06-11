// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_DETAIL_WORKAROUNDS_HPP
#define BOOST_CONVERT_DETAIL_WORKAROUNDS_HPP

#include <boost/version.hpp>

#if defined(_MSC_VER)
#   pragma warning(disable: 4348)
#   pragma warning(disable: 4800)
#   pragma warning(disable: 4996)

#   define snprintf _snprintf
#endif

#define BOOST_NO_CXX11_REF_QUALIFIERS

#endif // BOOST_CONVERT_DETAIL_WORKAROUNDS_HPP
