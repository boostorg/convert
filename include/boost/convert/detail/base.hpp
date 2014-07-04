// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_CONVERTER_BASE_HPP
#define BOOST_CONVERT_CONVERTER_BASE_HPP

#include <boost/convert/parameters.hpp>
#include <cctype>

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
        skipws_     (false),
        precision_  (0),
        uppercase_  (false),
        width_      (0),
        fill_       (' '),
        adjust_     (boost::cnv::adjust::right)
    {}

    derived&
    operator()(tag<ARG::locale, std::locale const>::type const& arg)
    {
        locale_ = arg[cnv::parameter::locale]; return dncast();
    }
    derived&
    operator()(tag<ARG::base, boost::cnv::base::type const>::type const& arg)
    {
        base_ = arg[cnv::parameter::base]; return dncast();
    }
    derived&
    operator()(tag<ARG::adjust, boost::cnv::adjust::type const>::type const& arg)
    {
        adjust_ = arg[cnv::parameter::adjust]; return dncast();
    }
    derived& operator()(tag<ARG::precision, int const>::type const& arg) { precision_ = arg[cnv::parameter::precision]; return dncast(); }
    derived& operator()(tag<ARG::precision,       int>::type const& arg) { precision_ = arg[cnv::parameter::precision]; return dncast(); }

    derived& operator()(tag<ARG::uppercase, bool const>::type const& arg) { uppercase_ = arg[cnv::parameter::uppercase]; return dncast(); }
    derived& operator()(tag<ARG::skipws,    bool const>::type const& arg) { skipws_    = arg[cnv::parameter::skipws]; return dncast(); }

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

    template<typename string_type> string_type format (char*, char*, char const*) const;

    derived const& dncast () const { return *static_cast<derived const*>(this); }
    derived&       dncast ()       { return *static_cast<derived*>(this); }

    // ULONG_MAX(8 bytes) = 18446744073709551615 (20(10) or 32(2) characters)
    // double (8 bytes) max is 316 chars
    static int const        bufsize_ = 1024;
    int                        base_;
    bool                     skipws_;
    int                   precision_;
    bool                  uppercase_;
    int                       width_;
    int                        fill_;
    boost::cnv::adjust::type adjust_;
    std::locale              locale_;
};

template<typename derived_type>
template<typename string_type>
inline
string_type
boost::cnv::detail::cnvbase<derived_type>::format(
    char*          beg,
    char*          end,
    char const* bufbeg) const
{
    // TODO: Need boundary checks.

    if (uppercase_)
    {
        for (char* p = beg; p < end; ++p) *p = toupper(*p);
    }
    if (width_)
    {
        int const num_fillers = width_ - (end - beg);
        int const    num_left = adjust_ == boost::cnv::adjust::left ? 0
                              : adjust_ == boost::cnv::adjust::right ? num_fillers
                              : (num_fillers / 2);
        int const   num_right = num_fillers - num_left;

        for (int k = 0; k <  num_left; *(--beg) = fill_, ++k);
        for (int k = 0; k < num_right; *(end++) = fill_, ++k);
    }
    return string_type(beg, end);
}

#define CONVERTER_PARAM_FUNC(PARAM_NAME, PARAM_TYPE)    \
    this_type&                                          \
    operator()(boost::parameter::aux::tag<boost::cnv::parameter::type::PARAM_NAME, PARAM_TYPE>::type const& arg)

#endif // BOOST_CONVERT_CONVERTER_BASE_HPP
