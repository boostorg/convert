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

namespace boost { namespace cnv {

    template<typename Converter, typename TypeIn, typename TypeOut, class Enable =void> class is_converter;

    template<typename Converter, typename TypeIn, typename TypeOut>
    class is_converter<Converter, TypeIn, TypeOut, typename boost::enable_if<boost::is_class<Converter>, void>::type>
    {
        typedef ::boost::type_traits::yes_type yes_type;
        typedef ::boost::type_traits:: no_type  no_type;
        typedef Converter*                 cnv_ptr_type;

        typedef void (Converter::*func_type)(TypeIn const&, boost::optional<TypeOut>&);

        static no_type  test_ (...);
        static yes_type test_ (func_type);
//      static yes_type test_ (signature_type);

        public:

        BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test_(&Converter::operator()))));
    };

    template<typename Converter, typename TypeIn, typename TypeOut>
    class is_converter<Converter, TypeIn, TypeOut, typename boost::enable_if<boost::function_types::is_function_pointer<Converter>, void>::type>
    {
        typedef ::boost::type_traits::yes_type yes_type;
        typedef ::boost::type_traits:: no_type  no_type;
        typedef Converter                      cnv_type;

        typedef void (*func_type)(TypeIn const&, boost::optional<TypeOut>&);

        static no_type  test_ (...);
        static yes_type test_ (func_type);

        public:

        BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test_(cnv_type(0)))));
    };
}}

#endif // BOOST_CONVERT_CHECKS_HPP

