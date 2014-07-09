#ifndef BOOST_CONVERT_DETAIL_IS_CHAR_HPP
#define BOOST_CONVERT_DETAIL_IS_CHAR_HPP

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace cnv { namespace trait
{
    namespace detail
    {
        template<typename T> struct is_char          : mpl::false_ {};
        template<>           struct is_char<char>    : mpl:: true_ {};
        template<>           struct is_char<wchar_t> : mpl:: true_ {};
    }
    template <typename T> struct is_char : detail::is_char<typename remove_const<T>::type> {};

    inline std::size_t size(char    const* s) { return ::strlen(s); }
    inline std::size_t size(wchar_t const* s) { return ::wcslen(s); }
}}}

#endif // BOOST_CONVERT_DETAIL_IS_CHAR_HPP
