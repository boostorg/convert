// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_DETAIL_ALGORITHM_HELPER_HPP
#define BOOST_CONVERT_DETAIL_ALGORITHM_HELPER_HPP

#include <boost/convert/forward.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost { namespace conversion
{
    template<typename TypeOut, typename Converter>
    struct algorithm_helper
    {
        algorithm_helper(Converter const& cnv) : converter_(&cnv) {}

        template<typename FallbackType>
        algorithm_helper_with_fallback<TypeOut, Converter>
        value_or(FallbackType const&);

        template<class TypeIn> TypeOut operator()(TypeIn const& value_in)
        {
            TypeOut result = boost::make_default<TypeOut>();
            bool      good = (*converter_)(value_in, result);

            if (!good)
                BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));

            return result;
        }

        protected:

        Converter const* converter_;
    };

    template<typename TypeOut, typename Converter>
    struct algorithm_helper_with_fallback
    :
        public algorithm_helper<TypeOut, Converter>
    {
        typedef algorithm_helper_with_fallback       this_type;
        typedef algorithm_helper<TypeOut, Converter> base_type;

        algorithm_helper_with_fallback(base_type const& ah, TypeOut const& fallback)
        :
            base_type   (ah),
            fallback_   (fallback)
        {}

        template<class TypeIn> TypeOut operator()(TypeIn const& value_in)
        {
            TypeOut result = boost::make_default<TypeOut>();
            bool      good = (*base_type::converter_)(value_in, result);

            return good ? result : fallback_;
        }

        TypeOut fallback_;
    };
}}

template<typename TypeOut, typename Converter>
template<typename FallbackType>
typename boost::conversion::algorithm_helper_with_fallback<TypeOut, Converter>
boost::conversion::algorithm_helper<TypeOut, Converter>::value_or(FallbackType const& fallback)
{
    return algorithm_helper_with_fallback<TypeOut, Converter>(*this, fallback);
}

#endif // BOOST_CONVERT_DETAIL_ALGORITHM_HELPER_HPP