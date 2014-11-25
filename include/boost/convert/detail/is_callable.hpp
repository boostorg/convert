// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_IS_CALLABLE_HPP
#define BOOST_CONVERT_IS_CALLABLE_HPP

#include <boost/convert/detail/has_member.hpp>

namespace boost { namespace cnv
{
    namespace detail
    {
        struct if_void_return {};

        // Overloaded comma operator.
        template <typename U> U const& operator, (U const&, if_void_return);
        template <typename U> U&       operator, (U&,       if_void_return);

    }
}}

#define BOOST_DECLARE_IS_CALLABLE(__trait_name__, __member_name__) \
                                                                                       \
template <typename class_type, typename func_signature>
class __trait_name__
{
    typedef ::boost::type_traits::yes_type yes_type;
    typedef ::boost::type_traits:: no_type  no_type;

    template <typename src, typename dst> struct match_const { typedef dst type; };
    template <typename src, typename dst> struct match_const<src const, dst> { typedef dst const type; };

    BOOST_DECLARE_HAS_MEMBER(has_name, __member_name__);

    struct mixin : public class_type
    {
        using class_type::__member_name__;
        no_type __member_name__(...) const;
    };

    typedef typename match_const<class_type, mixin>::type* mixin_ptr;

    template<typename T, typename return_type>
    struct return_type_check
    {
        // Overloads in case return_type has template constructor
        static no_type  test (no_type);
        static no_type  test (boost::cnv::detail::if_void_return<class_type>);
        static no_type  test (...);
        static yes_type test (return_type);
    };

    template<typename T>
    struct return_type_check<T, void>
    {
        static yes_type test (...);
        static no_type  test (no_type);
    };

    template <bool has_name, typename F> struct check { static const bool value = false; };

    template <typename Arg1, typename Arg2, typename R>
    struct check<true, R (Arg1, Arg2)>
    {
        typedef typename boost::decay<Arg1>::type* arg1_ptr;
        typedef typename boost::decay<Arg2>::type* arg2_ptr;

        static const bool value = sizeof(yes_type)
                               == sizeof(return_type_check<class_type, R>::test(
                                  // Applying comma operator
                                  (mixin_ptr(0)->__member_name__(*arg1_ptr(0), *arg2_ptr(0)),
                                  boost::cnv::detail::if_void_return())));
    };

    public:

    // Check the existence of the operator() (with has_funop) first, then the signature.
    static bool const value = check<has_funop<class_type>::value, func_signature>::value;
};

#endif // BOOST_CONVERT_IS_CALLABLE_HPP
