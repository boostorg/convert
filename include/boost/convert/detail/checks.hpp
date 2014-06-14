// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_CHECKS_HPP
#define BOOST_CONVERT_CHECKS_HPP

#include <boost/convert/detail/forward.hpp>
#include <boost/function_types/is_function_pointer.hpp>
#include <boost/type_traits.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/at.hpp>

namespace boost { namespace cnv {

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
        typename boost::enable_if<boost::is_class<Converter>, void>::type>
    {
        typedef ::boost::type_traits::yes_type yes_type;
        typedef ::boost::type_traits:: no_type  no_type;
        typedef Converter*                 cnv_ptr_type;
        typedef TypeIn const* type_in_ptr;
        typedef boost::optional<TypeOut>* type_out_ptr;

        typedef void (Converter::*func_type)(TypeIn const&, boost::optional<TypeOut>&);

        static no_type  test_ (...);
        static yes_type test_ (int);
//      static yes_type test_ (signature_type);

        public:

//      BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test_((Converter(*type_in_ptr(0), *type_out_ptr(0)), int(0))))));
      BOOST_STATIC_CONSTANT(bool, value = true);
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
        typedef TypeIn                                                                 in_type;
        typedef boost::optional<TypeOut>&                                             out_type;
        typedef boost::function_types::parameter_types<Converter>                  func_params;
        typedef typename boost::mpl::at<func_params, boost::mpl::int_<0> >::type  func_in_type;
        typedef typename boost::mpl::at<func_params, boost::mpl::int_<1> >::type func_out_type;

        public:

        BOOST_STATIC_CONSTANT(bool, p1 = (boost::is_convertible<in_type, func_in_type>::value));
        BOOST_STATIC_CONSTANT(bool, p2 = (boost::is_same<out_type, func_out_type>::value));
        BOOST_STATIC_CONSTANT(bool, value = (p1 && p2));
    };
}}

#endif // BOOST_CONVERT_CHECKS_HPP

