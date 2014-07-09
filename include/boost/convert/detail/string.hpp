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
#include <string>

namespace boost { namespace cnv {

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

template<class String, class Char>
struct is_string_of
{
    BOOST_STATIC_CONSTANT(bool, value = (is_c_string_of<String, Char>::value || is_std_string_of<String, Char>::value));
};

}}

#endif // BOOST_CONVERT_DETAIL_STRING_SFINAE_HPP
