// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

namespace { namespace local
{
    ///////////////////////////////////////////////////////////////////////////
    // Generate a random number string with N digits
    std::string
    gen_int(int digits, bool negative)
    {
        std::string result;

        if (negative)                       // Prepend a '-'
            result += '-';

        result += '1' + (rand()%9);         // The first digit cannot be '0'

        for (int i = 1; i < digits; ++i)    // Generate the remaining digits
            result += '0' + (rand()%10);
        return result;
    }
}}

test::cnv::ints const&
test::cnv::get_ints()
{
    static test::cnv::ints ints;
    static bool          filled;

    if (!filled)
    {
        boost::random::mt19937                     gen (::time(0));
        boost::random::uniform_int_distribution<> dist (SHRT_MIN, SHRT_MAX); // SHRT_MAX(16) = 32767
//      boost::random::uniform_int_distribution<> dist (INT_MIN, INT_MAX); // INT_MAX(32) = 2147483647

        for (size_t k = 0; k < ints.size(); ++k)
            ints[k] = dist(gen);

        filled = true;
    }
    return ints;
}

#ifdef sdfsdfsdfsdfsdfsdf

test::cnv::strings const&
test::cnv::get_strs()
{
    static test::cnv::strings strings;
    static bool                filled;

    if (!filled)
    {
        test::cnv::ints const& ints = test::cnv::get_ints();

        BOOST_ASSERT(strings.size() == ints.size());

        for (size_t k = 0; k < ints.size(); ++k)
        {
            int const   this_i = ints[k];
            std::string this_s = boost::convert<std::string>(this_i, boost::cnv::lexical_cast()).value();
            int const   back_i = boost::convert<int>(this_s, boost::cnv::spirit()).value();

            BOOST_ASSERT(this_i == back_i);

            strings[k] = this_s;
        }
        filled = true;
    }
    return strings;
}
#endif

test::cnv::strings const&
test::cnv::get_strs()
{
    static test::cnv::strings strings;
    static bool                filled;
    static bool              negative = true;

    if (!filled)
    {
        // Seed the random generator
        srand(time(0));

        for (size_t k = 0; k < strings.size(); ++k)
            strings[k] = local::gen_int(k/2 + 1, negative = !negative);

        printf("Testing: ");

        for (size_t k = 0; k < strings.size(); ++k)
            printf("%s%s", k ? "," : "", strings[k].c_str());

        printf("\n");

        filled = true;
    }
    return strings;
}

