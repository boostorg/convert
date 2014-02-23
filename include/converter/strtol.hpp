// Boost.Convert library
//
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRTOL_CONVERTER_HPP
#define BOOST_CONVERT_STRTOL_CONVERTER_HPP

#include "../details/string_sfinae.hpp"
#include <stdlib.h>

namespace boost 
{
    struct strtol_converter;
}

struct boost::strtol_converter
{
    typedef boost::strtol_converter this_type;

    strtol_converter()
    :
        base_(10)
    {}
	
	bool operator()(std::string const& v, int&               r) const { return operator()(v.c_str(), r); }
    bool operator()(std::string const& v, long int&          r) const { return operator()(v.c_str(), r); }
    bool operator()(std::string const& v, unsigned long int& r) const { return operator()(v.c_str(), r); }
    bool operator()(std::string const& v, double&            r) const { return operator()(v.c_str(), r); }

    bool operator()(char const* value_in, double& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        
        result_out = strtod(value_in, &cnv_end);

        return cnv_end == str_end;
    }
    bool operator()(char const* value_in, int& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        long int     result = strtol(value_in, &cnv_end, base_);
        bool        success = INT_MIN <= result && result <= INT_MAX && cnv_end == str_end;

        return success ? (result_out = int(result), success) : success;
    }
    bool operator()(char const* value_in, long int& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        
        result_out = strtol(value_in, &cnv_end, base_);

        return result_out != LONG_MIN && result_out != LONG_MAX && cnv_end == str_end;
    }
    bool operator()(char const* value_in, unsigned long int& result_out) const
    {
        char const* str_end = value_in + strlen(value_in);
        char*       cnv_end = 0;
        
        result_out = strtoul(value_in, &cnv_end, base_);

        return result_out != ULONG_MAX && cnv_end == str_end;
    }

    this_type const&
    operator()(parameter::aux::tag<conversion::parameter::type::base, conversion::base::type const>::type const& arg) const
    {
        boost::conversion::base::type base = arg[conversion::parameter::base];
        
        base_ = base == conversion::base::dec ? 10
              : base == conversion::base::hex ? 16
              : base == conversion::base::oct ? 8 : 0;

        return *this;
    }
    
    private:

    int mutable base_;
};

#endif // BOOST_CONVERT_STRTOL_CONVERTER_HPP
