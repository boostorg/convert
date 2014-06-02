// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_DETAIL_STRING_SFINAE_HPP
#define BOOST_CONVERT_DETAIL_STRING_SFINAE_HPP

#include <boost/type_traits.hpp>
#include <boost/type_traits/decay.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range.hpp>
#include <boost/range/as_literal.hpp>

namespace boost { namespace conversion {

using ::boost::type_traits::yes_type;
using ::boost::type_traits:: no_type;

template<class String, class Char>
class is_c_string_of
{
    typedef
        typename boost::remove_const<
            typename boost::remove_pointer<
                typename boost::remove_reference<
                    typename boost::decay<String>::type /* Char(&)[] -> Char* */
                >::type
            >::type
        >::type* ptr_type;

    static no_type  test (...);
    static yes_type test (Char*);

    public:

    BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test(ptr_type(0)))));
};

template<class String, class Char>
class is_std_string_of
{
    typedef
        typename boost::remove_const<
            typename boost::remove_pointer<
                typename boost::remove_reference<String>::type
            >::type
        >::type* ptr_type;

    static no_type  test (...);
    template<class T, class A> static yes_type test (std::basic_string<Char,T,A>*);

    public:

    BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test(ptr_type(0)))));
};

template<class String>
struct is_c_string
{
    BOOST_STATIC_CONSTANT(bool, value = (is_c_string_of<String, char>::value || is_c_string_of<String, wchar_t>::value));
};

template<class String>
struct is_std_string
{
    BOOST_STATIC_CONSTANT(bool, value = (is_std_string_of<String, char>::value || is_std_string_of<String, wchar_t>::value));
};

template<class String, class Char>
struct is_string_of
{
    BOOST_STATIC_CONSTANT(bool, value = (is_c_string_of<String, Char>::value || is_std_string_of<String, Char>::value));
};

template<class T>
struct is_char_string
{
    BOOST_STATIC_CONSTANT(bool, value = (is_string_of<T, char>::value));
};

template<class T>
struct is_wide_string
{
    BOOST_STATIC_CONSTANT(bool, value = (is_string_of<T, wchar_t>::value));
};

template<class T>
struct is_any_string
{
    BOOST_STATIC_CONSTANT(bool, value = (is_char_string<T>::value || is_wide_string<T>::value));
};

template<class Type, class Enable =void>
struct corrected
{
    typedef Type type;
};

template<class String>
struct corrected<String, typename boost::enable_if<is_any_string<String> >::type>
{
    typedef typename boost::range_value<String>::type char_type;
    typedef std::basic_string<char_type>                   type;
};

template<class Type, class Enable =void>
struct string_range
{
    typedef Type type;
};

template<class String>
struct string_range<String, typename boost::enable_if<is_c_string_of<String, char> >::type>
{
    typedef typename boost::range_value<String>::type char_type;
    typedef char_type const*                           iterator;

    static iterator       begin (String const& s) { return s; }
//  static iterator         end (String const& s) { return s + strlen(s); }
    static std::streamsize size (String const& s) { return std::streamsize(strlen(s)); }
};

template<class String>
struct string_range<String, typename boost::enable_if<is_c_string_of<String, wchar_t> >::type>
{
    typedef typename boost::range_value<String>::type char_type;
    typedef char_type const*                           iterator;

    static iterator       begin (String const& s) { return s; }
//  static iterator         end (String const& s) { return s + strlen(s); }
    static std::streamsize size (String const& s) { return std::streamsize(wcslen(s)); }
};

template<class String>
struct string_range<String, typename boost::enable_if<is_std_string<String> >::type>
{
    typedef typename boost::range_value<String>::type char_type;
    typedef char_type const*                  iterator;

    static iterator       begin (String const& s) { return &*s.begin(); }
//  static iterator         end (String const& s) { return s.end(); }
    static std::streamsize size (String const& s) { return std::streamsize(s.size()); }
};

}}

#endif // BOOST_CONVERT_DETAIL_STRING_SFINAE_HPP
