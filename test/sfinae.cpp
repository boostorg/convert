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

//[has_member_declaration
namespace { namespace local
{
    BOOST_DECLARE_HAS_MEMBER(has_begin, begin);
    BOOST_DECLARE_HAS_MEMBER(  has_end, end);
    BOOST_DECLARE_HAS_MEMBER(has_funop, operator());
}}
//]

//[has_member_classes_tested
namespace { namespace local
{
    template<typename T> struct is_range
    {
        static bool const value = local::has_begin<T>::value && local::has_end<T>::value;
    };

    struct test1 { int   begin; };
    struct test2 { char* begin(); };
    struct test3 { void  begin(int); };
    struct test4
    {
        char* begin();
        char*   end();
    };
}}
//]

namespace { namespace local
{
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

struct no_range
{
};

struct yes_range1
{
    char* begin();
    char const* begin() const;
    char* end();
    char const* end() const;
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

namespace { namespace callable
{
    struct  test1 { int  operator()(double, std::string); };
    struct  test2 { void operator()(double, std::string); };
    struct  test3 { void operator()(int); };
    struct  test4 { std::string operator()(int) const; };
    struct  test5 { std::string operator()(int, std::string const& =std::string()) const; };


//    struct  test2 { template<typename T> T operator()(int, std::string); };
}}

static
void
test_is_callable()
{
    // C1. Unfortunately, passing 'double' where 'int' is expected returns 'true'.
    //     The same as the following compiles:
    //          void fun(int) {}
    //          fun(double(1));

    BOOST_TEST((boost::cnv::is_callable<callable::test1, int (double, std::string)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test1, double (int, std::string)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test1, void (double, std::string)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test1, void (int, std::string)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test1, void (int, char const*)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test1, int (double, int)>::value == false));
    BOOST_TEST((boost::cnv::is_callable<callable::test1, int (double)>::value == false));

    BOOST_TEST((boost::cnv::is_callable<callable::test2, int  (double, std::string)>::value == false));
    BOOST_TEST((boost::cnv::is_callable<callable::test2, void (double, std::string)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test2, void (   int, std::string)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test2, void (   int, char const*)>::value == true));

    BOOST_TEST((boost::cnv::is_callable<callable::test3,       void (double)>::value == true)); //C1
    BOOST_TEST((boost::cnv::is_callable<callable::test3,       void (int)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test3 const, void (int)>::value == false));

    BOOST_TEST((boost::cnv::is_callable<callable::test4 const, std::string (int)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test4,       std::string (int)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test4 const, void (int)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test4,       void (int)>::value == true));

    BOOST_TEST((boost::cnv::is_callable<callable::test5, std::string (int, std::string)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test5, std::string (int, std::string const&)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test5, void        (int, char const*)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test5,       std::string (int)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test5 const, std::string (int)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test5,       void (int)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test5 const, void (int)>::value == true));
    BOOST_TEST((boost::cnv::is_callable<callable::test5,       void (char const*)>::value == false));
    BOOST_TEST((boost::cnv::is_callable<callable::test5 const, void (char const*)>::value == false));
}

int
main(int argc, char const* argv[])
{
    //[has_member_usage
    BOOST_TEST(local::has_begin<local::test1>::value ==  true);
    BOOST_TEST(local::has_begin<local::test2>::value ==  true);
    BOOST_TEST(local::has_begin<local::test3>::value ==  true);
    BOOST_TEST(local::has_begin<local::test4>::value ==  true);
    BOOST_TEST(local::  has_end<local::test1>::value == false);
    BOOST_TEST(local::  has_end<local::test2>::value == false);
    BOOST_TEST(local::  has_end<local::test3>::value == false);
    BOOST_TEST(local::  has_end<local::test4>::value ==  true);

    BOOST_TEST(local::is_range<local::test1>::value == false);
    BOOST_TEST(local::is_range<local::test4>::value ==  true);
    //]

    //[is_callable_usage
    //]

    test_is_callable();

    BOOST_TEST(boost::cnv::has_funop<local::no1>::value == false);

    BOOST_TEST(boost::cnv::has_funop<local::yes01>::value ==  true);
    BOOST_TEST(boost::cnv::has_funop<local::yes02>::value ==  true);
    BOOST_TEST(boost::cnv::has_funop<local::yes03>::value ==  true);
    BOOST_TEST(boost::cnv::has_funop<local::yes04>::value ==  true);
    BOOST_TEST(boost::cnv::has_funop<local::yes05>::value ==  true);

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
