// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_LEXICAL_CAST_BASED_CONVERTER_HPP
#define BOOST_CONVERT_LEXICAL_CAST_BASED_CONVERTER_HPP

#include <boost/lexical_cast.hpp>

namespace boost { namespace cnv
{
    struct lexical_cast;
}}

struct boost::cnv::lexical_cast
{
    template<typename TypeOut, typename TypeIn>
    bool
    operator()(TypeIn const& value_in, TypeOut& result_out) const
    {
        try
        {
            result_out = boost::lexical_cast<TypeOut>(value_in);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
};

#endif // BOOST_CONVERT_LEXICAL_CAST_BASED_CONVERTER_HPP
