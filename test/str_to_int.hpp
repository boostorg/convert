#ifndef BOOST_CONVERT_TEST_STR_TO_INT_HPP
#define BOOST_CONVERT_TEST_STR_TO_INT_HPP

#include "./test.hpp"

using std::string;

template<typename Converter>
void
test::str_to_int(Converter const& cnv)
{
    string const           not_int_str = "not an int";
    string const               std_str = "-11";
    char const* const            c_str = "-12";
    char const             array_str[] = "-15";
    char const* const    container_str = "-16";

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

    BOOST_TEST(a00 ==  -1); // Failed conversion
    BOOST_TEST(a01 == -11);
    BOOST_TEST(a02 == -12);
    BOOST_TEST(a05 == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing with the fallback value value provided.
    // Testing implicit conversion to cnv::optional<TypeOut>.
    // cnv::optional<> exhibits the SAME behavior, i.e.
    // on failure it returns the provided fallback value and DOES NOT THROW.
    ////////////////////////////////////////////////////////////////////////////

    boost::cnv::optional<int> r000 = boost::convert<int>(not_int_str, cnv);
    boost::cnv::optional<int> r001 = boost::convert<int>(std_str,     cnv);
    boost::cnv::optional<int> r002 = boost::convert<int>(c_str,       cnv);
    boost::cnv::optional<int> r005 = boost::convert<int>(array_str,   cnv);

    BOOST_TEST(!r000); // Failed conversion
    BOOST_TEST( r001 && r001.value() == -11);
    BOOST_TEST( r002 && r002.value() == -12);
    BOOST_TEST( r005 && r005.value() == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing without a fallback value provided.
    // On failure it throws as there is nothing to return.
    // Requires TypeOut to be DefaultConstructible.
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
    int a021 = boost::convert<int>(std_str,   cnv).value();
    int a022 = boost::convert<int>(c_str,     cnv).value();
    int a025 = boost::convert<int>(array_str, cnv).value();

    BOOST_TEST(a021 == -11);
    BOOST_TEST(a022 == -12);
    BOOST_TEST(a025 == -15);

    ////////////////////////////////////////////////////////////////////////////
    // Testing cnv::optional<> interface.
    // cnv::optional exhibits the SAME (but delayed) behavior, i.e.
    // for failed conversion it throws on an attempt to retrieve the value
    // as there is nothing to return.
    ////////////////////////////////////////////////////////////////////////////

    cnv::optional<int> const r010 = boost::convert<int>(not_int_str, cnv);
    cnv::optional<int> const r011 = boost::convert<int>(std_str,     cnv);
    cnv::optional<int> const r012 = boost::convert<int>(c_str,       cnv);
    cnv::optional<int> const r015 = boost::convert<int>(array_str,   cnv);

    BOOST_TEST(!r010); // Failed conversion
    BOOST_TEST( r011 && r011.value() == -11);
    BOOST_TEST( r012 && r012.value() == -12);
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
//    for (int k = INT_MIN; k <= INT_MAX; ++k)
//    {
//        string str = boost::convert<string>(k, boost::cnv::strtol()).value();
//        int k_back = boost::convert<int>(str, cnv).value();
//
//        BOOST_TEST(k == k_back);
//    }
}

#endif // BOOST_CONVERT_TEST_STR_TO_INT_HPP
