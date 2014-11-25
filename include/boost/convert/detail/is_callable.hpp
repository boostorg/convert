// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_IS_CALLABLE_HPP
#define BOOST_CONVERT_IS_CALLABLE_HPP

#include <boost/convert/detail/has_member.hpp>

namespace boost { namespace cnv { namespace detail
{
    typedef ::boost::type_traits::yes_type yes_type;
    typedef ::boost::type_traits:: no_type  no_type;

    struct not_found {};
    struct void_return_substitute {};

    // The overloaded comma operator only kicks in for U != void essentially short-circuiting
    // itself ineffective. Otherwise, when U=void, the standard op,() kicks in and returns
    // 'void_return_substitute'.
    template<typename U> U const& operator, (U const&, void_return_substitute);
    template<typename U> U&       operator, (U&,       void_return_substitute);

    template <typename src, typename dst> struct match_const { typedef dst type; };
    template <typename src, typename dst> struct match_const<src const, dst> { typedef dst const type; };

    template<typename T, typename return_type>
    struct redirect
    {
        static no_type  test (...);
        static yes_type test (return_type);
    };

    template<typename T>
    struct redirect<T, void>
    {
        static yes_type test (...);
        static no_type  test (not_found);
    };
}}}

// No-args case needs to be implemented differently and has not been implemented yet.
//        template <typename R>
//        struct check<true, R ()>

#define BOOST_DECLARE_IS_CALLABLE(__class_name__, __member_name__)                          \
                                                                                            \
template <typename class_type, typename func_signature>                                     \
class __class_name__                                                                        \
{                                                                                           \
    BOOST_DECLARE_HAS_MEMBER(has_name, __member_name__);                                    \
                                                                                            \
    struct mixin : public class_type                                                        \
    {                                                                                       \
        using class_type::__member_name__;                                                  \
        boost::cnv::detail::not_found __member_name__(...) const;                           \
    };                                                                                      \
                                                                                            \
    typedef typename boost::cnv::detail::match_const<class_type, mixin>::type* mixin_ptr;   \
                                                                                            \
    template <bool has, typename F> struct check { static const bool value = false; };      \
                                                                                            \
    template <typename Arg1, typename R>                                                    \
    struct check<true, R (Arg1)>                                                            \
    {                                                                                       \
        typedef typename boost::decay<Arg1>::type* a1;                                      \
                                                                                            \
        static const bool value = sizeof(boost::cnv::detail::yes_type)                      \
                               == sizeof(boost::cnv::detail::redirect<class_type, R>::test( \
                                  (mixin_ptr(0)->operator()(*a1(0)),                        \
                                          boost::cnv::detail::void_return_substitute())));  \
        };                                                                                  \
        template <typename Arg1, typename Arg2, typename R>                                 \
        struct check<true, R (Arg1, Arg2)>                                                  \
        {                                                                                   \
            typedef typename boost::decay<Arg1>::type* a1;                                  \
            typedef typename boost::decay<Arg2>::type* a2;                                  \
                                                                                            \
        static const bool value = sizeof(boost::cnv::detail::yes_type)                      \
                               == sizeof(boost::cnv::detail::redirect<class_type, R>::test( \
                                  (mixin_ptr(0)->operator()(*a1(0), *a2(0)),                \
                                          boost::cnv::detail::void_return_substitute())));  \
    };                                                                                      \
                                                                                            \
    public:                                                                                 \
                                                                                            \
    /* Check the existence of __member_name__ first, then the signature. */                 \
    static bool const value = check<has_name<class_type>::value, func_signature>::value;    \
};

#endif // BOOST_CONVERT_IS_CALLABLE_HPP
