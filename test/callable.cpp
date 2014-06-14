// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/function.hpp>

//[callable_example1
void plain_old_func(std::string const& value_in, boost::optional<int>& value_out)
//]
{
    try
    {
        value_out = boost::lexical_cast<int>(value_in);
    }
    catch (...)
    {
    }
}

template<typename TypeIn, typename TypeOut>
void
convert_all_pof(TypeIn const&, boost::optional<TypeOut>&)
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

struct take_double { void operator()(double const&, boost::optional<std::string>&) const {}};
struct    take_int { void operator()(int const&, boost::optional<std::string>&) const {}};

void
test::cnv::callables()
{
    typedef boost::function<void (std::string const& value_in, boost::optional<int>&)> boost_func;

    char const* const str = "-12";

    // Testing old-function-based converter.
    //[callable_example2
    int v01 = boost::convert<int>(str, plain_old_func).value_or(-1);
    //]
    // Testing boost::function-based converter.
    int v02 = boost::convert<int>(str, boost_func(plain_old_func)).value_or(-1);
    // Testing crazy boost::bind-based converter.
    //[callable_example3
    int v03 = boost::convert<int>(str, boost::bind(assign<int>, _2, boost::bind(boost::lexical_cast<int, std::string>, _1))).value_or(-1);
    //]
    BOOST_TEST(v01 == -12);
    BOOST_TEST(v02 == -12);
    BOOST_TEST(v03 == -12);

    boost::convert<int>(str, convert_all_pof<std::string, int>);
    boost::convert<std::string>(11, convert_all_pof<int, std::string>);
    boost::convert<int>(str, converter1());
    boost::convert<std::string>(11, converter1());
    boost::convert<std::string>(11, take_double());
    boost::convert<std::string>(11.23, take_int());
}
