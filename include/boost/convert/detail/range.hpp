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

        template<typename T> struct is_range<T*, false>
        {
            static bool const value = true;
        };
        template <typename T, std::size_t N> struct is_range<T [N], false>
        {
            static bool const value = true;
        };
        template<typename T> struct is_range<T, /*is_class=*/true>
        {
            DECLARE_HAS_MEMBER(has_begin, begin);
            DECLARE_HAS_MEMBER(  has_end, end);

            static bool const value = has_begin<T>::value && has_end<T>::value;
        };
    }
    template<typename T> struct is_range : detail::is_range<typename remove_const<T>::type, boost::is_class<T>::value> {};
    template<typename T, typename enable =void> struct range;

    template<typename T>
    struct range<T, typename enable_if_c<is_class<T>::value && is_range<T>::value>::type>
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

        range (T* b, T* e =0) : begin_(b), end_(e) {}

        iterator&       begin ()       { return begin_; }
        iterator&         end ()       { return end_ ? end_ : (end_ = begin_ + size()); }
        iterator const& begin () const { return begin_; }
        iterator const&   end () const { return end_ ? end_ : (end_ = begin_ + size()); }
        sentry_type    sentry () const { return sentry_type(); }
        std::size_t      size () const { return std::char_traits<T>::length(begin_); }

        private:

        iterator       begin_;
        mutable iterator end_;
    };
    template<typename T>
    struct range<T* const, typename enable_if<cnv::is_char<T>, void>::type> : public range<T*>
    {
        typedef range<T* const>             this_type;
        typedef range<T*>                   base_type;
        typedef typename base_type::iterator iterator;

        range (T* b, T* e =0) : base_type(b, e) {}
    };
    template <typename T, std::size_t N>
    struct range<T [N], typename enable_if<cnv::is_char<T>, void>::type>
    {
        // TODO. Should we take advantage of knowing the actual size?

        typedef typename remove_const<T>::type char_type;
        typedef T* iterator;

        struct sentry_type
        {
            friend bool operator!=(iterator it, sentry_type const&) { return !!*it; }
        };

        range (T* b, T* e =0) : begin_(b), end_(e) {}

        iterator&       begin ()       { return begin_; }
        iterator&         end ()       { return end_ ? end_ : (end_ = begin_ + size()); }
        iterator const& begin () const { return begin_; }
        iterator const&   end () const { return end_ ? end_ : (end_ = begin_ + size()); }
        sentry_type    sentry () const { return sentry_type(); }
        std::size_t      size () const { return std::char_traits<T>::length(begin_); }

        private:

        iterator       begin_;
        mutable iterator end_;
    };
}}

#endif // BOOST_CONVERT_DETAIL_RANGE_HPP
