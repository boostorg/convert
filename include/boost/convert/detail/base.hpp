// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_CONVERTER_BASE_HPP
#define BOOST_CONVERT_CONVERTER_BASE_HPP

#include <boost/convert/parameters.hpp>

namespace boost { namespace cnv { namespace detail
{
    using boost::parameter::aux::tag;
    namespace ARG = boost::cnv::parameter::type;

    template<typename> struct cnvbase;
}}}

template<typename derived>
struct boost::cnv::detail::cnvbase
{
    typedef cnvbase this_type;

    cnvbase()
    :
        base_       (10),
        precision_  (0),
        uppercase_  (false),
        width_      (0),
        fill_       (' '),
        adjustment_ (boost::cnv::adjustment::right)
    {}

    derived&
    operator()(tag<ARG::base, boost::cnv::base::type const>::type const& arg)
    {
        base_ = arg[cnv::parameter::base]; return dncast();
    }
    derived&
    operator()(tag<ARG::adjustment, boost::cnv::adjustment::type const>::type const& arg)
    {
        adjustment_ = arg[cnv::parameter::adjustment]; return dncast();
    }
    derived& operator()(tag<ARG::precision, int const>::type const& arg) { precision_ = arg[cnv::parameter::precision]; return dncast(); }
    derived& operator()(tag<ARG::precision,       int>::type const& arg) { precision_ = arg[cnv::parameter::precision]; return dncast(); }

    derived&
    operator()(tag<ARG::uppercase, bool const>::type const& arg)
    {
        uppercase_ = arg[cnv::parameter::uppercase]; return dncast();
    }
    derived&
    operator()(tag<ARG::width, int const>::type const& arg)
    {
        width_ = arg[cnv::parameter::width]; return dncast();
    }
    derived&
    operator()(tag<ARG::fill, char const>::type const& arg)
    {
        fill_ = arg[cnv::parameter::fill]; return dncast();
    }

    protected:

    derived const& dncast () const { return *static_cast<derived const*>(this); }
    derived&       dncast ()       { return *static_cast<derived*>(this); }

    int                                base_;
    int                           precision_;
    bool                          uppercase_;
    int                               width_;
    int                                fill_;
    boost::cnv::adjustment::type adjustment_;
};

#define CONVERTER_PARAM_FUNC(PARAM_NAME, PARAM_TYPE)    \
    this_type&                                          \
    operator()(boost::parameter::aux::tag<boost::cnv::parameter::type::PARAM_NAME, PARAM_TYPE>::type const& arg)

#endif // BOOST_CONVERT_CONVERTER_BASE_HPP
