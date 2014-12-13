#ifndef BOOST_CONVERT_DETAIL_RANGE_HPP
#define BOOST_CONVERT_DETAIL_RANGE_HPP

#include <boost/convert/detail/has_member.hpp>
#include <boost/convert/detail/char.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range/iterator.hpp>

namespace boost { namespace cnv
{
    namespace detail
    {
        template<typename T, bool is_class> struct is_range : mpl::false_ {};

//        template<typename T> struct is_range<T*, false>
//        {
//            static bool const value = true;
//        };
//        template <typename T, std::size_t N> struct is_range<T [N], false>
//        {
//            static bool const value = true;
//        };
        template<typename T> struct is_range<T, /*is_class=*/true>
        {
            BOOST_DECLARE_HAS_MEMBER(has_begin, begin);
            BOOST_DECLARE_HAS_MEMBER(  has_end, end);

            static bool const value = has_begin<T>::value && has_end<T>::value;
        };
    }
    template<typename T> struct is_range : detail::is_range<typename remove_const<T>::type, boost::is_class<T>::value> {};
    template<typename T, typename enable =void> struct range;

    template<typename T>
    struct range<T, typename enable_if_c<is_class<T>::value && is_range<T>::value>::type>
    {
        typedef typename boost::range_iterator<T>::type             iterator;
        typedef typename boost::range_iterator<T const>::type const_iterator;
        typedef typename boost::range_iterator<T>::type          sentry_type;
        typedef typename boost::iterator_value<iterator>::type    value_type;

        range (T& r) : begin_(r.begin()), end_(r.end()) {}
        range (iterator b, iterator e) : begin_(b), end_(e) {}

        iterator       begin () { return begin_; }
        iterator         end () { return   end_; }
        const_iterator begin () const { return begin_; }
        const_iterator   end () const { return   end_; }
        sentry_type   sentry () const { return   end_; }
        void      operator++ () { ++begin_; }
        void      operator-- () { --end_; }

        private:

        iterator begin_;
        iterator   end_;
    };

    template<typename T>
    struct range<T*, typename enable_if<cnv::is_char<T>, void>::type>
    {
        typedef typename remove_const<T>::type value_type;
        typedef T*                               iterator;
        typedef value_type const*          const_iterator;

        struct sentry_type
        {
            friend bool operator!=(iterator it, sentry_type const&) { return !!*it; }
        };

        range (T* b, T* e =0) : begin_(b), end_(e) {}

        iterator       begin ()       { return begin_; }
        iterator         end ()       { return end_ ? end_ : (end_ = begin_ + size()); }
        const_iterator begin () const { return begin_; }
        const_iterator   end () const { return end_ ? end_ : (end_ = begin_ + size()); }
        sentry_type   sentry () const { return sentry_type(); }
        std::size_t     size () const { return std::char_traits<value_type>::length(begin_); }
        void      operator++ () { ++begin_; }
        void      operator-- () { --end_; }

        private:

        iterator       begin_;
        mutable iterator end_; // Calculated when requested.
    };
    template<typename T>
    struct range<T* const, void> : public range<T*>
    {
        range (T* b, T* e =0) : range<T*>(b, e) {}
    };
    template <typename T, std::size_t N>
    struct range<T [N], void> : public range<T*>
    {
        range (T* b, T* e =0) : range<T*>(b, e) {}
    };
}}

#endif // BOOST_CONVERT_DETAIL_RANGE_HPP
