// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;

void
test::cnv::string_to_bool()
{
    ////////////////////////////////////////////////////////////////////////////
    // Testing string-to-bool and bool-to-string conversions
    ////////////////////////////////////////////////////////////////////////////

//    boost::optional<bool>t bool_res1 = boost::convert<bool>("1",     false, ccnv);
//    boost::optional<bool>t bool_res2 = boost::convert<bool>("true",  false, ccnv);
//    boost::optional<bool>t bool_res3 = boost::convert<bool>("yes",   false, ccnv);
//    boost::optional<bool>t bool_res4 = boost::convert<bool>(L"1",    false, wcnv);
//    boost::optional<bool>t bool_res5 = boost::convert<bool>(L"true", false, wcnv);
//    boost::optional<bool>t bool_res6 = boost::convert<bool>(L"yes",  false, wcnv);
//    boost::optional<bool>t bool_res7 = boost::convert<bool>("junk",   true, ccnv);
//
//    BOOST_TEST( bool_res1 && bool_res1.value() == true);
//    BOOST_TEST( bool_res2 && bool_res2.value() == true);
//    BOOST_TEST( bool_res3 && bool_res3.value() == true);
//    BOOST_TEST( bool_res4 && bool_res4.value() == true);
//    BOOST_TEST( bool_res5 && bool_res5.value() == true);
//    BOOST_TEST( bool_res6 && bool_res6.value() == true);
//    BOOST_TEST(!bool_res7 && bool_res7.value() == true);
//
//    string bool_str1 = boost::convert<string>(true,  ccnv).value_or("failed");
//    string bool_str2 = boost::convert<string>(false, ccnv).value_or("failed");
//    string bool_str3 = boost::convert<string>(1,     ccnv).value_or("failed");
//    string bool_str4 = boost::convert<string>(0,     ccnv).value_or("failed");
//
//    BOOST_TEST(bool_str1 == "1");
//    BOOST_TEST(bool_str2 == "0");
//    BOOST_TEST(bool_str3 == "1");
//    BOOST_TEST(bool_str4 == "0");
}

