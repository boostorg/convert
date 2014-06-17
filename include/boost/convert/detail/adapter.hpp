// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_DETAIL_ADAPTER_HPP
#define BOOST_CONVERT_DETAIL_ADAPTER_HPP

#include <boost/convert/detail/forward.hpp>

namespace boost { namespace cnv
{
    template<typename, typename> struct adapter;
    template<typename, typename> struct adapter_with_fallback;

    template<typename TypeOut, typename Converter>
    struct adapter
    {
        adapter(Converter const& cnv) : converter_(&cnv) {}

        template<typename FallbackType>
        adapter_with_fallback<TypeOut, Converter>
        value_or(FallbackType const&);

        template<class TypeIn> TypeOut operator()(TypeIn const& value_in)
        {
            boost::optional<TypeOut> result;
            (*converter_)(value_in, result);
            return result.value();
        }

        protected:

        Converter const* converter_;
    };

    template<typename TypeOut, typename Converter>
    struct adapter_with_fallback
    :
        public adapter<TypeOut, Converter>
    {
        typedef adapter_with_fallback       this_type;
        typedef adapter<TypeOut, Converter> base_type;

        adapter_with_fallback(base_type const& ah, TypeOut const& fallback)
        :
            base_type   (ah),
            fallback_   (fallback)
        {}

        template<class TypeIn> TypeOut operator()(TypeIn const& value_in)
        {
            boost::optional<TypeOut> result;
            (*base_type::converter_)(value_in, result);
            return result.value_or(fallback_);
        }

        TypeOut fallback_;
    };
}}

template<typename TypeOut, typename Converter>
template<typename FallbackType>
typename boost::cnv::adapter_with_fallback<TypeOut, Converter>
boost::cnv::adapter<TypeOut, Converter>::value_or(FallbackType const& fallback)
{
    return adapter_with_fallback<TypeOut, Converter>(*this, fallback);
}

#endif // BOOST_CONVERT_DETAIL_ADAPTER_HPP
