// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <boost/detail/lightweight_test.hpp>
//[default_converter_declaration
struct boost::cnv::by_default : public boost::cnv::lexical_cast {};
//]
//[default_converter_headers1
using std::string;
using boost::convert;
//]
int
main(int argc, char const* argv[])
{
    //[default_converter_example1
    int    i = convert<int>("123").value();
    string s = convert<string>(123).value();

    BOOST_TEST(i == 123);
    BOOST_TEST(s == "123");
    //]
}

