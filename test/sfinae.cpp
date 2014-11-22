// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include <boost/convert/detail/forward.hpp>

#ifdef BOOST_CONVERT_BROKEN_COMPILER
int main(int, char const* []) { return 0; }
#else

#include <boost/convert.hpp>
#include <boost/convert/detail/is_string.hpp>
#include <boost/detail/lightweight_test.hpp>
#include "./test.hpp"

namespace { namespace local
{
    DECLARE_HAS_MEMBER(has_begin, begin);
    DECLARE_HAS_MEMBER(  has_end, end);
}}


struct no_range
{
};

struct yes_range1
{
    char* begin() { return 0; }
    char const* begin() const { return 0; }
    char* end() { return 0; }
    char const* end() const { return 0; }
};

struct yes_range2
{
    char* begin() { return 0; }
    char* end() { return 0; }
};

struct no_begin
{
    char* end() { return 0; }
    char const* end() const { return 0; }
};

struct no_end
{
    char* begin() { return 0; }
    char const* begin() const { return 0; }
};

int
main(int argc, char const* argv[])
{
    BOOST_TEST(local::has_begin<no_range>::value == false);
    BOOST_TEST(local::has_begin<no_begin>::value == false);
    BOOST_TEST(local::has_begin<no_end>::value == true);
    BOOST_TEST(local::has_begin<yes_range1>::value == true);
    BOOST_TEST(local::has_begin<yes_range2>::value == true);

    BOOST_TEST(boost::cnv::is_range<no_range>::value == false);
    BOOST_TEST(boost::cnv::is_range<no_begin>::value == false);
    BOOST_TEST(boost::cnv::is_range<no_end>::value == false);
    BOOST_TEST(boost::cnv::is_range<yes_range1>::value == true);
    BOOST_TEST(boost::cnv::is_range<yes_range2>::value == true);
    BOOST_TEST(boost::cnv::is_range<yes_range1 const>::value == true);
    BOOST_TEST(boost::cnv::is_range<yes_range2 const>::value == true);
    BOOST_TEST(boost::cnv::is_range<std::string>::value == true);
    BOOST_TEST(boost::cnv::is_range<std::wstring>::value == true);
//  BOOST_TEST(boost::cnv::is_range<char*>::value == true);
//  BOOST_TEST(boost::cnv::is_range<char[25]>::value == true);
    BOOST_TEST(boost::cnv::is_range<my_string>::value == true);

    BOOST_TEST(boost::cnv::is_string<direction>::value == false);
    BOOST_TEST(boost::cnv::is_string<std::string>::value == true);
    BOOST_TEST(boost::cnv::is_string<std::wstring>::value == true);
    BOOST_TEST(boost::cnv::is_string<my_string>::value == true);

    return boost::report_errors();
}

#endif
