// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_TEST_STR_TO_INT_HPP
#define BOOST_CONVERT_TEST_STR_TO_INT_HPP

#include "./test.hpp"

using std::string;

template<typename Converter>
void
test::cnv::str_to_int(Converter const& cnv)
{
    string const           not_int_str = "not an int";
    string const               std_str =  "-11";
    char const* const            c_str = "-123";
    char const             array_str[] = "3456";

    ////////////////////////////////////////////////////////////////////////////
    // Testing various kinds of string containers.
    // Testing implicit conversion directly to TypeOut (int).
    // Testing with the fallback value value provided.
    // On failure returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    int const a00 = boost::convert<int>(not_int_str, cnv).value_or(-1);
    int const a01 = boost::convert<int>(std_str,     cnv).value_or(-1);
    int const a02 = boost::convert<int>(c_str,       cnv).value_or(-1);
    int const a05 = boost::convert<int>(array_str,   cnv).value_or(-1);

    BOOST_TEST(a00 ==   -1); // Failed conversion
    BOOST_TEST(a01 ==  -11);
    BOOST_TEST(a02 == -123);
    BOOST_TEST(a05 == 3456);

    ////////////////////////////////////////////////////////////////////////////
    // Testing "optional"
    ////////////////////////////////////////////////////////////////////////////

    boost::optional<int> const r00 = boost::convert<int>(not_int_str, cnv);
    boost::optional<int> const r01 = boost::convert<int>(std_str,     cnv);
    boost::optional<int> const r02 = boost::convert<int>(c_str,       cnv);
    boost::optional<int> const r05 = boost::convert<int>(array_str,   cnv);

    BOOST_TEST(!r00); // Failed conversion
    BOOST_TEST( r01 && r01.value() ==  -11);
    BOOST_TEST( r02 && r02.value() == -123);
    BOOST_TEST( r05 && r05.value() == 3456);

    ////////////////////////////////////////////////////////////////////////////
    // Testing value() on invalid result.
    ////////////////////////////////////////////////////////////////////////////

    try
    {
        boost::convert<int>(not_int_str, cnv).value();
        BOOST_TEST(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    ////////////////////////////////////////////////////////////////////////////
    // Testing value() on valid result.
    ////////////////////////////////////////////////////////////////////////////

    int a021 = boost::convert<int>(std_str,   cnv).value();
    int a022 = boost::convert<int>(c_str,     cnv).value();
    int a025 = boost::convert<int>(array_str, cnv).value();

    BOOST_TEST(a021 ==  -11);
    BOOST_TEST(a022 == -123);
    BOOST_TEST(a025 == 3456);

//    for (int k = INT_MIN; k <= INT_MAX; ++k)
//    {
//        string str = boost::convert<string>(k, boost::cnv::strtol()).value();
//        int k_back = boost::convert<int>(str, cnv).value();
//
//        BOOST_TEST(k == k_back);
//    }
}

#endif // BOOST_CONVERT_TEST_STR_TO_INT_HPP
