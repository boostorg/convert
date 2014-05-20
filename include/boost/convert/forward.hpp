// Boost.Convert library
// Copyright (c) 2009-2014 Vladimir Batov.
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_FORWARD_HPP
#define BOOST_CONVERT_FORWARD_HPP

namespace boost
{
	namespace conversion
	{
		template<typename, typename> struct algorithm_helper;
		template<typename, typename> struct algorithm_helper_with_fallback;
		template<typename> struct result;
	}

	template<typename TypeOut, typename TypeIn, typename Converter>
	boost::conversion::result<TypeOut>
	convert(TypeIn const&, Converter const&);

    template<typename T> T allocate_storage() { return T(); }
}

#endif // BOOST_CONVERT_FORWARD_HPP
