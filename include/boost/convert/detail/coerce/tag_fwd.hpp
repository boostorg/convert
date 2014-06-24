//              Copyright Jeroen Habraken 2012.
//
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_COERCE_TAG_FWD_HPP)
#define BOOST_COERCE_TAG_FWD_HPP

#if defined(_MSC_VER) && _MSC_VER >= 1200
    #pragma once
#endif  // defined(_MSC_VER) && _MSC_VER >= 1200

namespace boost { namespace coerce { namespace tag {

    struct none;

    template <unsigned Radix>
    struct base;

    struct bin;

    struct oct;
    
    struct hex;

} } }  // namespace boost::coerce::tag

#endif  // !defined(BOOST_COERCE_TAG_FWD_HPP)
