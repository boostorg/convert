// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

void
test::algorithms()
{
    boost::array<char const*, 5> strings = {{ "0XF", "0X10", "0X11", "0X12", "not int" }};
    std::vector<int>            integers;
    std::vector<std::string> new_strings;
    boost::cstringstream_converter  ccnv; // stringstream-based char converter

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

        BOOST_TEST(!"Failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior.
        BOOST_TEST(integers.size() == 0);
    }
    try
    {
        std::transform(
            strings.begin(),
            strings.end(),
            std::back_inserter(integers),
            boost::convert<int>::from<std::string>(ccnv(std::hex)));

        BOOST_TEST(!"Failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior.
        BOOST_TEST(integers.size() == 4);
    }
    BOOST_TEST(integers[0] == 15);
    BOOST_TEST(integers[1] == 16);
    BOOST_TEST(integers[2] == 17);
    BOOST_TEST(integers[3] == 18);

    integers.clear();

    ////////////////////////////////////////////////////////////////////////////
    // String to integer conversion. Explicit fallback, i.e. no throwing. Hex formatting.
    ////////////////////////////////////////////////////////////////////////////

    std::transform(
        strings.begin(),
        strings.end(),
        std::back_inserter(integers),
        boost::convert<int>::from<std::string>(ccnv(arg::base = cnv::base::hex)).value_or(-1));

    BOOST_TEST(integers.size() == 5);
    BOOST_TEST(integers[0] == 15);
    BOOST_TEST(integers[1] == 16);
    BOOST_TEST(integers[2] == 17);
    BOOST_TEST(integers[3] == 18);
    BOOST_TEST(integers[4] == -1); // Failed conversion

#ifdef NOT_AVAILABLE_UNTIL_CPP11

    std::vector<boost::convert<int>::result> opt_ints;

    std::transform(
        strings.begin(),
        strings.end(),
        std::back_inserter(opt_ints),
        boost::convert<int>::from<std::string>(ccnv(arg::base = cnv::base::hex)));

    BOOST_TEST(opt_ints.size() == 5);
    BOOST_TEST( opt_ints[0]);
    BOOST_TEST( opt_ints[1]);
    BOOST_TEST( opt_ints[2]);
    BOOST_TEST( opt_ints[3]);
    BOOST_TEST(!opt_ints[4]); // Failed conversion
    BOOST_TEST(opt_ints[0].value() == 15);
    BOOST_TEST(opt_ints[1].value() == 16);
    BOOST_TEST(opt_ints[2].value() == 17);
    BOOST_TEST(opt_ints[3].value() == 18);

#endif

    ////////////////////////////////////////////////////////////////////////////
    // int-to-string conversion. No explicit fallback value.
    ////////////////////////////////////////////////////////////////////////////

    std::transform(
        integers.begin(),
        integers.end(),
        std::back_inserter(new_strings),
        boost::convert<std::string>::from<int>(ccnv(std::dec)));

    BOOST_TEST(new_strings.size() == 5);
    BOOST_TEST(new_strings[0] == "15");
    BOOST_TEST(new_strings[1] == "16");
    BOOST_TEST(new_strings[2] == "17");
    BOOST_TEST(new_strings[3] == "18");
    BOOST_TEST(new_strings[4] == "-1");



}
