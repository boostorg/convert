#ifndef BOOST_CONVERT_DETAIL_RANGE_HPP
#define BOOST_CONVERT_DETAIL_RANGE_HPP

#include <boost/convert/detail/has_memfun_name.hpp>
#include <boost/convert/detail/char.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range/iterator.hpp>

namespace boost { namespace cnv
{
    namespace detail
    {
        DECLARE_HAS_MEMFUN_NAME(has_begin, begin);
        DECLARE_HAS_MEMFUN_NAME(  has_end, end);

        template<bool is_class, typename T> struct is_range : mpl::false_ {};

        template<typename T> struct is_range<true, T>
        {
            static bool const value = has_begin<T>::value && has_end<T>::value;
        };
    }

    template<typename T, typename enable =void> struct range;

    template<typename T>
    struct range<T, typename enable_if<detail::is_range<is_class<T>::value, T>, void>::type>
    {
        typedef typename boost::range_iterator<T>::type    iterator;
        typedef typename boost::range_iterator<T>::type sentry_type;
        typedef typename boost::iterator_value<iterator>::type char_type;

        range (T& r) : begin_(r.begin()), end_(r.end()) {}
        range (iterator b, iterator e) : begin_(b), end_(e) {}

        iterator&       begin () { return begin_; }
        iterator&         end () { return   end_; }
        iterator const& begin () const { return begin_; }
        iterator const&   end () const { return   end_; }
        sentry_type    sentry () const { return   end_; }

        private:

        iterator begin_;
        iterator   end_;
    };

    template<typename T>
    struct range<T*, typename enable_if<cnv::is_char<T>, void>::type>
    {
        typedef typename remove_const<T>::type char_type;
        typedef T* iterator;

        struct sentry_type
        {
            friend bool operator!=(iterator it, sentry_type const&) { return !!*it; }
        };

        range (T* r) : begin_(r), end_(0) {}
        range (iterator b, iterator e) : begin_(b), end_(e) {}

        iterator&       begin ()       { return begin_; }
        iterator&         end ()       { return end_ ? end_ : (end_ = begin_ + std::char_traits<T>::length(begin_)); }
        iterator const& begin () const { return begin_; }
        iterator const&   end () const { return end_ ? end_ : (end_ = begin_ + std::char_traits<T>::length(begin_)); }
        sentry_type    sentry () const { return sentry_type(); }

        private:

        iterator       begin_;
        mutable iterator end_;
    };
    template<typename T>
    struct range<T* const, typename enable_if<cnv::is_char<T>, void>::type>
    {
        typedef typename remove_const<T>::type char_type;
        typedef T* iterator;

        struct sentry_type
        {
            friend bool operator!=(iterator it, sentry_type const&) { return !!*it; }
        };

        range (T* r) : begin_(r), end_(0) {}
        range (iterator b, iterator e) : begin_(b), end_(e) {}

        iterator&       begin ()       { return begin_; }
        iterator&         end ()       { return end_ ? end_ : (end_ = begin_ + std::char_traits<T>::length(begin_)); }
        iterator const& begin () const { return begin_; }
        iterator const&   end () const { return end_ ? end_ : (end_ = begin_ + std::char_traits<T>::length(begin_)); }
        sentry_type    sentry () const { return sentry_type(); }

        private:

        iterator       begin_;
        mutable iterator end_;
    };
    template <typename T, std::size_t N>
    struct range<T [N], typename enable_if<cnv::is_char<T>, void>::type>
    {
        typedef typename remove_const<T>::type char_type;
        typedef T* iterator;

        struct sentry_type
        {
            friend bool operator!=(iterator it, sentry_type const&) { return !!*it; }
        };

        range (T* r) : begin_(r), end_(0) {}
        range (iterator b, iterator e) : begin_(b), end_(e) {}

        iterator&       begin ()       { return begin_; }
        iterator&         end ()       { return end_ ? end_ : (end_ = begin_ + std::char_traits<T>::length(begin_)); }
        iterator const& begin () const { return begin_; }
        iterator const&   end () const { return end_ ? end_ : (end_ = begin_ + std::char_traits<T>::length(begin_)); }
        sentry_type    sentry () const { return sentry_type(); }

        private:

        iterator       begin_;
        mutable iterator end_;
    };
}}

#endif // BOOST_CONVERT_DETAIL_RANGE_HPP
