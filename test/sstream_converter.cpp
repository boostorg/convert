// Boost.Convert library test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;

void
test::sstream_converter()
{
    char const* const invalid_input = "7 + 5";

    try
    {
        boost::lexical_cast<int>(invalid_input);
        BOOST_TEST(!"We should not be here");
    }
    catch (...)
    {
    }
    boost::cnv::cstringstream cnv; // std::stringstream-based char converter

    BOOST_TEST(!boost::convert<int>(invalid_input, cnv));
}
