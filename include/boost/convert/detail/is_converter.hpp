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

#undef  BOOST_CONVERTOR_SIGNATURE
#define BOOST_CONVERTOR_SIGNATURE operator()

namespace boost { namespace cnv
{
    template <typename type>
    class has_member
    {
        // C1. This class only checks if type has a member function by the name of BOOST_CONVERTOR_SIGNATURE.
        // C2. The signature of BOOST_CONVERTOR_SIGNATURE does not matter.
        //     Simply introducing BOOST_CONVERTOR_SIGNATURE() in the base class will cause mixin->BOOST_CONVERTOR_SIGNATURE() call
        //     to fail to compile (due to ambiguity) if type::BOOST_CONVERTOR_SIGNATURE(any-signature) exist.
        // C3. &U::BOOST_CONVERTOR_SIGNATURE (a.k.a. &mixin::BOOST_CONVERTOR_SIGNATURE) has the type of func_type
        //     only if type::BOOST_CONVERTOR_SIGNATURE does not exist. If type::BOOST_CONVERTOR_SIGNATURE does exist, it is ambiguous and
        //     "yes_type test (...)" kicks in.

        typedef ::boost::type_traits::yes_type yes_type;
        typedef ::boost::type_traits:: no_type  no_type;

        struct  base { void BOOST_CONVERTOR_SIGNATURE(/*C2*/){} };
        struct mixin : public base, public type {};

        template <typename U, U> struct aux {};

        typedef void (base::*func_type)();
        typedef mixin* mixin_ptr;

        template <typename U> static no_type  test (U*, aux<func_type, &U::BOOST_CONVERTOR_SIGNATURE>* =0); //C3
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

        template <typename src_type, typename dest_type>
        struct clone_constness
        {
            typedef dest_type type;
        };

        template <typename src_type, typename dest_type>
        struct clone_constness<const src_type, dest_type>
        {
            typedef const dest_type type;
        };
    }

    template <typename type, typename func_signature>
    class is_callable
    {
        typedef ::boost::type_traits::yes_type yes_type;
        typedef ::boost::type_traits:: no_type  no_type;

        //добавляем перегруженный метод
        struct mixin : public type
        {
            using type::BOOST_CONVERTOR_SIGNATURE;
            no_type BOOST_CONVERTOR_SIGNATURE(...) const;
        };

        //Учтем возможную константность типа класса
        typedef typename details::clone_constness<type, mixin>::type mixin_type;

        //Проверка результата выражения
        template <typename T, typename due_type>
        struct return_value_check
        {
            //явные перегрузки в случае, если
            //due_type имеет шаблонный конструктор
            static no_type  deduce (no_type);
            static no_type  deduce (details::void_exp_result<type>);
            static no_type  deduce (...);
            static yes_type deduce (due_type);
        };

        //Проверка результата выражения в случае,
        //если не интересует возвращаемый тип метода
        template <typename T>
        struct return_value_check<T, void>
        {
            static yes_type deduce (...);
            static no_type  deduce (no_type);
        };

        template <bool has, typename F>
        struct impl
        {
            static const bool value = false;
        };

        template <typename Arg1, typename Arg2, typename R>
        struct impl<true, R (Arg1, Arg2)>
        {
            typedef typename boost::decay<Arg1>::type arg1_type;
            typedef typename boost::decay<Arg2>::type arg2_type;

            static const bool value =
                sizeof(
                       return_value_check<type, R>::deduce(
                            //трюк с 'operator,'
                            (((mixin_type*)0)->BOOST_CONVERTOR_SIGNATURE(*(arg1_type*)0, *(arg2_type*)0), details::void_exp_result<type>())
                                                    )
                       ) == sizeof(yes_type);

        };
        public:
        //сначала проверяем наличие метода с помощью has_member
        static const bool value = impl<has_member<type>::value, func_signature>::value;
    };

    template<typename Converter, typename TypeIn, typename TypeOut, class Enable =void>
    class is_converter
    {
        public: BOOST_STATIC_CONSTANT(bool, value = false);
    };

    template<typename Converter, typename TypeIn, typename TypeOut>
    class is_converter<
        Converter,
        TypeIn,
        TypeOut,
        typename boost::enable_if_c<boost::is_class<Converter>::value,
        void>::type>
    {
        typedef ::boost::type_traits::yes_type yes_type;
        typedef ::boost::type_traits:: no_type  no_type;

        public:

        BOOST_STATIC_CONSTANT(bool, value = (is_callable<Converter, void (TypeIn const&, boost::optional<TypeOut>&)>::value));
    };

    template<typename Converter, typename TypeIn, typename TypeOut>
    class is_converter<
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

#undef  BOOST_CONVERTOR_SIGNATURE

#endif // BOOST_CONVERT_IS_CONVERTER_HPP

