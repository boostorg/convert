#ifndef BOOST_CONVERT_DETAIL_IS_STRING_HPP
#define BOOST_CONVERT_DETAIL_IS_STRING_HPP

#include <boost/convert/detail/range.hpp>

namespace boost { namespace cnv
{
    namespace detail
    {
        template<typename T, bool is_range =false> struct is_string : mpl::false_ {};

        template<typename T> struct is_string<T*>
        {
            static bool const value = cnv::is_char<T>::value;
        };
        template <typename T, std::size_t N> struct is_string<T [N]>
        {
            static bool const value = cnv::is_char<T>::value;
        };
        template<typename T> struct is_string<T, /*is_range=*/true>
        {
            static bool const value = cnv::is_char<typename cnv::range<T>::char_type>::value;
        };
    }
    template<typename T> struct is_string : detail::is_string<
        typename remove_const<T>::type,
        detail::is_range<T, is_class<T>::value>::value> {};
}}

#endif // BOOST_CONVERT_DETAIL_IS_STRING_HPP
