// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"
#include <boost/convert.hpp>
#include <boost/detail/lightweight_test.hpp>

using std::string;
using std::wstring;

namespace { namespace local
{
	DECLARE_HAS_MEMBER(has_foo, foo);

	struct  no1 { void zoo () {} };
	struct yes01 { void operator() () {} };
    struct yes02 { void operator() () const {} };
    struct yes03 { int  operator() (int) { return 0; } };
    struct yes04 { int  operator() (int) const { return 0; } };
    struct yes05 { int  operator() (int) const { return 0; } void operator() () const {} };

    struct yes11 { int foo; void moo() {} };
    struct yes12 { void  foo () {} };
    struct yes13 { void  foo () const {} };
    struct yes14 { void* foo (char const*, int) { return 0; } };
    struct yes15 { void* foo (char const*, int) const { return 0; } };
}}

int
main(int argc, char const* argv[])
{
	BOOST_TEST(boost::cnv::has_funop<local::no1>::value == false);
	BOOST_TEST(       local::has_foo<local::no1>::value == false);

	BOOST_TEST(boost::cnv::has_funop<local::yes01>::value ==  true);
	BOOST_TEST(boost::cnv::has_funop<local::yes02>::value ==  true);
	BOOST_TEST(boost::cnv::has_funop<local::yes03>::value ==  true);
	BOOST_TEST(boost::cnv::has_funop<local::yes04>::value ==  true);
	BOOST_TEST(boost::cnv::has_funop<local::yes05>::value ==  true);

	BOOST_TEST(local::has_foo<local::yes11>::value ==  true);
	BOOST_TEST(local::has_foo<local::yes12>::value ==  true);
	BOOST_TEST(local::has_foo<local::yes13>::value ==  true);
	BOOST_TEST(local::has_foo<local::yes14>::value ==  true);
	BOOST_TEST(local::has_foo<local::yes15>::value ==  true);

    return boost::report_errors();
}

