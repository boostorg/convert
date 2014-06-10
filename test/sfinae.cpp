// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

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

    bool q01 = boost::cnv::is_any_string<std::string>::value;
    bool q02 = boost::cnv::is_any_string<char const*>::value;
    bool q03 = boost::cnv::is_any_string<std::wstring>::value;
    bool q04 = boost::cnv::is_any_string<wchar_t const*>::value;
    bool q05 = boost::cnv::is_any_string<char []>::value;
    bool q06 = boost::cnv::is_any_string<wchar_t []>::value;
    bool q07 = boost::cnv::is_any_string<std::vector<char> >::value;
    bool q08 = boost::cnv::is_any_string<std::list<wchar_t> >::value;
    bool q98 = boost::cnv::is_any_string<int>::value;
    bool q99 = boost::cnv::is_any_string<local::a_struct>::value;

    BOOST_TEST( q01);
    BOOST_TEST( q02);
    BOOST_TEST( q03);
    BOOST_TEST( q04);
    BOOST_TEST( q05);
    BOOST_TEST( q06);
    BOOST_TEST(!q07); // Support withdrawn. So, evaluates to false.
    BOOST_TEST(!q08); // Support withdrawn. So, evaluates to false.
    BOOST_TEST(!q98);
    BOOST_TEST(!q99);
}
