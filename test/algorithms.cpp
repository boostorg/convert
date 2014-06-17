// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include "../example/example.hpp"

static
void
user_type_to_strs()
{
    boost::array<change, 3>             chgs1 = {{ change::no, change::up, change::dn }};
    boost::array<change::value_type, 3> chgs2 = {{ change::no, change::up, change::dn }};
    std::vector<std::string>            strs1;
    std::vector<std::string>            strs2;
    std::vector<std::string>            strs3;
    boost::cnv::cstringstream           cnv;

    std::transform(chgs1.begin(), chgs1.end(), std::back_inserter(strs1), boost::convert<std::string, change>(cnv));
    std::transform(chgs2.begin(), chgs2.end(), std::back_inserter(strs2), boost::convert<std::string, change>(cnv));
    std::transform(chgs2.begin(), chgs2.end(), std::back_inserter(strs3), boost::convert<std::string, change::value_type>(cnv));

    BOOST_TEST(strs1.size() == 3);
    BOOST_TEST(strs1[0] == "no");
    BOOST_TEST(strs1[1] == "up");
    BOOST_TEST(strs1[2] == "dn");

//    BOOST_TEST(strs2.size() == 3);
//    BOOST_TEST(strs2[0] == "no");
//    BOOST_TEST(strs2[1] == "up");
//    BOOST_TEST(strs2[2] == "dn");

    BOOST_TEST(strs3.size() == 3);
    BOOST_TEST(strs3[0] == "0");
    BOOST_TEST(strs3[1] == "1");
    BOOST_TEST(strs3[2] == "2");
}

static
void
ints_to_strings()
{
    boost::array<int, 4>     ints = {{ 15, 16, 17, 18 }};
    std::vector<std::string> strs;
    boost::cnv::cstringstream cnv;

    cnv(std::hex)(std::uppercase)(std::showbase);

    std::transform(
        ints.begin(),
        ints.end(),
        std::back_inserter(strs),
        boost::convert<std::string, int>(cnv));

    BOOST_TEST(strs.size() == 4);
    BOOST_TEST(strs[0] ==  "0XF");
    BOOST_TEST(strs[1] == "0X10");
    BOOST_TEST(strs[2] == "0X11");
    BOOST_TEST(strs[3] == "0X12");
}

static
void
strings_to_ints()
{
    boost::array<char const*, 5> strs = {{ "0XF", "0X10", "0X11", "0X12", "not an int" }};
    std::vector<int>             ints;
    boost::cnv::cstringstream     cnv; // stringstream-based char converter

    ////////////////////////////////////////////////////////////////////////////
    // String to integer conversion.
    // No explicit fallback, i.e. throws on failure. Hex formatting.
    ////////////////////////////////////////////////////////////////////////////
    try
    {
        std::transform(
            strs.begin(),
            strs.end(),
            std::back_inserter(ints),
            boost::bind(boost::lexical_cast<int, std::string>, _1));

        BOOST_TEST(!"Failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior.
        BOOST_TEST(ints.size() == 0);
    }
    try
    {
        std::transform(
            strs.begin(),
            strs.end(),
            std::back_inserter(ints),
            boost::convert<int, std::string>(cnv(std::hex)));

        BOOST_TEST(!"Failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior.
        BOOST_TEST(ints.size() == 4);
    }
    BOOST_TEST(ints[0] == 15);
    BOOST_TEST(ints[1] == 16);
    BOOST_TEST(ints[2] == 17);
    BOOST_TEST(ints[3] == 18);

    ints.clear();

#ifdef NOT_AVAILABLE_UNTIL_CPP11

    std::vector<optional<int>> opts;

    std::transform(
        strs.begin(),
        strs.end(),
        std::back_inserter(opts),
        boost::convert<int, std::string>(ccnv(arg::base = cnv::base::hex)));

    BOOST_TEST( opts.size() == 5);
    BOOST_TEST( opts[0]);
    BOOST_TEST( opts[1]);
    BOOST_TEST( opts[2]);
    BOOST_TEST( opts[3]);
    BOOST_TEST(!opts[4]); // Failed conversion
    BOOST_TEST( opts[0].value() == 15);
    BOOST_TEST( opts[1].value() == 16);
    BOOST_TEST( opts[2].value() == 17);
    BOOST_TEST( opts[3].value() == 18);

#endif
}

void
test::cnv::algorithms()
{
    strings_to_ints();
    ints_to_strings();
    user_type_to_strs();
}
