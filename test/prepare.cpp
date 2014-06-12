// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/convert/spirit.hpp>

test::cnv::ints
test::cnv::prepare_ints(int const num_ints)
{
    test::cnv::ints                           ints;
    boost::random::mt19937                     gen (std::time(0));
    boost::random::uniform_int_distribution<> dist (SHRT_MIN, SHRT_MAX); // SHRT_MAX(16) = 32767
//  boost::random::uniform_int_distribution<> dist (INT_MIN, INT_MAX); // INT_MAX(32) = 2147483647

    ints.reserve(num_ints);

    for (int k = 0; k < num_ints; ++k)
        ints.push_back(dist(gen));

    return ints;
}

void
test::cnv::prepare_strs(my_string strs[], int const size)
{
    test::cnv::ints const ints = test::cnv::prepare_ints(size);

    for (int k = 0; k < size; ++k)
    {
        int const   this_i = ints[k];
        std::string this_s = boost::convert<std::string>(this_i, boost::cnv::lexical_cast()).value();
        int const   back_i = boost::convert<int>(this_s, boost::cnv::spirit()).value();

        BOOST_ASSERT(this_i == back_i);

        strs[k] = this_s;
    }
}

