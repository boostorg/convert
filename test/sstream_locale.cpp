// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;

void
test::sstream_locale()
{
    boost::cstringstream_converter cnv;

    char const* eng_locale_name = test::is_msc ? "" // I do not know MS presentation of US locale
                                : test::is_gcc ? "en_US.UTF-8"
                                : "";
    char const* rus_locale_name = test::is_msc ? "Russian_Russia.1251"
                                : test::is_gcc ? "ru_RU.UTF-8"
                                : "";
    char const*    rus_expected = test::is_msc ? "1,235e-002" : test::is_gcc ? "1,235e-02" : "";
    char const*    eng_expected = test::is_msc ? "1.235e-002" : test::is_gcc ? "1.235e-02" : "";
    std::locale      rus_locale;
    std::locale      eng_locale;

    char const*  double_s01 = test::is_msc ? "1.2345E-002"
                            : test::is_gcc ? "1.2345E-02"
                            : "";
//  cnv(std::setprecision(4))(std::uppercase)(std::scientific);
    cnv(arg::precision = 4)
       (arg::uppercase = true)
       (arg::notation = cnv::notation::scientific);

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
    printf("rus locale=%s, presentation=%s.\n", rus_locale.name().c_str(), double_rus.c_str());
    printf("eng locale=%s, presentation=%s.\n", eng_locale.name().c_str(), double_eng.c_str());

    BOOST_TEST(double_rus == rus_expected);
    BOOST_TEST(double_eng == eng_expected);

}
