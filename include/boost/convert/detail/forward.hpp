// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_FORWARD_HPP
#define BOOST_CONVERT_FORWARD_HPP

#if defined(_MSC_VER)
#   pragma warning(disable : 4244)
#   pragma warning(disable : 4224)
#   pragma warning(disable : 4996)
#   pragma warning(disable : 4180) // qualifier applied to function type has no meaning

#   define snprintf _snprintf
#endif

#if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#undef BOOST_CONVERT_CXX11
#else
#define BOOST_CONVERT_CXX11
#endif

#define BOOST_NO_CXX11_REF_QUALIFIERS

#include <boost/config.hpp>
#include <boost/version.hpp>
#include <boost/optional.hpp>

#endif // BOOST_CONVERT_FORWARD_HPP
