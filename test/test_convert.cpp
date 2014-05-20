// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#if !defined(_MSC_VER)
#include "./sfinae.cpp"
#include "./algorithms.cpp"
#include "./performance.cpp"
#include "./encryption.cpp"
#include "./callable.cpp"
#endif

using std::string;
using std::wstring;

template<typename Converter>
void
test::type_to_string(Converter const& cnv)
{
    BOOST_TEST("255" == boost::cnv<std::string>(255, cnv(arg::base = cnv::base::dec)).value_or("bad"));
    BOOST_TEST( "ff" == boost::cnv<std::string>(255, cnv(arg::base = cnv::base::hex)).value_or("bad"));
    BOOST_TEST("377" == boost::cnv<std::string>(255, cnv(arg::base = cnv::base::oct)).value_or("bad"));
}

template<typename Converter>
void
test::string_to_type(Converter const& cnv)
{
    BOOST_TEST( 255 == boost::cnv<int>("255", cnv(arg::base = cnv::base::dec)).value_or(999));
    BOOST_TEST( 999 == boost::cnv<int>( "FF", cnv(arg::base = cnv::base::dec)).value_or(999));
    BOOST_TEST( 255 == boost::cnv<int>( "FF", cnv(arg::base = cnv::base::hex)).value_or(999));
    BOOST_TEST( 173 == boost::cnv<int>("255", cnv(arg::base = cnv::base::oct)).value_or(999));
    BOOST_TEST( 999 != boost::cnv<double>("1.23", cnv).value_or(999));
}

int
main(int argc, char const* argv[])
{
	test::sfinae();
    test::string_to_type(boost::strtol_converter()); 
    test::string_to_type(boost::printf_converter());
	test::type_to_string(boost::printf_converter());
	test::algorithms();
	test::performance();
    test::encryption();
    test::callables();

    string const           not_int_str = "not an int";
    string const               std_str = "-11";
    char const* const            c_str = "-12";
    wstring const             wstd_str = L"-13";
    wchar_t const* const        wc_str = L"-14";
    char const             array_str[] = "-15";
    char const* const    container_str = "-16";
    std::vector<char> const vector_str (container_str, container_str + strlen(container_str));
    std::list<char> const     list_str (container_str, container_str + strlen(container_str));

    boost::lexical_cast_converter  lcnv; // lexical_cast-based converter
    boost::cstringstream_converter ccnv; // stringstream-based char converter
	boost::wstringstream_converter wcnv; // stringstream-based wchar_t converter
		
    ////////////////////////////////////////////////////////////////////////////
    // Testing lexical_cast-based converter.
    ////////////////////////////////////////////////////////////////////////////

    int lc00 = boost::cnv<int>(not_int_str, lcnv).value_or(-1);
    int lc01 = boost::cnv<int>(std_str,     lcnv).value_or(-1);
    int lc02 = boost::cnv<int>(c_str,       lcnv).value_or(-1);

    BOOST_TEST(lc00 ==  -1); // Failed conversion. No throw
    BOOST_TEST(lc01 == -11);
    BOOST_TEST(lc02 == -12);

    ////////////////////////////////////////////////////////////////////////////
    // Testing various kinds of string containers.
    // Testing implicit conversion directly to TypeOut (int).
    // Testing with the fallback value value provided.
    // On failure returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    int a000 = boost::cnv<int>(not_int_str, ccnv).value_or(-1);
    int a001 = boost::cnv<int>(std_str,     ccnv).value_or(-1);
    int a002 = boost::cnv<int>(c_str,       ccnv).value_or(-1);
    int a003 = boost::cnv<int>(wstd_str,    wcnv).value_or(-1);
    int a004 = boost::cnv<int>(wc_str,      wcnv).value_or(-1);
    int a005 = boost::cnv<int>(array_str,   ccnv).value_or(-1);

    BOOST_TEST(a000 ==  -1); // Failed conversion
    BOOST_TEST(a001 == -11);
    BOOST_TEST(a002 == -12);
    BOOST_TEST(a003 == -13);
    BOOST_TEST(a004 == -14);
    BOOST_TEST(a005 == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing with the fallback value value provided.
    // Testing implicit conversion to conversion::result<TypeOut>.
    // conversion::result<> exhibits the SAME behavior, i.e.
    // on failure it returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    boost::conversion::result<int> r000 = boost::cnv<int>(not_int_str, ccnv);
    boost::conversion::result<int> r001 = boost::cnv<int>(std_str,     ccnv);
    boost::conversion::result<int> r002 = boost::cnv<int>(c_str,       ccnv);
    boost::conversion::result<int> r003 = boost::cnv<int>(wstd_str,    wcnv);
    boost::conversion::result<int> r004 = boost::cnv<int>(wc_str,      wcnv);
    boost::conversion::result<int> r005 = boost::cnv<int>(array_str,   ccnv);

    BOOST_TEST(!r000); // Failed conversion
    BOOST_TEST( r001 && r001.value() == -11);
    BOOST_TEST( r002 && r002.value() == -12);
    BOOST_TEST( r003 && r003.value() == -13);
    BOOST_TEST( r004 && r004.value() == -14);
    BOOST_TEST( r005 && r005.value() == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing without a fallback value provided.
    // On failure it throws as there is nothing to return.
    // Requires TypeOut to be DefaultConstructible.
    ////////////////////////////////////////////////////////////////////////////

    try
    {
        boost::cnv<int>(not_int_str, ccnv).value();
        BOOST_TEST(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    int a021 = boost::cnv<int>(std_str,   ccnv).value();
    int a022 = boost::cnv<int>(c_str,     ccnv).value();
    int a023 = boost::cnv<int>(wstd_str,  wcnv).value();
    int a024 = boost::cnv<int>(wc_str,    wcnv).value();
    int a025 = boost::cnv<int>(array_str, ccnv).value();

    BOOST_TEST(a021 == -11);
    BOOST_TEST(a022 == -12);
    BOOST_TEST(a023 == -13);
    BOOST_TEST(a024 == -14);
    BOOST_TEST(a025 == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing conversion::result<> interface.
    // conversion::result exhibits the SAME (but delayed) behavior, i.e.
    // for failed conversion it throws on an attempt to retrieve the value
    // as there is nothing to return.
    ////////////////////////////////////////////////////////////////////////////

    boost::conversion::result<int> r010 = boost::cnv<int>(not_int_str, ccnv);
    boost::conversion::result<int> r011 = boost::cnv<int>(std_str, ccnv);
    boost::conversion::result<int> r012 = boost::cnv<int>(c_str, ccnv);
    boost::conversion::result<int> r013 = boost::cnv<int>(wstd_str, wcnv);
    boost::conversion::result<int> r014 = boost::cnv<int>(wc_str, wcnv);
    boost::conversion::result<int> r015 = boost::cnv<int>(array_str, ccnv);

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
    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with various string
    // containers as the fallback values.
    ////////////////////////////////////////////////////////////////////////////

    string                    s001 = boost::cnv< string>(-5, ccnv).value_or(std_str);
    string                    s002 = boost::cnv< string>(-5, ccnv).value_or(c_str);
    wstring                   s003 = boost::cnv<wstring>(-5, wcnv).value_or(wstd_str);
    wstring                   s004 = boost::cnv<wstring>(-5, wcnv).value_or(wc_str);
    string                    s005 = boost::cnv< string>(-5, ccnv).value_or(array_str);
    boost::conversion::result< string> rs001 = boost::cnv< string>(-5, ccnv);
    boost::conversion::result< string> rs002 = boost::cnv< string>(-5, ccnv);
    boost::conversion::result<wstring> rs003 = boost::cnv<wstring>(-5, wcnv);
    boost::conversion::result<wstring> rs004 = boost::cnv<wstring>(-5, wcnv);
    boost::conversion::result< string> rs005 = boost::cnv< string>(-5, ccnv);

    BOOST_TEST(s001 ==  "-5"); BOOST_TEST(rs001 && rs001.value() ==  "-5");
    BOOST_TEST(s002 ==  "-5"); BOOST_TEST(rs002 && rs002.value() ==  "-5");
    BOOST_TEST(s003 == L"-5"); BOOST_TEST(rs003 && rs003.value() == L"-5");
    BOOST_TEST(s004 == L"-5"); BOOST_TEST(rs004 && rs004.value() == L"-5");
    BOOST_TEST(s005 ==  "-5"); BOOST_TEST(rs005 && rs005.value() ==  "-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with no fallback value.
    ////////////////////////////////////////////////////////////////////////////

    string                    s010 = boost::cnv< string>(-5, ccnv).value();
    wstring                   s011 = boost::cnv<wstring>(-5, wcnv).value();
    boost::conversion::result< string> rs010 = boost::cnv< string>(-5, ccnv);
    boost::conversion::result<wstring> rs011 = boost::cnv<wstring>(-5, wcnv);

    BOOST_TEST( s010 ==  "-5");
    BOOST_TEST( s011 == L"-5");
    BOOST_TEST(rs010 && rs010.value() ==  "-5");
    BOOST_TEST(rs011 && rs011.value() == L"-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing conversions of a user-defined type
    ////////////////////////////////////////////////////////////////////////////

    direction const            up_dir1 = direction::up;
    direction const            dn_dir1 = direction::dn;
    string                 up_dir0_str = boost::cnv<string>(direction::up, ccnv).value();
    string                 dn_dir0_str = boost::cnv<string>(direction::dn, ccnv).value();
    string                 up_dir1_str = boost::cnv<string>(up_dir1, ccnv).value();
    string                 dn_dir1_str = boost::cnv<string>(dn_dir1, ccnv).value();
    direction                  up_dir2 = boost::cnv<direction>(up_dir1_str, ccnv).value();
    direction                  dn_dir2 = boost::cnv<direction>(dn_dir1_str, ccnv).value();
    direction                  up_dir3 = boost::cnv<direction>(up_dir1_str, ccnv).value();
    direction                  dn_dir3 = boost::cnv<direction>(dn_dir1_str, ccnv).value();
    direction                  dn_dir4 = boost::cnv<direction>("junk", ccnv).value_or(direction::dn);
    boost::conversion::result<direction> up_dir4 = boost::cnv<direction>("junk", ccnv);

    BOOST_TEST(up_dir0_str == "up");
    BOOST_TEST(dn_dir0_str == "dn");
    BOOST_TEST(up_dir1_str == "up");
    BOOST_TEST(dn_dir1_str == "dn");
    BOOST_TEST(up_dir2 == up_dir1);
    BOOST_TEST(dn_dir2 == dn_dir1);
    BOOST_TEST(up_dir3 == direction::up);
    BOOST_TEST(dn_dir3 == direction::dn);
    BOOST_TEST(dn_dir4 == direction::dn);
    BOOST_TEST(!up_dir4); // Failed conversion

    ////////////////////////////////////////////////////////////////////////////
    // Testing formatters/manipulators
    ////////////////////////////////////////////////////////////////////////////

    try
    {
        boost::lexical_cast<int>("FF");
        BOOST_TEST(!"We should not be here");
    }
    catch (...)
    {
    }

    int hex_v01 = boost::cnv<int>("FF", ccnv(std::hex)).value_or(0);
    int hex_v02 = boost::cnv<int>(L"F", wcnv(std::hex)).value_or(0);
    int hex_v03 = boost::cnv<int>("FF", ccnv(std::dec)).value_or(-5);
    int hex_v04 = boost::cnv<int>(L"F", wcnv(std::dec)).value_or(-5);

    BOOST_TEST(hex_v01 == 255); // "FF"
    BOOST_TEST(hex_v02 ==  15); // L"F"
    BOOST_TEST(hex_v03 ==  -5);
    BOOST_TEST(hex_v04 ==  -5);

    ccnv(std::showbase)(std::uppercase)(std::hex);

    BOOST_TEST(boost::cnv<string>(255, ccnv).value() == "0XFF");
    BOOST_TEST(boost::cnv<string>( 15, ccnv).value() ==  "0XF");

    char const*  double_s01 = test::is_msc ? "1.2345E-002"
                            : test::is_gcc ? "1.2345E-02"
                            : "";
//  ccnv(std::setprecision(4))(std::uppercase)(std::scientific);
    ccnv(arg::precision = 4)
        (arg::uppercase = true)
        (arg::notation = cnv::notation::scientific);

    double double_v01 = boost::cnv<double>(double_s01, ccnv).value();
    string double_s02 = boost::cnv<string>(double_v01, ccnv).value();

    BOOST_TEST(double_s01 == double_s02);

    ////////////////////////////////////////////////////////////////////////////
    // Testing locale
    ////////////////////////////////////////////////////////////////////////////
    char const* eng_locale_name = test::is_msc ? "" // I do not know MCS presentation of US locale
                                : test::is_gcc ? "en_US.UTF-8"
                                : "";
    char const* rus_locale_name = test::is_msc ? "Russian_Russia.1251"
                                : test::is_gcc ? "ru_RU.UTF-8"
                                : "";
    char const*    rus_expected = test::is_msc ? "1,235e-002" : test::is_gcc ? "1,235e-02" : "";
    char const*    eng_expected = test::is_msc ? "1.235e-002" : test::is_gcc ? "1.235e-02" : "";
    std::locale      rus_locale;
    std::locale      eng_locale;

    try { rus_locale = std::locale(rus_locale_name); }
    catch (...) { printf("Test failed: bad locale %s.\n", rus_locale_name); exit(1); }

    try { eng_locale = std::locale(eng_locale_name); }
    catch (...) { printf("Test failed: bad locale %s.\n", eng_locale_name); exit(1); }

//  ccnv(std::setprecision(3))(std::nouppercase);
    ccnv(arg::precision = 3)(arg::uppercase = false);

    string double_rus = boost::cnv<string>(double_v01, ccnv(rus_locale)).value();
    string double_eng = boost::cnv<string>(double_v01, ccnv(eng_locale)).value();
    printf("rus locale=%s, presentation=%s.\n", rus_locale.name().c_str(), double_rus.c_str());
    printf("eng locale=%s, presentation=%s.\n", eng_locale.name().c_str(), double_eng.c_str());

    BOOST_TEST(double_rus == rus_expected);
    BOOST_TEST(double_eng == eng_expected);

    ////////////////////////////////////////////////////////////////////////////
    // Testing string-to-bool and bool-to-string conversions
    ////////////////////////////////////////////////////////////////////////////

//    boost::conversion::result<bool>t bool_res1 = boost::cnv<bool>("1",     false, ccnv);
//    boost::conversion::result<bool>t bool_res2 = boost::cnv<bool>("true",  false, ccnv);
//    boost::conversion::result<bool>t bool_res3 = boost::cnv<bool>("yes",   false, ccnv);
//    boost::conversion::result<bool>t bool_res4 = boost::cnv<bool>(L"1",    false, wcnv);
//    boost::conversion::result<bool>t bool_res5 = boost::cnv<bool>(L"true", false, wcnv);
//    boost::conversion::result<bool>t bool_res6 = boost::cnv<bool>(L"yes",  false, wcnv);
//    boost::conversion::result<bool>t bool_res7 = boost::cnv<bool>("junk",   true, ccnv);
//
//    BOOST_TEST( bool_res1 && bool_res1.value() == true);
//    BOOST_TEST( bool_res2 && bool_res2.value() == true);
//    BOOST_TEST( bool_res3 && bool_res3.value() == true);
//    BOOST_TEST( bool_res4 && bool_res4.value() == true);
//    BOOST_TEST( bool_res5 && bool_res5.value() == true);
//    BOOST_TEST( bool_res6 && bool_res6.value() == true);
//    BOOST_TEST(!bool_res7 && bool_res7.value() == true);
//
//    string bool_str1 = boost::cnv<string>(true,  ccnv).value_or("failed");
//    string bool_str2 = boost::cnv<string>(false, ccnv).value_or("failed");
//    string bool_str3 = boost::cnv<string>(1,     ccnv).value_or("failed");
//    string bool_str4 = boost::cnv<string>(0,     ccnv).value_or("failed");
//
//    BOOST_TEST(bool_str1 == "1");
//    BOOST_TEST(bool_str2 == "0");
//    BOOST_TEST(bool_str3 == "1");
//    BOOST_TEST(bool_str4 == "0");

    return boost::report_errors();
}
