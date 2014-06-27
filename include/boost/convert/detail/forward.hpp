// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_FORWARD_HPP
#define BOOST_CONVERT_FORWARD_HPP

#define BOOST_NO_CXX11_REF_QUALIFIERS

#include <boost/config.hpp>
#include <boost/version.hpp>
#include <boost/optional.hpp>

#if defined(_MSC_VER)
#   pragma warning(disable : 4348)
#   pragma warning(disable : 4800)
#   pragma warning(disable : 4996)
#   pragma warning(disable : 4127)  // conditional expression is constant.
#   pragma warning(disable : 4189)  // local variable is initialized but not referenced.
#   pragma warning(disable : 4100)  // unreferenced formal parameter.
#   pragma warning(disable : 4714)  // marked as __forceinline not #endif

#   define _CRT_SECURE_NO_WARNINGS
#   define snprintf _snprintf
#endif

#if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#undef BOOST_CONVERT_CXX11
#else
#define BOOST_CONVERT_CXX11
#endif

#endif // BOOST_CONVERT_FORWARD_HPP
