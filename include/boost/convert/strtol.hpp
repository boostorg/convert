// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRTOL_CONVERTER_HPP
#define BOOST_CONVERT_STRTOL_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <stdlib.h>
#include <limits.h>

namespace boost { namespace cnv
{
    struct strtol;
}}

struct boost::cnv::strtol : public boost::cnv::detail::cnvbase<boost::cnv::strtol>
{
    typedef boost::cnv::strtol                     this_type;
    typedef boost::cnv::detail::cnvbase<this_type> base_type;

    using base_type::operator();

    void operator()(std::string const& v, boost::optional<int>&               r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<long int>&          r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<unsigned long int>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, boost::optional<double>&            r) const { operator()(v.c_str(), r); }

    void operator()(char const* value_in, boost::optional<double>& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        double const result = ::strtod(value_in, &cnv_end);

        if (cnv_end == str_end)
            result_out = result;
    }
    void operator()(char const* value_in, boost::optional<int>& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        long int     result = ::strtol(value_in, &cnv_end, base_);

        if (INT_MIN <= result && result <= INT_MAX && cnv_end == str_end)
            result_out = int(result);
    }
    void operator()(char const* value_in, boost::optional<long int>& result_out) const
    {
        char const*   str_end = value_in + strlen(value_in);
        char*         cnv_end = 0;
        long int const result = ::strtol(value_in, &cnv_end, base_);

        if (result != LONG_MIN && result != LONG_MAX && cnv_end == str_end)
            result_out = result;
    }
    void operator()(char const* value_in, boost::optional<unsigned long int>& result_out) const
    {
        char const*            str_end = value_in + strlen(value_in);
        char*                  cnv_end = 0;
        unsigned long int const result = ::strtoul(value_in, &cnv_end, base_);

        if (result != ULONG_MAX && cnv_end == str_end)
            result_out = result;
    }
    void operator()(int value_in, boost::optional<std::string>& result_out) const
    {
        result_out = this_type::ltostr(value_in, base_);
    }
    std::string ltostr(long int num, unsigned int base) const;
};

inline
std::string
boost::cnv::strtol::ltostr(long int num, unsigned int base) const
{ 
    int const     strsz = 256;
    char     str[strsz];
    int const sign_size = (num < 0) ? (num = -num, 1) : 0;
    char* const     beg = str + sign_size;
    char*           end = str + strsz / 2;
    char*           pos = end;
    
    for (; num /*&& beg < pos*/; num /= base)
    { 
        int remainder = num % base;
        
        if (remainder < 10) *(--pos) = remainder += '0';
        else                *(--pos) = remainder += 'A' - 10;
    } 
    if (sign_size) 
        *(--pos) = '-'; 
    
    if (width_)
    {
        int const num_fillers = width_ - (end - pos);
        bool const      right = adjustment_ == boost::cnv::adjustment::right;

        if (right) for (int k = 0; k < num_fillers; *(--pos) = fill_, ++k);
        else       for (int k = 0; k < num_fillers; *(end++) = fill_, ++k);
    }
    return std::string(pos, end); 
} 

#endif // BOOST_CONVERT_STRTOL_CONVERTER_HPP
