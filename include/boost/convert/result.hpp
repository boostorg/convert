// Boost.Convert library
// Copyright (c) 2009-2014 Vladimir Batov.
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_RESULT_HPP
#define BOOST_CONVERT_RESULT_HPP

#include <boost/convert/forward.hpp>
#include <boost/convert/detail/safebool.hpp>
#include <boost/convert/detail/string.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost { namespace conversion
{
	// Used temporarily. To be replaced with std::tr2::optional or improved boost::optional.
	template<typename TypeOut>
	struct result
	{
		typedef result                                           this_type;
		typedef typename convert_detail::corrected<TypeOut>::type out_type; //C1
		typedef boost::safebool<result>                           safebool;

		result (out_type const& v) : value_(v), good_(true) {}

		bool                   operator! () const { return !good_; }
		operator typename safebool::type () const { return safebool(!operator!()); }
	//  operator         out_type const& () const { return this_type::value(); }

		out_type const& value() const
		{
			if (!good_)
				BOOST_THROW_EXCEPTION(std::invalid_argument("boost::convert failed"));

			return value_;
		}

		template<typename FallbackType>
		out_type value_or(FallbackType const& fallback) const
		{
			return good_ ? value_ : fallback;
		}

		private:

		template<typename Type_Out, typename TypeIn, typename Converter>
		friend
		boost::conversion::result<Type_Out>
		boost::convert(TypeIn const& value_in, Converter const& converter);

		this_type& operator()(bool good) { return (good_ = good, *this); }

		out_type value_;
		bool      good_;
	};
}}

#endif // BOOST_CONVERT_RESULT_HPP
