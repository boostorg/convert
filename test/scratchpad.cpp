// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/convert/stream.hpp>
#include <boost/detail/lightweight_test.hpp>

using std::string;
using boost::convert;

struct func
{
    double operator()() const { return 0; }
};

void
test::cnv::scratchpad()
{
    boost::cnv::cstream cnv;

    int i1 = convert<int>("11", cnv, func());

    BOOST_TEST(i1 == 11);
}
