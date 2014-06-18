// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_IS_CONVERTER_HPP
#define BOOST_CONVERT_IS_CONVERTER_HPP

#include <boost/convert/detail/forward.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/function_types/is_function_pointer.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/result_type.hpp>

#undef  BOOST_CONVERT_FUNCNAME
#define BOOST_CONVERT_FUNCNAME operator()

namespace boost { namespace cnv
{
    typedef ::boost::type_traits::yes_type yes_type;
    typedef ::boost::type_traits:: no_type  no_type;

    template <typename type>
    class has_funcname
    {
        // C1. This class only checks if type has a member function by the name of BOOST_CONVERT_FUNCNAME.
        // C2. The signature of BOOST_CONVERT_FUNCNAME does not matter.
        //     Simply introducing BOOST_CONVERT_FUNCNAME() in the base class will cause
        //     mixin->BOOST_CONVERT_FUNCNAME() call to fail to compile (due to ambiguity)
        //     if type::BOOST_CONVERT_FUNCNAME(any-signature) exists.
        // C3. &U::BOOST_CONVERT_FUNCNAME (a.k.a. &mixin::BOOST_CONVERT_FUNCNAME)
        //     has the type of func_type only if type::BOOST_CONVERT_FUNCNAME does not exist.
        //     If type::BOOST_CONVERT_FUNCNAME does exist, it is ambiguous and "yes_type test (...)" kicks in.

        struct  base { void BOOST_CONVERT_FUNCNAME(/*C2*/){} };
        struct mixin : public base, public type {};

        template <typename U, U> struct aux {};

        typedef void (base::*func_type)();
        typedef mixin* mixin_ptr;

        template <typename U> static no_type  test (U*, aux<func_type, &U::BOOST_CONVERT_FUNCNAME>* =0); //C3
        /*******************/ static yes_type test (...);

        public:

        BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test(mixin_ptr(0)))));
    };

    namespace details
    {
        template <typename type> class void_exp_result {};

        //Перегрузка оператора запятая 'operator,'
        template <typename type, typename U> U const& operator, (U const&, void_exp_result<type>);
        template <typename type, typename U> U&       operator, (U&,       void_exp_result<type>);

        template <typename src, typename dest> struct clone_constness { typedef dest type; };
        template <typename src, typename dest> struct clone_constness<src const, dest> { typedef dest const type; };
    }

    template <typename type, typename func_signature>
    class is_callable
    {
        struct mixin : public type
        {
            using type::BOOST_CONVERT_FUNCNAME;
            no_type BOOST_CONVERT_FUNCNAME(...) const;
        };

        //Учтем возможную константность типа класса
        typedef typename details::clone_constness<type, mixin>::type mixin_type;

        //Проверка результата выражения
        template <typename T, typename due_type>
        struct return_value_check
        {
            //явные перегрузки в случае, если
            //due_type имеет шаблонный конструктор
            static no_type  test (no_type);
            static no_type  test (details::void_exp_result<type>);
            static no_type  test (...);
            static yes_type test (due_type);
        };

        //Проверка результата выражения в случае,
        //если не интересует возвращаемый тип метода
        template <typename T>
        struct return_value_check<T, void>
        {
            static yes_type test (...);
            static no_type  test (no_type);
        };

        template <bool has, typename F> struct check { static const bool value = false; };

        template <typename Arg1, typename Arg2, typename R>
        struct check<true, R (Arg1, Arg2)>
        {
            typedef typename boost::decay<Arg1>::type arg1_type;
            typedef typename boost::decay<Arg2>::type arg2_type;

            static const bool value = sizeof(yes_type) == sizeof(
                                      return_value_check<type, R>::test(
                                      //трюк с 'operator,'
                                      (((mixin_type*) 0)->BOOST_CONVERT_FUNCNAME(*(arg1_type*)0, *(arg2_type*)0), details::void_exp_result<type>())));

        };

        public:

        // Check the existence of the FUNCNAME (with has_funcname) first, then the signature.
        static bool const value = check<has_funcname<type>::value, func_signature>::value;
    };
}}

namespace boost { namespace cnv
{
    template<typename Converter, typename TypeIn, typename TypeOut, class Enable =void>
    class is_cnv
    {
        public: BOOST_STATIC_CONSTANT(bool, value = false);
    };

    template<typename Converter, typename TypeIn, typename TypeOut>
    class is_cnv<
        Converter,
        TypeIn,
        TypeOut,
        typename boost::enable_if_c<boost::is_class<Converter>::value,
        void>::type>
    {
        public:

        BOOST_STATIC_CONSTANT(bool, value = (true));
//        BOOST_STATIC_CONSTANT(bool, value = (is_callable<Converter, void (TypeIn const&, boost::optional<TypeOut>&)>::value));
    };

    template<typename Converter, typename TypeIn, typename TypeOut>
    class is_cnv<
        Converter,
        TypeIn,
        TypeOut,
        typename boost::enable_if_c<boost::is_function<Converter>::value &&
        boost::function_types::function_arity<Converter>::value == 2,
        void>::type>
    {
        typedef TypeIn                                                      in_type;
        typedef boost::optional<TypeOut>&                                  out_type;
        typedef typename boost::function_traits<Converter>::arg1_type  func_in_type;
        typedef typename boost::function_traits<Converter>::arg2_type func_out_type;

        BOOST_STATIC_CONSTANT(bool, p1 = (boost::is_convertible<in_type, func_in_type>::value));
        BOOST_STATIC_CONSTANT(bool, p2 = (boost::is_same<out_type, func_out_type>::value));

        public:

        BOOST_STATIC_CONSTANT(bool, value = (p1 && p2));
    };
}}

namespace boost { namespace cnv
{
    template <bool has_operator, typename Functor, typename TypeOut>
    struct check_functor
    {
        static const bool value = false;
    };
    template <typename Functor, typename TypeOut>
    struct check_functor<true, Functor, TypeOut>
    {
        static yes_type test (TypeOut const&);
        static no_type  test (...);

        static const bool value = sizeof(yes_type) == sizeof(test(((Functor const*) 0)->operator()()));
    };

    template<typename Func, typename TypeOut, class Enable =void>
    struct is_fun
    {
        BOOST_STATIC_CONSTANT(bool, value = false);
    };

    template<typename Functor, typename TypeOut>
    struct is_fun<
        Functor,
        TypeOut,
        typename boost::enable_if_c<
            boost::is_class<Functor>::value &&
            !boost::is_convertible<Functor, TypeOut>::value,
        void>::type>
    {
        BOOST_STATIC_CONSTANT(bool, value = (check_functor<has_funcname<Functor>::value, Functor, TypeOut>::value));
    };

    template<typename Function, typename TypeOut>
    struct is_fun<
        Function,
        TypeOut,
        typename boost::enable_if_c<
            boost::function_types::is_function_pointer<Function>::value &&
            boost::function_types::function_arity<Function>::value == 0 &&
            !boost::is_same<Function, TypeOut>::value,
        void>::type>
    {
        typedef TypeOut                                                          out_type;
        typedef typename boost::function_types::result_type<Function>::type func_out_type;

        BOOST_STATIC_CONSTANT(bool, value = (boost::is_convertible<func_out_type, out_type>::value));
    };
}}

#undef  BOOST_CONVERT_FUNCNAME

#endif // BOOST_CONVERT_IS_CONVERTER_HPP

