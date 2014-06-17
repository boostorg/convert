// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_DETAIL_ADAPTER_HPP
#define BOOST_CONVERT_DETAIL_ADAPTER_HPP

#include <boost/convert/detail/forward.hpp>

namespace boost { namespace cnv
{
    template<typename TypeOut, typename Converter>
    struct adapter
    {
        typedef adapter this_type;

        adapter(Converter const& cnv) : converter_(&cnv) {}

        template<typename FallbackType>
        this_type&
        value_or(FallbackType const& fallback)
        {
            return (fallback_ = fallback, *this);
        }

        template<class TypeIn> TypeOut operator()(TypeIn const& value_in)
        {
            boost::optional<TypeOut> result;
            (*converter_)(value_in, result);
            return fallback_ ? result.value_or(*fallback_) : result.value();
        }

        protected:

        Converter const*        converter_;
        boost::optional<TypeOut> fallback_;
    };
}}

#endif // BOOST_CONVERT_DETAIL_ADAPTER_HPP
