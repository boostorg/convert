// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_IS_CONVERTER_HPP
#define BOOST_CONVERT_IS_CONVERTER_HPP

#include <boost/convert/detail/forward.hpp>
#include <boost/convert/detail/has_member.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/ref.hpp>

namespace boost { namespace cnv
{
    typedef ::boost::type_traits::yes_type yes_type;
    typedef ::boost::type_traits:: no_type  no_type;

    // has_funop<T>::value indicates if T has a function-call operator
//    DECLARE_HAS_MEMBER(has_funop, operator());

    // TEMPORARY EXPLICIT SUBSTITUTION OF DECLARE_HAS_MEMBER TO SEE WHY MSVC FAILS
    template <typename __T__>
    class has_funop
    {
        typedef typename ::boost::remove_const<__T__>::type check_type;
        typedef ::boost::type_traits::yes_type                yes_type;
        typedef ::boost::type_traits:: no_type                 no_type;

        struct  base { void operator()(/*C2*/) {}};
        struct mixin : public base, public check_type {};

        template <void (base::*)()> struct aux {};

        typedef mixin* mixin_ptr;

        template <typename U>
        static no_type  test(U*, aux<&U::operator()>* =0);
        static yes_type test(...);

        public:

        BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test(mixin_ptr(0)))));
    };
    namespace details
    {
        template <typename type> class void_exp_result {};

        // Overloaded comma operator.
        template <typename type, typename U> U const& operator, (U const&, void_exp_result<type>);
        template <typename type, typename U> U&       operator, (U&,       void_exp_result<type>);

        template <typename src, typename dst> struct match_const { typedef dst type; };
        template <typename src, typename dst> struct match_const<src const, dst> { typedef dst const type; };
    }

    template <typename class_type, typename func_signature>
    class is_callable
    {
        struct mixin : public class_type
        {
            using class_type::operator();
            no_type operator()(...) const;
        };

        typedef typename details::match_const<class_type, mixin>::type* mixin_ptr;

        template<typename T, typename return_type>
        struct return_type_check
        {
            // Overloads in case return_type has template constructor
            static no_type  test (no_type);
            static no_type  test (details::void_exp_result<class_type>);
            static no_type  test (...);
            static yes_type test (return_type);
        };

        template<typename T>
        struct return_type_check<T, void>
        {
            static yes_type test (...);
            static no_type  test (no_type);
        };

        template <bool has, typename F> struct check { static const bool value = false; };

        template <typename Arg1, typename Arg2, typename R>
        struct check<true, R (Arg1, Arg2)>
        {
            typedef typename boost::decay<Arg1>::type* arg1_ptr;
            typedef typename boost::decay<Arg2>::type* arg2_ptr;

            static const bool value = sizeof(yes_type) == sizeof(
                                      return_type_check<class_type, R>::test(
                                      // Applying comma operator
                                      (mixin_ptr(0)->operator()(*arg1_ptr(0), *arg2_ptr(0)),
                                      details::void_exp_result<class_type>())));
        };

        public:

        // Check the existence of the operator() (with has_funop) first, then the signature.
        static bool const value = check<has_funop<class_type>::value, func_signature>::value;
    };
}}

namespace boost { namespace cnv
{
    template<typename, typename, typename, typename =void>
    struct is_cnv { BOOST_STATIC_CONSTANT(bool, value = false); };

    template<typename Class, typename TypeIn, typename TypeOut>
    struct is_cnv<Class, TypeIn, TypeOut, typename enable_if<is_class<Class>, void>::type>
    {
        typedef typename ::boost::unwrap_reference<Class>::type class_type;
        typedef void signature_type(TypeIn const&, optional<TypeOut>&);

        BOOST_STATIC_CONSTANT(bool, value = (is_callable<class_type, signature_type>::value));
    };

    template<typename Function, typename TypeIn, typename TypeOut>
    struct is_cnv<Function, TypeIn, TypeOut,
        typename enable_if_c<is_function<Function>::value && function_types::function_arity<Function>::value == 2,
        void>::type>
    {
        typedef TypeIn                                              in_type;
        typedef optional<TypeOut>&                                 out_type;
        typedef typename function_traits<Function>::arg1_type  func_in_type;
        typedef typename function_traits<Function>::arg2_type func_out_type;

        BOOST_STATIC_CONSTANT(bool,  in_good = (is_convertible<in_type, func_in_type>::value));
        BOOST_STATIC_CONSTANT(bool, out_good = (is_same<out_type, func_out_type>::value));
        BOOST_STATIC_CONSTANT(bool,    value = (in_good && out_good));
    };
}}

namespace boost { namespace cnv
{
    template <bool has_operator, typename Functor, typename TypeOut>
    struct check_functor { BOOST_STATIC_CONSTANT(bool, value = false); };

    template<typename Func, typename TypeOut, class Enable =void>
    struct is_fun { BOOST_STATIC_CONSTANT(bool, value = false); };

    template <typename Functor, typename TypeOut>
    struct check_functor<true, Functor, TypeOut>
    {
        static yes_type test (TypeOut const&);
        static no_type  test (...);

        static const bool value = sizeof(yes_type) == sizeof(test(((Functor*) 0)->operator()()));
    };

    template<typename Functor, typename TypeOut>
    struct is_fun<Functor, TypeOut,
        typename enable_if_c<is_class<Functor>::value && !is_convertible<Functor, TypeOut>::value, void>::type>
    {
        BOOST_STATIC_CONSTANT(bool, value = (check_functor<has_funop<Functor>::value, Functor, TypeOut>::value));
    };

    template<typename Function, typename TypeOut>
    struct is_fun<Function, TypeOut,
        typename enable_if_c<
            function_types::is_function_pointer<Function>::value &&
            function_types::function_arity<Function>::value == 0 &&
            !is_same<Function, TypeOut>::value,
        void>::type>
    {
        typedef TypeOut                                                   out_type;
        typedef typename function_types::result_type<Function>::type func_out_type;

        BOOST_STATIC_CONSTANT(bool, value = (is_convertible<func_out_type, out_type>::value));
    };
}}

#endif // BOOST_CONVERT_IS_CONVERTER_HPP

