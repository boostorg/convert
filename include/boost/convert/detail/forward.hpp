// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_FORWARD_HPP
#define BOOST_CONVERT_FORWARD_HPP

#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost
{
    namespace cnv
    {
        template<typename, typename> struct algorithm_helper;
        template<typename, typename> struct algorithm_helper_with_fallback;
        template<typename> struct optional;

        static void dothrow()
        {
            BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));
        }
    }

    template<typename TypeOut, typename TypeIn, typename Converter>
    boost::cnv::optional<TypeOut>
    convert(TypeIn const&, Converter const&);
}

#endif // BOOST_CONVERT_FORWARD_HPP
