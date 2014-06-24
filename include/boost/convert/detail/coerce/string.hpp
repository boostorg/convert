//           Copyright Jeroen Habraken 2011 - 2012.
//
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_STRING_HPP)
#define BOOST_COERCE_STRING_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

#include <boost/convert/detail/coerce/char.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <cstddef>  // std::size_t
#include <string>
#include <vector>

namespace boost { namespace coerce { namespace traits {

    template <typename T>
    struct string_traits_impl;

    template <typename T>
    struct string_traits_impl<T *> {
        typedef T const * const_iterator;

        static inline const_iterator
        begin(T * const value) {
            return value;
        }

        static inline const_iterator
        end(T * const value) {
            return value + std::char_traits<
                               typename remove_const<T>::type
                           >::length(value);
        }
    };

    template <typename T, std::size_t N>
    struct string_traits_impl<T [N]> {
        typedef T const * const_iterator;

        static inline const_iterator
        begin(T const(& value)[N]) {
            return value;
        }

        static inline const_iterator
        end(T const(& value)[N]) {
            return value + (value[N - 1] == 0 ? N - 1 : N);
        }
    };

    template <typename T, typename Traits, typename Allocator>
    struct string_traits_impl<std::basic_string<T, Traits, Allocator> > {
        typedef std::basic_string<T, Traits, Allocator> type;

        typedef typename type::const_iterator const_iterator;

        static inline const_iterator
        begin(type const & value) {
            return value.begin();
        }

        static inline const_iterator
        end(type const & value) {
            return value.end();
        }

        typedef std::back_insert_iterator<type> back_insert_iterator;

        static inline back_insert_iterator
        back_inserter(type & value) {
            return std::back_inserter(value);
        }
    };

    template <typename T>
    struct string_traits_impl<boost::iterator_range<T> > {
        typedef boost::iterator_range<T> type;

        typedef typename type::const_iterator const_iterator;

        static inline const_iterator
        begin(type const & value) {
            return value.begin();
        }

        static inline const_iterator
        end(type const & value) {
            return value.end();
        }
    };

    template <typename T, typename Allocator>
    struct string_traits_impl<std::vector<T, Allocator> > {
        typedef std::vector<T, Allocator> type;

        typedef typename type::const_iterator const_iterator;

        static inline const_iterator
        begin(type const & value) {
            return value.begin();
        }

        static inline const_iterator
        end(type const & value) {
            return value.end();
        }

        typedef std::back_insert_iterator<type> back_insert_iterator;

        static inline back_insert_iterator
        back_inserter(type & value) {
            return std::back_inserter(value);
        }
    };

    template <typename T, typename Enable = void>
    struct string_traits
        : string_traits_impl<T> { };

    template <typename T>
    struct is_source_string_impl
        : mpl::false_ { };

    template <typename T>
    struct is_source_string_impl<T *>
        : traits::is_char<T> { };

    template <typename T, std::size_t N>
    struct is_source_string_impl<T [N]>
        : traits::is_char<T> { };

    template <typename T, typename Traits, typename Allocator>
    struct is_source_string_impl<std::basic_string<T, Traits, Allocator> >
        : traits::is_char<T> { };

    template <typename T>
    struct is_source_string_impl<boost::iterator_range<T> >
        : traits::is_char<typename boost::iterator_range<T>::value_type> { };

    template <typename T, typename Allocator>
    struct is_source_string_impl<std::vector<T, Allocator> >
        : traits::is_char<T> { };

    template <typename T, typename Enable = void>
    struct is_source_string
        : is_source_string_impl<T> { };

    template <typename T>
    struct is_target_string_impl
        : mpl::false_ { };

    template <typename T, typename Traits, typename Allocator>
    struct is_target_string_impl<std::basic_string<T, Traits, Allocator> >
        : traits::is_char<T> { };

    template <typename T, typename Allocator>
    struct is_target_string_impl<std::vector<T, Allocator> >
        : traits::is_char<T> { };

    template <typename T, typename Enable = void>
    struct is_target_string
        : is_target_string_impl<T> { };

} } }  // namespace boost::coerce::traits

#endif  // !defined(BOOST_COERCE_STRING_HPP)
