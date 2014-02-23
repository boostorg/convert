// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_MIXED_CONVERTER_HPP
#define BOOST_CONVERT_MIXED_CONVERTER_HPP

#include "./string_sfinae.hpp"
#include <stdlib.h>

namespace boost 
{
    struct mixed_converter;
}
// 
// fcvt() for double-to-string. http://www.indiabix.com/technical/c/strings/4
//
struct boost::mixed_converter
{
    bool operator()(std::string const& v, double&            r) const { return operator()(v.c_str(), r); }
    bool operator()(std::string const& v, float&             r) const { return operator()(v.c_str(), r); }
    bool operator()(std::string const& v, int&               r) const { return operator()(v.c_str(), r); }
    bool operator()(std::string const& v, long int&          r) const { return operator()(v.c_str(), r); }
    bool operator()(std::string const& v, unsigned long int& r) const { return operator()(v.c_str(), r); }

#if _XOPEN_SOURCE >= 600 || _ISOC99_SOURCE || _POSIX_C_SOURCE >= 200112L

    bool operator()(char const* value_in, double& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        
        result_out = strtod(value_in, &cnv_end);

        return cnv_end == str_end;
    }
    bool operator()(char const* value_in, float& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        
        result_out = strtof(value_in, &cnv_end);

        return cnv_end == str_end;
    }
#endif
    bool operator()(char const* value_in, int& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        long int     result = strtol(value_in, &cnv_end, 0);
        bool        success = INT_MIN <= result && result <= INT_MAX && cnv_end == str_end;

        return success ? (result_out = int(result), success) : success;
    }
    bool operator()(char const* value_in, long int& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        
        result_out = strtol(value_in, &cnv_end, 0);

        return result_out != LONG_MIN && result_out != LONG_MAX && cnv_end == str_end;
    }
    bool operator()(char const* value_in, unsigned long int& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        
        result_out = strtoul(value_in, &cnv_end, 0);

        return result_out != ULONG_MAX && cnv_end == str_end;
    }
};

#endif // BOOST_CONVERT_MIXED_CONVERTER_HPP
