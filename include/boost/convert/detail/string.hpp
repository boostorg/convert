/// \file boost/convert/string_sfinae.hpp
/// Defines various string-related type checks
//
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

namespace boost { namespace convert_detail {

namespace sfinae {

using ::boost::type_traits::yes_type;
using ::boost::type_traits:: no_type;

template<typename T>
class is_range // not completed and not used
{
    // Does not work for derived classes (like derived from std::string) as the U::begin signature
    // is different (of the base class).

    template<typename U, typename U::const_iterator (U::*)() const> struct exists {};
    template<typename U> static no_type  test (...);
    template<typename C> static yes_type test (exists<C, &C::begin>*);

    public:

    BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test<T>(0))));
};

template<class String, class Char>
class is_any_string
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
    template<class T, class A> static yes_type test (std::basic_string<Char,T,A>*);

    // Support for Char-based range-like containers (std::list<char>, etc) have been
    // disabled as the attempt seems overzealous and in addition some unexpected
    // types seem to jump in (like boost::regex) which happen to define container-like
    // properties.
//     template<class C> static yes_type test (
//         C/*ontainer*/ const*,
//         typename boost::enable_if<boost::is_same<typename C::value_type, Char> >::type* =0,
//         typename C::const_iterator* =0);

    public:

    BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test(ptr_type(0)))));
};

template<class String, class Char>
class is_std_string
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

} // end of sfinae

/// Check if the provided type is a char-based range
template<class T>
struct is_char_string
{
    BOOST_STATIC_CONSTANT(bool, value = (sfinae::is_any_string<T, char>::value));
};

/// Check if the provided type is a wide-char-based range
template<class T>
struct is_wide_string
{
    BOOST_STATIC_CONSTANT(bool, value = (sfinae::is_any_string<T, wchar_t>::value));
};

/// Check if both provided types are char-based or wide-char-based ranges
template<class T, class Y>
struct is_same_string
{
    BOOST_STATIC_CONSTANT(
        bool,
        value = ((is_char_string<T>::value && is_char_string<Y>::value) ||
                 (is_wide_string<T>::value && is_wide_string<Y>::value)));
};

template<class T>
struct is_any_string
{
    BOOST_STATIC_CONSTANT(
        bool,
        value = (is_char_string<T>::value || is_wide_string<T>::value));
};

/// Check if the provided type is std::string, std::wstring (or derived)
template<class T>
struct is_std_string
{
    BOOST_STATIC_CONSTANT(
        bool,
        value = (sfinae::is_std_string<T, char>::value ||
                 sfinae::is_std_string<T, wchar_t>::value));
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

}}

#endif // BOOST_CONVERT_DETAIL_STRING_SFINAE_HPP