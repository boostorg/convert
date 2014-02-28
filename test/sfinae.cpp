// Boost.Convert library test and usage example
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

void
test::sfinae()
{
    ////////////////////////////////////////////////////////////////////////////
    // Test string SFINAE.
    ////////////////////////////////////////////////////////////////////////////

    bool q01 = boost::convert_detail::is_any_string<std::string>::value;
    bool q02 = boost::convert_detail::is_any_string<char const*>::value;
    bool q03 = boost::convert_detail::is_any_string<std::wstring>::value;
    bool q04 = boost::convert_detail::is_any_string<wchar_t const*>::value;
    bool q05 = boost::convert_detail::is_any_string<char []>::value;
    bool q06 = boost::convert_detail::is_any_string<wchar_t []>::value;
    bool q07 = boost::convert_detail::is_any_string<std::vector<char> >::value;
    bool q08 = boost::convert_detail::is_any_string<std::list<wchar_t> >::value;
    bool q98 = boost::convert_detail::is_any_string<int>::value;
    bool q99 = boost::convert_detail::is_any_string<direction>::value;

    BOOST_ASSERT( q01);
    BOOST_ASSERT( q02);
    BOOST_ASSERT( q03);
    BOOST_ASSERT( q04);
    BOOST_ASSERT( q05);
    BOOST_ASSERT( q06);
    BOOST_ASSERT(!q07); // Support withdrawn. So, evaluates to false.
    BOOST_ASSERT(!q08); // Support withdrawn. So, evaluates to false.
    BOOST_ASSERT(!q98);
    BOOST_ASSERT(!q99);
}
