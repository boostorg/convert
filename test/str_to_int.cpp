// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;
using std::wstring;

void
test::string_to_int()
{
    string const           not_int_str = "not an int";
    string const               std_str = "-11";
    char const* const            c_str = "-12";
    wstring const             wstd_str = L"-13";
    wchar_t const* const        wc_str = L"-14";
    char const             array_str[] = "-15";
    char const* const    container_str = "-16";
    std::vector<char> const vector_str (container_str, container_str + strlen(container_str));
    std::list<char> const     list_str (container_str, container_str + strlen(container_str));

    boost::cstringstream_converter ccnv; // std::stringstream-based char converter
    boost::wstringstream_converter wcnv; // std::stringstream-based wchar_t converter

    ////////////////////////////////////////////////////////////////////////////
    // Testing various kinds of string containers.
    // Testing implicit conversion directly to TypeOut (int).
    // Testing with the fallback value value provided.
    // On failure returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    int const a000 = boost::convert<int>(not_int_str, ccnv).value_or(-1);
    int const a001 = boost::convert<int>(std_str,     ccnv).value_or(-1);
    int const a002 = boost::convert<int>(c_str,       ccnv).value_or(-1);
    int const a003 = boost::convert<int>(wstd_str,    wcnv).value_or(-1);
    int const a004 = boost::convert<int>(wc_str,      wcnv).value_or(-1);
    int const a005 = boost::convert<int>(array_str,   ccnv).value_or(-1);

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

    boost::conversion::result<int> r000 = boost::convert<int>(not_int_str, ccnv);
    boost::conversion::result<int> r001 = boost::convert<int>(std_str,     ccnv);
    boost::conversion::result<int> r002 = boost::convert<int>(c_str,       ccnv);
    boost::conversion::result<int> r003 = boost::convert<int>(wstd_str,    wcnv);
    boost::conversion::result<int> r004 = boost::convert<int>(wc_str,      wcnv);
    boost::conversion::result<int> r005 = boost::convert<int>(array_str,   ccnv);

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
        boost::convert<int>(not_int_str, ccnv).value();
        BOOST_TEST(!"failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior: received 'boost::convert failed' exception. All well.
    }
    int a021 = boost::convert<int>(std_str,   ccnv).value();
    int a022 = boost::convert<int>(c_str,     ccnv).value();
    int a023 = boost::convert<int>(wstd_str,  wcnv).value();
    int a024 = boost::convert<int>(wc_str,    wcnv).value();
    int a025 = boost::convert<int>(array_str, ccnv).value();

    BOOST_TEST(a021 == -11);
    BOOST_TEST(a022 == -12);
    BOOST_TEST(a023 == -13);
    BOOST_TEST(a024 == -14);
    BOOST_TEST(a025 == -15);
}
