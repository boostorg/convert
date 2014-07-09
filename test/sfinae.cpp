// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert/detail/string.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <vector>
#include <list>

namespace { namespace local
{
    struct a_struct {};
}}

void
test::cnv::sfinae()
{
    ////////////////////////////////////////////////////////////////////////////
    // Test string SFINAE.
    ////////////////////////////////////////////////////////////////////////////

    bool q01 = boost::cnv::is_string_of<std::string, char>::value;
    bool q02 = boost::cnv::is_string_of<char const*, char>::value;
    bool q05 = boost::cnv::is_string_of<char [], char>::value;

    bool q03 = boost::cnv::is_string_of<std::wstring, wchar_t>::value;
    bool q04 = boost::cnv::is_string_of<wchar_t const*, wchar_t>::value;
    bool q06 = boost::cnv::is_string_of<wchar_t [], wchar_t>::value;

    BOOST_TEST( q01);
    BOOST_TEST( q02);
    BOOST_TEST( q03);
    BOOST_TEST( q04);
    BOOST_TEST( q05);
    BOOST_TEST( q06);
}
