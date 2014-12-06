// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include <boost/convert/detail/forward.hpp>

#if defined(BOOST_CONVERT_BROKEN_COMPILER) || defined(BOOST_CONVERT_MSVC_SFINAE_BROKEN)
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

//[is_callable_declaration
namespace { namespace local
{
    BOOST_DECLARE_IS_CALLABLE(can_call_funop, operator());
    BOOST_DECLARE_IS_CALLABLE(can_call_func, func);
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
    struct test2 { char* begin() { return 0; } };
    struct test3 { void  begin(int) {} };
    struct test4
    {
        char* begin() { return 0; }
        char*   end() { return 0; }
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

struct only_end
{
    char* end() { return 0; }
    char const* end() const { return 0; }
};

struct only_begin
{
    char* begin() { return 0; }
    char const* begin() const { return 0; }
};

//[is_callable_classes_tested
namespace { namespace callable
{
    struct  test1 { int  operator()(double, std::string) { return 0; }};
    struct  test2 { void operator()(double, std::string) {}};
    struct  test3 { void operator()(int) {}};
    struct  test4 { std::string operator()(int) const { return std::string(); }};
    struct  test5 { std::string operator()(int, std::string const& =std::string()) const { return std::string(); }};
    struct  test6 { template<typename T> std::string operator()(T) const { return std::string(); }};
    struct  test7 { template<typename T> T operator()(T) const  { return T(); }};

    struct  test11 { int  func(double, std::string) { return 0; }};
    struct  test12 { void func(double, std::string) {}};
    struct  test13 { void func(int) {}};
    struct  test14 { std::string func(int) const { return std::string(); }};
    struct  test15 { std::string func(int, std::string const& =std::string()) const { return std::string(); }};
    struct  test16 { template<typename T> std::string func(T) const { return std::string(); }};
    struct  test17 { template<typename T> T func(T) const { return T(); }};
}}
//]

static
void
test_is_callable()
{
    // C1. Unfortunately, passing 'double' where 'int' is expected returns 'true'.
    //     The same as the following (which successfully compiles):
    //          void fun(int) {}
    //          fun(double(1));

    //[is_callable_usage1
    BOOST_TEST((local::can_call_funop<callable::test1, int (double, std::string)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test1, double (int, std::string)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test1, void (double, std::string)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test1, void (int, std::string)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test1, void (int, char const*)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test1, int (double, int)>::value == false));
    BOOST_TEST((local::can_call_funop<callable::test1, int (double)>::value == false));

    BOOST_TEST((local::can_call_funop<callable::test2, int  (double, std::string)>::value == false));
    BOOST_TEST((local::can_call_funop<callable::test2, void (double, std::string)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test2, void (   int, std::string)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test2, void (   int, char const*)>::value == true));
    //]
    BOOST_TEST((local::can_call_funop<callable::test3,       void (double)>::value == true)); //C1
    BOOST_TEST((local::can_call_funop<callable::test3,       void (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test3 const, void (int)>::value == false));

    BOOST_TEST((local::can_call_funop<callable::test4 const, std::string (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test4,       std::string (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test4 const, void (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test4,       void (int)>::value == true));

    BOOST_TEST((local::can_call_funop<callable::test5, std::string (int, std::string)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test5, std::string (int, std::string const&)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test5, void        (int, char const*)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test5,       std::string (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test5 const, std::string (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test5,       void (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test5 const, void (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test5,       void (char const*)>::value == false));
    BOOST_TEST((local::can_call_funop<callable::test5 const, void (char const*)>::value == false));

    BOOST_TEST((local::can_call_funop<callable::test6, std::string (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test6, std::string (std::string)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test6, void        (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test6, void        (std::string)>::value == true));

    BOOST_TEST((local::can_call_funop<callable::test7, std::string (int)>::value == false));
    BOOST_TEST((local::can_call_funop<callable::test7, std::string (std::string)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test7, void        (int)>::value == true));
    BOOST_TEST((local::can_call_funop<callable::test7, void        (std::string)>::value == true));

    //[is_callable_usage2
    BOOST_TEST((local::can_call_func<callable::test11, int (double, std::string)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test11, double (int, std::string)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test11, void (double, std::string)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test11, void (int, std::string)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test11, void (int, char const*)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test11, int (double, int)>::value == false));
    BOOST_TEST((local::can_call_func<callable::test11, int (double)>::value == false));

    BOOST_TEST((local::can_call_func<callable::test12, int  (double, std::string)>::value == false));
    BOOST_TEST((local::can_call_func<callable::test12, void (double, std::string)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test12, void (   int, std::string)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test12, void (   int, char const*)>::value == true));
    //]
    BOOST_TEST((local::can_call_func<callable::test13,       void (double)>::value == true)); //C1
    BOOST_TEST((local::can_call_func<callable::test13,       void (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test13 const, void (int)>::value == false));

    BOOST_TEST((local::can_call_func<callable::test14 const, std::string (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test14,       std::string (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test14 const, void (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test14,       void (int)>::value == true));

    BOOST_TEST((local::can_call_func<callable::test15, std::string (int, std::string)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test15, std::string (int, std::string const&)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test15, void        (int, char const*)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test15,       std::string (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test15 const, std::string (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test15,       void (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test15 const, void (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test15,       void (char const*)>::value == false));
    BOOST_TEST((local::can_call_func<callable::test15 const, void (char const*)>::value == false));

    BOOST_TEST((local::can_call_func<callable::test16, std::string (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test16, std::string (std::string)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test16, void        (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test16, void        (std::string)>::value == true));

    BOOST_TEST((local::can_call_func<callable::test17, std::string (int)>::value == false));
    BOOST_TEST((local::can_call_func<callable::test17, std::string (std::string)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test17, void        (int)>::value == true));
    BOOST_TEST((local::can_call_func<callable::test17, void        (std::string)>::value == true));
}

int
main(int argc, char const* argv[])
{
    //[has_member_usage
    BOOST_TEST(local::has_begin<local::test1>::value == true);
    BOOST_TEST(local::has_begin<local::test2>::value == true);
    BOOST_TEST(local::has_begin<local::test3>::value == true);
    BOOST_TEST(local::has_begin<local::test4>::value == true);

    BOOST_TEST(local::has_end<local::test1>::value == false);
    BOOST_TEST(local::has_end<local::test2>::value == false);
    BOOST_TEST(local::has_end<local::test3>::value == false);
    BOOST_TEST(local::has_end<local::test4>::value ==  true);

    BOOST_TEST(local::is_range<local::test1>::value == false);
    BOOST_TEST(local::is_range<local::test2>::value == false);
    BOOST_TEST(local::is_range<local::test3>::value == false);
    BOOST_TEST(local::is_range<local::test4>::value ==  true);
    //]

    //[is_callable_usage
    //]

    test_is_callable();

    BOOST_TEST(local::has_funop<local::no1>::value == false);

    BOOST_TEST(local::has_funop<local::yes01>::value == true);
    BOOST_TEST(local::has_funop<local::yes02>::value == true);
    BOOST_TEST(local::has_funop<local::yes03>::value == true);
    BOOST_TEST(local::has_funop<local::yes04>::value == true);
    BOOST_TEST(local::has_funop<local::yes05>::value == true);

    BOOST_TEST(local::has_begin<  no_range>::value == false);
    BOOST_TEST(local::has_begin<  only_end>::value == false);
    BOOST_TEST(local::has_begin<only_begin>::value ==  true);
    BOOST_TEST(local::has_begin<yes_range1>::value ==  true);
    BOOST_TEST(local::has_begin<yes_range2>::value ==  true);

    BOOST_TEST(boost::cnv::is_string<direction>::value == false);
    BOOST_TEST(boost::cnv::is_string<std::string>::value == true);
    BOOST_TEST(boost::cnv::is_string<std::wstring>::value == true);
    BOOST_TEST(boost::cnv::is_string<my_string>::value == true);

    return boost::report_errors();
}

#endif
