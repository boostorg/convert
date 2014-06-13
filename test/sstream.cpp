// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;

static
void
test_skipws()
{
    boost::cnv::cstringstream ccnv;
    boost::cnv::wstringstream wcnv;
    char const*    const cstr_good =  "  123";
    wchar_t const* const wstr_good = L"  123";
    char const*    const  cstr_bad =  "  123 "; // std::skipws only affects leading spaces.
    wchar_t const* const  wstr_bad = L"  123 ";

    ccnv(std::skipws);
    wcnv(std::skipws);

    BOOST_TEST( boost::convert<int>(cstr_good, ccnv).value() == 123);
    BOOST_TEST( boost::convert<int>(wstr_good, wcnv).value() == 123);
    BOOST_TEST(!boost::convert<int>( cstr_bad, ccnv));
    BOOST_TEST(!boost::convert<int>( wstr_bad, wcnv));

    ccnv(std::noskipws);
    wcnv(std::noskipws);

    BOOST_TEST(!boost::convert<int>(cstr_good, ccnv));
    BOOST_TEST(!boost::convert<int>(wstr_good, wcnv));
    BOOST_TEST(!boost::convert<int>( cstr_bad, ccnv));
    BOOST_TEST(!boost::convert<int>( wstr_bad, wcnv));
}

static
void
test_manipulators()
{
    boost::cnv::cstringstream ccnv;
    boost::cnv::wstringstream wcnv;

    int const hex_v01 = boost::convert<int>("FF", ccnv(std::hex)).value_or(0);
    int const hex_v02 = boost::convert<int>(L"F", wcnv(std::hex)).value_or(0);
    int const hex_v03 = boost::convert<int>("FF", ccnv(std::dec)).value_or(-5);
    int const hex_v04 = boost::convert<int>(L"F", wcnv(std::dec)).value_or(-6);

    BOOST_TEST(hex_v01 == 255); // "FF"
    BOOST_TEST(hex_v02 ==  15); // L"F"
    BOOST_TEST(hex_v03 ==  -5); // Failed conversion
    BOOST_TEST(hex_v04 ==  -6); // Failed conversion

    ccnv(std::noshowbase)(std::nouppercase)(std::oct);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "377");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "17");

    ccnv(std::showbase);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "0377");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "017");

    ccnv(std::uppercase)(std::hex);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "0XFF");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "0XF");

    ccnv(std::noshowbase)(std::nouppercase)(std::oct);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "377");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "17");

    ccnv(std::showbase)(arg::uppercase = true)(arg::base = boost::cnv::base::hex);

    BOOST_TEST(boost::convert<string>(255, ccnv).value() == "0XFF");
    BOOST_TEST(boost::convert<string>( 15, ccnv).value() ==  "0XF");
}

static
void
test_locale()
{
    boost::cnv::cstringstream cnv;

    char const* eng_locale_name = test::cnv::is_msc ? "" // I do not know MS presentation of US locale
                                : test::cnv::is_gcc ? "en_US.UTF-8"
                                : "";
    char const* rus_locale_name = test::cnv::is_msc ? "Russian_Russia.1251"
                                : test::cnv::is_gcc ? "ru_RU.UTF-8"
                                : "";
    char const*    rus_expected = test::cnv::is_msc ? "1,235e-002" : test::cnv::is_gcc ? "1,235e-02" : "";
    char const*    eng_expected = test::cnv::is_msc ? "1.235e-002" : test::cnv::is_gcc ? "1.235e-02" : "";
    std::locale      rus_locale;
    std::locale      eng_locale;

    char const*  double_s01 = test::cnv::is_msc ? "1.2345E-002"
                            : test::cnv::is_gcc ? "1.2345E-02"
                            : "";
//  cnv(std::setprecision(4))(std::uppercase)(std::scientific);
    cnv(arg::precision = 4)
       (arg::uppercase = true)
       (arg::notation = boost::cnv::notation::scientific);

    double double_v01 = boost::convert<double>(double_s01, cnv).value();
    string double_s02 = boost::convert<string>(double_v01, cnv).value();

    BOOST_TEST(double_s01 == double_s02);

    try { rus_locale = std::locale(rus_locale_name); }
    catch (...) { printf("Test failed: bad locale %s.\n", rus_locale_name); exit(1); }

    try { eng_locale = std::locale(eng_locale_name); }
    catch (...) { printf("Test failed: bad locale %s.\n", eng_locale_name); exit(1); }

//  cnv(std::setprecision(3))(std::nouppercase);
    cnv(arg::precision = 3)(arg::uppercase = false);

    string double_rus = boost::convert<string>(double_v01, cnv(rus_locale)).value();
    string double_eng = boost::convert<string>(double_v01, cnv(eng_locale)).value();

//  printf("rus locale=%s, presentation=%s.\n", rus_locale.name().c_str(), double_rus.c_str());
//  printf("eng locale=%s, presentation=%s.\n", eng_locale.name().c_str(), double_eng.c_str());

    BOOST_TEST(double_rus == rus_expected);
    BOOST_TEST(double_eng == eng_expected);
}

void
test::cnv::sstream()
{
    test_skipws();
    test_manipulators();
    test_locale();
}
