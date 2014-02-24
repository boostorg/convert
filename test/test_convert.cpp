// Boost.Convert library test and usage example
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#if !defined(_MSC_VER)
#include "./performance.cpp"
#endif

using std::string;
using std::wstring;
using boost::array;
using boost::convert;

namespace cnv = boost::conversion;
namespace arg = boost::conversion::parameter;

bool
my_cypher(std::string const& value_in, std::string& value_out)
{
    size_t const cypher = 'A' - '1';

    value_out = value_in;

    for (string::iterator it = value_out.begin(); it != value_out.end(); ++it)
        (*it < 'A') ? (*it += cypher) : (*it -= cypher);

    return true;
}


template<typename Converter>
void
test_type_to_string_converter(Converter const& cnv)
{
    BOOST_ASSERT("255" == boost::convert<std::string>::from(255, cnv(arg::base = cnv::base::dec)).value_or("bad"));
    BOOST_ASSERT( "ff" == boost::convert<std::string>::from(255, cnv(arg::base = cnv::base::hex)).value_or("bad"));
    BOOST_ASSERT("377" == boost::convert<std::string>::from(255, cnv(arg::base = cnv::base::oct)).value_or("bad"));
}

template<typename Converter>
void
test_string_to_type_converter(Converter const& cnv)
{
    BOOST_ASSERT( 255 == boost::convert<int>::from("255", cnv(arg::base = cnv::base::dec)).value_or(999));
    BOOST_ASSERT( 999 == boost::convert<int>::from( "FF", cnv(arg::base = cnv::base::dec)).value_or(999));
    BOOST_ASSERT( 255 == boost::convert<int>::from( "FF", cnv(arg::base = cnv::base::hex)).value_or(999));
    BOOST_ASSERT( 173 == boost::convert<int>::from("255", cnv(arg::base = cnv::base::oct)).value_or(999));
    BOOST_ASSERT( 999 != boost::convert<double>::from("1.23", cnv).value_or(999));
}

template<typename Type>
bool
assign(Type& value_out, Type const& value_in)
{
    value_out = value_in;
    return true;
}

int
main(int argc, char const* argv[])
{
    test_string_to_type_converter(boost::strtol_converter()); 
    test_string_to_type_converter(boost::printf_converter());
	test_type_to_string_converter(boost::printf_converter());
    
    test::performance();

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
    // Testing crazy in-place converter.
    ////////////////////////////////////////////////////////////////////////////

    int lc99 = convert<int>::from(c_str, boost::bind(assign<int>, _2, boost::bind(boost::lexical_cast<int, std::string>, _1))).value_or(-1);

    BOOST_ASSERT(lc99 == -12);

    ////////////////////////////////////////////////////////////////////////////
    // Testing lexical_cast-based converter.
    ////////////////////////////////////////////////////////////////////////////

    int lc00 = convert<int>::from(not_int_str, lcnv).value_or(-1);
    int lc01 = convert<int>::from(std_str,     lcnv).value_or(-1);
    int lc02 = convert<int>::from(c_str,       lcnv).value_or(-1);

    BOOST_ASSERT(lc00 ==  -1); // Failed conversion. No throw
    BOOST_ASSERT(lc01 == -11);
    BOOST_ASSERT(lc02 == -12);

    ////////////////////////////////////////////////////////////////////////////
    // Testing various kinds of string containers.
    // Testing implicit conversion directly to TypeOut (int).
    // Testing with the fallback value value provided.
    // On failure returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    int a000 = convert<int>::from(not_int_str, ccnv).value_or(-1);
    int a001 = convert<int>::from(std_str,     ccnv).value_or(-1);
    int a002 = convert<int>::from(c_str,       ccnv).value_or(-1);
    int a003 = convert<int>::from(wstd_str,    wcnv).value_or(-1);
    int a004 = convert<int>::from(wc_str,      wcnv).value_or(-1);
    int a005 = convert<int>::from(array_str,   ccnv).value_or(-1);

    BOOST_ASSERT(a000 ==  -1); // Failed conversion
    BOOST_ASSERT(a001 == -11);
    BOOST_ASSERT(a002 == -12);
    BOOST_ASSERT(a003 == -13);
    BOOST_ASSERT(a004 == -14);
    BOOST_ASSERT(a005 == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing with the fallback value value provided.
    // Testing implicit conversion to convert<TypeOut>::result.
    // convert<>::result exhibits the SAME behavior, i.e.
    // on failure it returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    convert<int>::result r000 = convert<int>::from(not_int_str, ccnv);
    convert<int>::result r001 = convert<int>::from(std_str,     ccnv);
    convert<int>::result r002 = convert<int>::from(c_str,       ccnv);
    convert<int>::result r003 = convert<int>::from(wstd_str,    wcnv);
    convert<int>::result r004 = convert<int>::from(wc_str,      wcnv);
    convert<int>::result r005 = convert<int>::from(array_str,   ccnv);

    BOOST_ASSERT(!r000); // Failed conversion
    BOOST_ASSERT( r001 && r001.value() == -11);
    BOOST_ASSERT( r002 && r002.value() == -12);
    BOOST_ASSERT( r003 && r003.value() == -13);
    BOOST_ASSERT( r004 && r004.value() == -14);
    BOOST_ASSERT( r005 && r005.value() == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing without a fallback value provided.
    // On failure it throws as there is nothing to return.
    // Requires TypeOut to be DefaultConstructible.
    ////////////////////////////////////////////////////////////////////////////

    try
    {
        convert<int>::from(not_int_str, ccnv).value();
        BOOST_ASSERT(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    int a021 = convert<int>::from(std_str,   ccnv).value();
    int a022 = convert<int>::from(c_str,     ccnv).value();
    int a023 = convert<int>::from(wstd_str,  wcnv).value();
    int a024 = convert<int>::from(wc_str,    wcnv).value();
    int a025 = convert<int>::from(array_str, ccnv).value();

    BOOST_ASSERT(a021 == -11);
    BOOST_ASSERT(a022 == -12);
    BOOST_ASSERT(a023 == -13);
    BOOST_ASSERT(a024 == -14);
    BOOST_ASSERT(a025 == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing convert<>::result interface.
    // convert<>::result exhibits the SAME (but delayed) behavior, i.e.
    // for failed conversion it throws on an attempt to retrieve the value
    // as there is nothing to return.
    ////////////////////////////////////////////////////////////////////////////

    convert<int>::result r010 = convert<int>::from(not_int_str, ccnv);
    convert<int>::result r011 = convert<int>::from(std_str, ccnv);
    convert<int>::result r012 = convert<int>::from(c_str, ccnv);
    convert<int>::result r013 = convert<int>::from(wstd_str, wcnv);
    convert<int>::result r014 = convert<int>::from(wc_str, wcnv);
    convert<int>::result r015 = convert<int>::from(array_str, ccnv);

    BOOST_ASSERT(!r010); // Failed conversion
    BOOST_ASSERT( r011 && r011.value() == -11);
    BOOST_ASSERT( r012 && r012.value() == -12);
    BOOST_ASSERT( r013 && r013.value() == -13);
    BOOST_ASSERT( r014 && r014.value() == -14);
    BOOST_ASSERT( r015 && r015.value() == -15);

    try
    {
        r010.value(); // Throws on an attempt to retrieve the value.
        BOOST_ASSERT(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with various string
    // containers as the fallback values.
    ////////////////////////////////////////////////////////////////////////////

    string                    s001 = convert< string>::from(-5, ccnv).value_or(std_str);
    string                    s002 = convert< string>::from(-5, ccnv).value_or(c_str);
    wstring                   s003 = convert<wstring>::from(-5, wcnv).value_or(wstd_str);
    wstring                   s004 = convert<wstring>::from(-5, wcnv).value_or(wc_str);
    string                    s005 = convert< string>::from(-5, ccnv).value_or(array_str);
    convert< string>::result rs001 = convert< string>::from(-5, ccnv);
    convert< string>::result rs002 = convert< string>::from(-5, ccnv);
    convert<wstring>::result rs003 = convert<wstring>::from(-5, wcnv);
    convert<wstring>::result rs004 = convert<wstring>::from(-5, wcnv);
    convert< string>::result rs005 = convert< string>::from(-5, ccnv);

    BOOST_ASSERT(s001 ==  "-5"); BOOST_ASSERT(rs001 && rs001.value() ==  "-5");
    BOOST_ASSERT(s002 ==  "-5"); BOOST_ASSERT(rs002 && rs002.value() ==  "-5");
    BOOST_ASSERT(s003 == L"-5"); BOOST_ASSERT(rs003 && rs003.value() == L"-5");
    BOOST_ASSERT(s004 == L"-5"); BOOST_ASSERT(rs004 && rs004.value() == L"-5");
    BOOST_ASSERT(s005 ==  "-5"); BOOST_ASSERT(rs005 && rs005.value() ==  "-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing int-to-string conversion with no fallback value.
    ////////////////////////////////////////////////////////////////////////////

    string                    s010 = convert< string>::from(-5, ccnv).value();
    wstring                   s011 = convert<wstring>::from(-5, wcnv).value();
    convert< string>::result rs010 = convert< string>::from(-5, ccnv);
    convert<wstring>::result rs011 = convert<wstring>::from(-5, wcnv);

    BOOST_ASSERT( s010 ==  "-5");
    BOOST_ASSERT( s011 == L"-5");
    BOOST_ASSERT(rs010 && rs010.value() ==  "-5");
    BOOST_ASSERT(rs011 && rs011.value() == L"-5");

    ////////////////////////////////////////////////////////////////////////////
    // Testing conversions of a user-defined type
    ////////////////////////////////////////////////////////////////////////////

    direction const            up_dir1 = direction::up;
    direction const            dn_dir1 = direction::dn;
    string                 up_dir0_str = convert<string>::from<direction>(direction::up, ccnv).value();
    string                 dn_dir0_str = convert<string>::from<direction>(direction::dn, ccnv).value();
    string                 up_dir1_str = convert<string>::from(up_dir1, ccnv).value();
    string                 dn_dir1_str = convert<string>::from(dn_dir1, ccnv).value();
    direction                  up_dir2 = convert<direction>::from(up_dir1_str, ccnv).value();
    direction                  dn_dir2 = convert<direction>::from(dn_dir1_str, ccnv).value();
    direction                  up_dir3 = convert<direction>::from(up_dir1_str, ccnv).value();
    direction                  dn_dir3 = convert<direction>::from(dn_dir1_str, ccnv).value();
    direction                  dn_dir4 = convert<direction>::from("junk", ccnv).value_or(direction::dn);
    convert<direction>::result up_dir4 = convert<direction>::from("junk", ccnv);

    BOOST_ASSERT(up_dir0_str == "up");
    BOOST_ASSERT(dn_dir0_str == "dn");
    BOOST_ASSERT(up_dir1_str == "up");
    BOOST_ASSERT(dn_dir1_str == "dn");
    BOOST_ASSERT(up_dir2 == up_dir1);
    BOOST_ASSERT(dn_dir2 == dn_dir1);
    BOOST_ASSERT(up_dir3 == direction::up);
    BOOST_ASSERT(dn_dir3 == direction::dn);
    BOOST_ASSERT(dn_dir4 == direction::dn);
    BOOST_ASSERT(!up_dir4); // Failed conversion

    ////////////////////////////////////////////////////////////////////////////
    // Testing with algorithms
    ////////////////////////////////////////////////////////////////////////////

    array<char const*, 5>   strings = {{ "0XF", "0X10", "0X11", "0X12", "not int" }};
    std::vector<int>       integers;
    std::vector<string> new_strings;

    ////////////////////////////////////////////////////////////////////////////
    // String to integer conversion.
    // No explicit fallback, i.e. throws on failure. Hex formatting.
    ////////////////////////////////////////////////////////////////////////////
    try
    {
        std::transform(
            strings.begin(),
            strings.end(),
            std::back_inserter(integers),
            boost::bind(boost::lexical_cast<int, std::string>, _1));

        BOOST_ASSERT(!"We should not be here");
    }
    catch (std::exception&)
    {
        // Expected behavior.
        printf("boost::lexical_cast processed: %d entries.\n", int(integers.size()));
    }
    try
    {
        std::transform(
            strings.begin(),
            strings.end(),
            std::back_inserter(integers),
            convert<int>::from<string>(ccnv(std::hex)));

        BOOST_ASSERT(!"We should not be here");
    }
    catch (std::exception&)
    {
        // Expected behavior.
        printf("boost::convert processed: %d entries.\n", int(integers.size()));
    }
    BOOST_ASSERT(integers[0] == 15);
    BOOST_ASSERT(integers[1] == 16);
    BOOST_ASSERT(integers[2] == 17);
    BOOST_ASSERT(integers[3] == 18);

    integers.clear();

    ////////////////////////////////////////////////////////////////////////////
    // String to integer conversion. Explicit fallback, i.e. no throwing. Hex formatting.
    ////////////////////////////////////////////////////////////////////////////
    std::transform(
        strings.begin(),
        strings.end(),
        std::back_inserter(integers),
        convert<int>::from<string>(ccnv(arg::base = cnv::base::hex)).value_or(-1));

    BOOST_ASSERT(integers[0] == 15);
    BOOST_ASSERT(integers[1] == 16);
    BOOST_ASSERT(integers[2] == 17);
    BOOST_ASSERT(integers[3] == 18);
    BOOST_ASSERT(integers[4] == -1); // Failed conversion

    ////////////////////////////////////////////////////////////////////////////
    // Testing formatters/manipulators
    ////////////////////////////////////////////////////////////////////////////

    try
    {
        boost::lexical_cast<int>("FF");
        BOOST_ASSERT(!"We should not be here");
    }
    catch (...)
    {
    }

    int hex_v01 = convert<int>::from("FF", ccnv(std::hex)).value_or(0);
    int hex_v02 = convert<int>::from(L"F", wcnv(std::hex)).value_or(0);
    int hex_v03 = convert<int>::from("FF", ccnv(std::dec)).value_or(-5);
    int hex_v04 = convert<int>::from(L"F", wcnv(std::dec)).value_or(-5);

    BOOST_ASSERT(hex_v01 == 255); // "FF"
    BOOST_ASSERT(hex_v02 ==  15); // L"F"
    BOOST_ASSERT(hex_v03 ==  -5);
    BOOST_ASSERT(hex_v04 ==  -5);

    ccnv(std::showbase)(std::uppercase)(std::hex);

    BOOST_ASSERT(convert<string>::from(255, ccnv).value() == "0XFF");
    BOOST_ASSERT(convert<string>::from( 15, ccnv).value() ==  "0XF");

    char const*  double_s01 = test::is_msc ? "1.2345E-002"
                            : test::is_gcc ? "1.2345E-02"
                            : "";
//  ccnv(std::setprecision(4))(std::uppercase)(std::scientific);
    ccnv(arg::precision = 4)
        (arg::uppercase = true)
        (arg::notation = cnv::notation::scientific);

    double double_v01 = convert<double>::from(double_s01, ccnv).value();
    string double_s02 = convert<string>::from(double_v01, ccnv).value();

    BOOST_ASSERT(double_s01 == double_s02);

    ////////////////////////////////////////////////////////////////////////////
    // Testing locale
    ////////////////////////////////////////////////////////////////////////////

    char const* rus_locale_name = test::is_msc ? "Russian_Russia.1251"
                                : test::is_gcc ? "ru_RU.UTF-8"
                                : "";
    char const*    rus_expected = test::is_msc ? "1,235e-002" : test::is_gcc ? "1,235e-02" : "";
    char const*    eng_expected = test::is_msc ? "1.235e-002" : test::is_gcc ? "1.235e-02" : "";
    std::locale      rus_locale;
    std::locale      eng_locale;

    try
    {
        rus_locale = std::locale(rus_locale_name);
        eng_locale = std::locale("");
    }
    catch (...)
    {
        printf("Test failed: bad locale %s.\n", rus_locale_name);
        exit(1);
    }

//  ccnv(std::setprecision(3))(std::nouppercase);
    ccnv(arg::precision = 3)(arg::uppercase = false);

    string double_rus = convert<string>::from(double_v01, ccnv(rus_locale)).value();
    string double_eng = convert<string>::from(double_v01, ccnv(eng_locale)).value();
    printf("rus locale=%s, presentation=%s.\n", rus_locale.name().c_str(), double_rus.c_str());
    printf("eng locale=%s, presentation=%s.\n", eng_locale.name().c_str(), double_eng.c_str());

    BOOST_ASSERT(double_rus == rus_expected);
    BOOST_ASSERT(double_eng == eng_expected);

    ////////////////////////////////////////////////////////////////////////////
    // int-to-string conversion. No explicit fallback value.
    ////////////////////////////////////////////////////////////////////////////
    std::transform(
        integers.begin(),
        integers.end(),
        std::back_inserter(new_strings),
        convert<string>::from<int>(ccnv(std::dec)));

//  for (size_t k = 0; k < new_strings.size(); ++k)
//      printf("%d. %s\n", int(k), new_strings[k].c_str());

    BOOST_ASSERT(new_strings[0] == "15");
    BOOST_ASSERT(new_strings[1] == "16");
    BOOST_ASSERT(new_strings[2] == "17");
    BOOST_ASSERT(new_strings[3] == "18");
    BOOST_ASSERT(new_strings[4] == "-1");

    ////////////////////////////////////////////////////////////////////////////
    // Testing string-to-bool and bool-to-string conversions
    ////////////////////////////////////////////////////////////////////////////

//    convert<bool>::result bool_res1 = convert<bool>::from("1",     false, ccnv);
//    convert<bool>::result bool_res2 = convert<bool>::from("true",  false, ccnv);
//    convert<bool>::result bool_res3 = convert<bool>::from("yes",   false, ccnv);
//    convert<bool>::result bool_res4 = convert<bool>::from(L"1",    false, wcnv);
//    convert<bool>::result bool_res5 = convert<bool>::from(L"true", false, wcnv);
//    convert<bool>::result bool_res6 = convert<bool>::from(L"yes",  false, wcnv);
//    convert<bool>::result bool_res7 = convert<bool>::from("junk",   true, ccnv);
//
//    BOOST_ASSERT( bool_res1 && bool_res1.value() == true);
//    BOOST_ASSERT( bool_res2 && bool_res2.value() == true);
//    BOOST_ASSERT( bool_res3 && bool_res3.value() == true);
//    BOOST_ASSERT( bool_res4 && bool_res4.value() == true);
//    BOOST_ASSERT( bool_res5 && bool_res5.value() == true);
//    BOOST_ASSERT( bool_res6 && bool_res6.value() == true);
//    BOOST_ASSERT(!bool_res7 && bool_res7.value() == true);
//
//    string bool_str1 = convert<string>::from(true,  ccnv).value_or("failed");
//    string bool_str2 = convert<string>::from(false, ccnv).value_or("failed");
//    string bool_str3 = convert<string>::from(1,     ccnv).value_or("failed");
//    string bool_str4 = convert<string>::from(0,     ccnv).value_or("failed");
//
//    BOOST_ASSERT(bool_str1 == "1");
//    BOOST_ASSERT(bool_str2 == "0");
//    BOOST_ASSERT(bool_str3 == "1");
//    BOOST_ASSERT(bool_str4 == "0");

    ////////////////////////////////////////////////////////////////////////////
    // Testing custom converter.
    ////////////////////////////////////////////////////////////////////////////

    string encrypted = convert<string>::from("ABC", my_cypher).value();
    string decrypted = convert<string>::from(encrypted, my_cypher).value();

    BOOST_ASSERT(encrypted == "123");
    BOOST_ASSERT(decrypted == "ABC");

    printf("Test completed successfully.\n");
    return 0;
}
