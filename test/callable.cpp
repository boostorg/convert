// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "./test.hpp"

using std::string;
using boost::convert;
using boost::lexical_cast;

//[callable_example1
void plain_old_func(string const& value_in, boost::optional<int>& value_out)
//]
{
    try
    {
        value_out = lexical_cast<int>(value_in);
    }
    catch (...)
    {
    }
}

template<typename TypeIn, typename TypeOut>
void
convert_all(TypeIn const&, boost::optional<TypeOut>&)
{
}

template<typename Type>
void
assign(boost::optional<Type>& value_out, Type const& value_in)
{
    value_out = value_in;
}

struct converter1
{
    template<typename TypeIn, typename TypeOut>
    void
    operator()(TypeIn const&, boost::optional<TypeOut>&) const
    {
    }
};

struct take_double { void operator()(double const&, boost::optional<string>&) const {}};
struct    take_int { void operator()(int const&, boost::optional<string>&) const {}};

void
test::cnv::callables()
{
    typedef boost::function<void (string const& value_in, boost::optional<int>&)> boost_func;

    char const* const str = "-12";

    // Testing old-function-based converter.
    //[callable_example2
    int v01 = convert<int>(str, plain_old_func).value_or(-1);
    //]
    // Testing boost::function-based converter.
    int v02 = convert<int>(str, boost_func(plain_old_func)).value_or(-1);
    // Testing crazy boost::bind-based converter.
    //[callable_example3
    int v03 = convert<int>(str,
                  boost::bind(assign<int>, _2,
                      boost::bind(lexical_cast<int, string>, _1))).value_or(-1);
    //]
    BOOST_TEST(v01 == -12);
    BOOST_TEST(v02 == -12);
    BOOST_TEST(v03 == -12);

    converter1  cnv1;
    take_double cnv2;
    take_int    cnv3;

    convert<int>(str, convert_all<string, int>);
    convert<string>(11, convert_all<int, string>);
    convert<   int>(str,   cnv1);
    convert<string>(11,    cnv1);
    convert<string>(11.23, cnv2);
    convert<string>(11,    cnv2);
    convert<string>(11,    cnv3);
    convert<string>(11.23, cnv3);

    // ^^^^^^^^^^^^^^^^^^^^^^^^
    // When I call convert<string>(11, take_int());
    // MSVC8-11 fail to compile lines 84-89.
    // So, I am trying to figure out what they do not like
}
