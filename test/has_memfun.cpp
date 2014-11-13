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
	struct yes1 { void operator() () {} };
    struct yes2 { void operator() () const {} };
    struct yes3 { int  operator() (int) { return 0; } };
    struct yes4 { int  operator() (int) const { return 0; } };

    struct yes5 { int foo; void moo() {} };
    struct yes6 { void  foo () {} };
    struct yes7 { void  foo () const {} };
    struct yes8 { void* foo (char const*, int) { return 0; } };
    struct yes9 { void* foo (char const*, int) const { return 0; } };
}}

void
test::cnv::has_memfun()
{
	BOOST_TEST(boost::cnv::has_callop<local::no1>::value == false);
	BOOST_TEST(        local::has_foo<local::no1>::value == false);

	BOOST_TEST(boost::cnv::has_callop<local::yes1>::value ==  true);
	BOOST_TEST(boost::cnv::has_callop<local::yes2>::value ==  true);
	BOOST_TEST(boost::cnv::has_callop<local::yes3>::value ==  true);
	BOOST_TEST(boost::cnv::has_callop<local::yes4>::value ==  true);

	BOOST_TEST(local::has_foo<local::yes5>::value ==  true);
	BOOST_TEST(local::has_foo<local::yes6>::value ==  true);
	BOOST_TEST(local::has_foo<local::yes7>::value ==  true);
	BOOST_TEST(local::has_foo<local::yes8>::value ==  true);
	BOOST_TEST(local::has_foo<local::yes9>::value ==  true);
}

