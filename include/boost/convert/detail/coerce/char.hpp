//              Copyright Jeroen Habraken 2011.
//
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_CHAR_HPP)
#define BOOST_COERCE_CHAR_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace boost { namespace coerce { namespace traits {

    template <typename T>
    struct is_char_impl
        : mpl::false_ { };

    template <>
    struct is_char_impl<char>
        : mpl::true_ { };

    template <>
    struct is_char_impl<wchar_t>
        : mpl::true_ { };

    template <typename T, typename Enable = void>
    struct is_char
        : is_char_impl<typename remove_const<T>::type> { };

} } }  // namespace boost::coerce::traits

#endif  // !defined(BOOST_COERCE_CHAR_HPP)
