// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/function.hpp>

static
bool
plain_old_func(std::string const& value_in, boost::optional<int>& value_out)
{
    try
    {
        value_out = boost::lexical_cast<int>(value_in);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

template<typename Type>
bool
assign(boost::optional<Type>& value_out, Type const& value_in)
{
    value_out = value_in;
    return true;
}

void
test::cnv::callables()
{
    typedef boost::function<bool (std::string const& value_in, boost::optional<int>&)> boost_func;

    char const* const str = "-12";

    // Testing old-function-based converter.
    int v01 = boost::convert<int>(str, plain_old_func).value_or(-1);
    // Testing boost::function-based converter.
    int v02 = boost::convert<int>(str, boost_func(plain_old_func)).value_or(-1);
    // Testing crazy boost::bind-based converter.
    int v03 = boost::convert<int>(str, boost::bind(assign<int>, _2, boost::bind(boost::lexical_cast<int, std::string>, _1))).value_or(-1);

    BOOST_TEST(v01 == -12);
    BOOST_TEST(v02 == -12);
    BOOST_TEST(v03 == -12);
}
