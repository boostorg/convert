// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include "../example/example.hpp"

static
void
user_type_to_strings()
{
    boost::array<change, 3>             changes1 = {{ change::no, change::up, change::dn }};
    boost::array<change::value_type, 3> changes2 = {{ change::no, change::up, change::dn }};
    std::vector<std::string>            strings1;
    std::vector<std::string>            strings2;
    boost::cnv::cstringstream           cnv;

    std::transform(changes1.begin(), changes1.end(), std::back_inserter(strings1), boost::convert<std::string>(cnv));
    std::transform(changes2.begin(), changes2.end(), std::back_inserter(strings2), boost::convert<std::string>(cnv));

    BOOST_TEST(strings1.size() == 3);
    BOOST_TEST(strings1[0] == "no");
    BOOST_TEST(strings1[1] == "up");
    BOOST_TEST(strings1[2] == "dn");

    BOOST_TEST(strings2.size() == 3);
    BOOST_TEST(strings2[0] == "0");
    BOOST_TEST(strings2[1] == "1");
    BOOST_TEST(strings2[2] == "2");
}

static
void
ints_to_strings()
{
    boost::array<int, 4>      integers = {{ 15, 16, 17, 18 }};
    std::vector<std::string>   strings;
    boost::cnv::cstringstream cnv;

    cnv(std::hex)(std::uppercase)(std::showbase);

    std::transform(
        integers.begin(),
        integers.end(),
        std::back_inserter(strings),
        boost::convert<std::string>(cnv));

    BOOST_TEST(strings.size() == 4);
    BOOST_TEST(strings[0] ==  "0XF");
    BOOST_TEST(strings[1] == "0X10");
    BOOST_TEST(strings[2] == "0X11");
    BOOST_TEST(strings[3] == "0X12");
}

static
void
strings_to_ints()
{
    boost::array<char const*, 5> strings = {{ "0XF", "0X10", "0X11", "0X12", "not an int" }};
    std::vector<int>            integers;
    boost::cnv::cstringstream   cnv; // stringstream-based char converter

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
            boost::convert<int>(cnv(std::hex)));

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

#ifdef NOT_AVAILABLE_UNTIL_CPP11

    std::vector<optional<int>> opt_ints;

    std::transform(
        strings.begin(),
        strings.end(),
        std::back_inserter(opt_ints),
        boost::convert<int>(ccnv(arg::base = cnv::base::hex)));

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
}

void
test::cnv::algorithms()
{
    strings_to_ints();
    ints_to_strings();
    user_type_to_strings();
}
