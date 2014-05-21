// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;
using std::wstring;

void
test::int_to_string()
{
    boost::cstringstream_converter ccnv; // std::stringstream-based char converter
    boost::wstringstream_converter wcnv; // std::stringstream-based wchar_t converter

    string const    not_int_str = "not an int";
    string const        std_str = "-11";
    char const* const     c_str = "-12";
    wstring const      wstd_str = L"-13";
    wchar_t const* const wc_str = L"-14";
    char const      array_str[] = "-15";

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with various string
    // containers as the fallback values.
    ////////////////////////////////////////////////////////////////////////////

    string                s01 = boost::convert< string>(-1, ccnv).value_or(std_str);
    string                s02 = boost::convert< string>(-2, ccnv).value_or(c_str);
    wstring               s03 = boost::convert<wstring>(-3, wcnv).value_or(wstd_str);
    wstring               s04 = boost::convert<wstring>(-4, wcnv).value_or(wc_str);
    string                s05 = boost::convert< string>(-5, ccnv).value_or(array_str);
    cnv::result< string> rs01 = boost::convert< string>(-1, ccnv);
    cnv::result< string> rs02 = boost::convert< string>(-2, ccnv);
    cnv::result<wstring> rs03 = boost::convert<wstring>(-3, wcnv);
    cnv::result<wstring> rs04 = boost::convert<wstring>(-4, wcnv);
    cnv::result< string> rs05 = boost::convert< string>(-5, ccnv);

    BOOST_TEST(s01 ==  "-1"); BOOST_TEST(rs01 && rs01.value() ==  "-1");
    BOOST_TEST(s02 ==  "-2"); BOOST_TEST(rs02 && rs02.value() ==  "-2");
    BOOST_TEST(s03 == L"-3"); BOOST_TEST(rs03 && rs03.value() == L"-3");
    BOOST_TEST(s04 == L"-4"); BOOST_TEST(rs04 && rs04.value() == L"-4");
    BOOST_TEST(s05 ==  "-5"); BOOST_TEST(rs05 && rs05.value() ==  "-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with no fallback value.
    ////////////////////////////////////////////////////////////////////////////

    string                s11 = boost::convert< string>(-1, ccnv).value();
    wstring               s12 = boost::convert<wstring>(-2, wcnv).value();
    cnv::result< string> rs11 = boost::convert< string>(-1, ccnv);
    cnv::result<wstring> rs12 = boost::convert<wstring>(-2, wcnv);

    BOOST_TEST( s11 ==  "-1");
    BOOST_TEST( s12 == L"-2");
    BOOST_TEST(rs11 && rs11.value() ==  "-1");
    BOOST_TEST(rs12 && rs12.value() == L"-2");

    ////////////////////////////////////////////////////////////////////////////
    // Testing conversion::result<> interface.
    // conversion::result exhibits the SAME (but delayed) behavior, i.e.
    // for failed conversion it throws on an attempt to retrieve the value
    // as there is nothing to return.
    ////////////////////////////////////////////////////////////////////////////

    cnv::result<int> const r010 = boost::convert<int>(not_int_str, ccnv);
    cnv::result<int> const r011 = boost::convert<int>(std_str, ccnv);
    cnv::result<int> const r012 = boost::convert<int>(c_str, ccnv);
    cnv::result<int> const r013 = boost::convert<int>(wstd_str, wcnv);
    cnv::result<int> const r014 = boost::convert<int>(wc_str, wcnv);
    cnv::result<int> const r015 = boost::convert<int>(array_str, ccnv);

    BOOST_TEST(!r010); // Failed conversion
    BOOST_TEST( r011 && r011.value() == -11);
    BOOST_TEST( r012 && r012.value() == -12);
    BOOST_TEST( r013 && r013.value() == -13);
    BOOST_TEST( r014 && r014.value() == -14);
    BOOST_TEST( r015 && r015.value() == -15);

    try
    {
        r010.value(); // Throws on an attempt to retrieve the value.
        BOOST_TEST(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
}
