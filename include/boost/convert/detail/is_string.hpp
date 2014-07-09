#ifndef BOOST_CONVERT_DETAIL_IS_STRING_HPP
#define BOOST_CONVERT_DETAIL_IS_STRING_HPP

#include <boost/convert/detail/has_memfun_name.hpp>
#include <boost/convert/detail/is_char.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/type_traits/is_same.hpp>
#include <string>

namespace boost { namespace cnv { namespace trait
{
    DECLARE_HAS_MEMFUN_NAME(has_begin, begin);
    DECLARE_HAS_MEMFUN_NAME(  has_end, end);

    template<typename T> struct is_range
    {
        static bool const value = has_begin<T>::value && has_end<T>::value;
    };

    // String is different from the Range as it must be contiguous.

    template <typename T>                struct is_string        : mpl::false_ {};
    template <typename T>                struct is_string<T*>    : trait::is_char<T> { typedef typename remove_const<T>::type char_type; };
    template <typename T, std::size_t N> struct is_string<T [N]> : trait::is_char<T> { typedef typename remove_const<T>::type char_type; };
    template <typename T, typename C, typename A> struct is_string<std::basic_string<T, C, A> > : trait::is_char<T> { typedef T char_type; };

    template <typename T, typename C> struct is_string_of
    {
        typedef typename remove_const<T>::type string_type;
        typedef typename remove_const<C>::type   char_type;

        template<bool is_string, typename S, typename Ch>
        struct is_same : mpl::false_ {};

        template<typename S, typename Ch>
        struct is_same<true, S, Ch>
        {
            static bool const value = boost::is_same<Ch, typename is_string<S>::char_type>::value;
        };

        static bool const value = is_same<is_string<string_type>::value, T, char_type>::value;
    };
}}}

#endif // BOOST_CONVERT_DETAIL_IS_STRING_HPP
