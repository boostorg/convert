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

        BOOST_ASSERT(!"Failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior.
        BOOST_ASSERT(integers.size() == 0);
    }
    try
    {
        std::transform(
            strings.begin(),
            strings.end(),
            std::back_inserter(integers),
            boost::convert<int>::from<std::string>(ccnv(std::hex)));

        BOOST_ASSERT(!"Failed to throw");
    }
    catch (std::exception&)
    {
        // Expected behavior.
        BOOST_ASSERT(integers.size() == 4);
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
        boost::convert<int>::from<std::string>(ccnv(arg::base = cnv::base::hex)).value_or(-1));

    BOOST_ASSERT(integers.size() == 5);
    BOOST_ASSERT(integers[0] == 15);
    BOOST_ASSERT(integers[1] == 16);
    BOOST_ASSERT(integers[2] == 17);
    BOOST_ASSERT(integers[3] == 18);
    BOOST_ASSERT(integers[4] == -1); // Failed conversion

    ////////////////////////////////////////////////////////////////////////////
    // int-to-string conversion. No explicit fallback value.
    ////////////////////////////////////////////////////////////////////////////
    std::transform(
        integers.begin(),
        integers.end(),
        std::back_inserter(new_strings),
        boost::convert<std::string>::from<int>(ccnv(std::dec)));

    BOOST_ASSERT(new_strings.size() == 5);
    BOOST_ASSERT(new_strings[0] == "15");
    BOOST_ASSERT(new_strings[1] == "16");
    BOOST_ASSERT(new_strings[2] == "17");
    BOOST_ASSERT(new_strings[3] == "18");
    BOOST_ASSERT(new_strings[4] == "-1");
}