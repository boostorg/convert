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

    std::transform(chgs1.begin(), chgs1.end(), std::back_inserter(strs1), boost::convert<std::string, change>(boost::cref(cnv)));
    std::transform(chgs2.begin(), chgs2.end(), std::back_inserter(strs2), boost::convert<std::string, change>(boost::cref(cnv)));
    std::transform(chgs2.begin(), chgs2.end(), std::back_inserter(strs3), boost::convert<std::string, change::value_type>(boost::cref(cnv)));

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

void
test::cnv::algorithms()
{
    user_type_to_strs();
}
