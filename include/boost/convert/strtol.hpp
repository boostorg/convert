// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_STRTOL_CONVERTER_HPP
#define BOOST_CONVERT_STRTOL_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <climits>

namespace boost { namespace cnv
{
    struct strtol;
}}

struct boost::cnv::strtol : public boost::cnv::detail::cnvbase<boost::cnv::strtol>
{
    typedef boost::cnv::strtol                     this_type;
    typedef boost::cnv::detail::cnvbase<this_type> base_type;
    typedef int                                     int_type;
    typedef unsigned int                           uint_type;
    typedef long int                               lint_type;
    typedef unsigned long int                     ulint_type;
    typedef short int                              sint_type;
    typedef unsigned short int                    usint_type;
    typedef       float                             flt_type;
    typedef      double                             dbl_type;
    typedef long double                            ldbl_type;

    using base_type::operator();

    void operator()(std::string const& v, optional<  int_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, optional< sint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, optional< lint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, optional< uint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, optional<usint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, optional<ulint_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, optional<  flt_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, optional<  dbl_type>& r) const { operator()(v.c_str(), r); }
    void operator()(std::string const& v, optional< ldbl_type>& r) const { operator()(v.c_str(), r); }

    void operator()(char const* v, optional<   int_type>& res) const { strtol_  (v, res); }
    void operator()(char const* v, optional<  sint_type>& res) const { strtol_  (v, res); }
    void operator()(char const* v, optional<  lint_type>& res) const { strtol_  (v, res); }
    void operator()(char const* v, optional<  uint_type>& res) const { strtoul_ (v, res); }
    void operator()(char const* v, optional< usint_type>& res) const { strtoul_ (v, res); }
    void operator()(char const* v, optional< ulint_type>& res) const { strtoul_ (v, res); }
    void operator()(char const* v, optional<   flt_type>& res) const { strtod_  (v, res); }
    void operator()(char const* v, optional<   dbl_type>& res) const { strtod_  (v, res); }
    void operator()(char const* v, optional<  ldbl_type>& res) const { strtod_  (v, res); }

    void operator()( int_type v, optional<std::string>& res) const { res = i_to_str(v); }
    void operator()(lint_type v, optional<std::string>& res) const { res = i_to_str(v); }
    void operator()( dbl_type v, optional<std::string>& res) const { res = d_to_str(v); }

    private:

    template<typename Type> void  strtod_ (char const*, optional<Type>&) const;
    template<typename Type> void  strtol_ (char const*, optional<Type>&) const;
    template<typename Type> void strtoul_ (char const*, optional<Type>&) const;

    template<typename T> std::string i_to_str (T) const;
    /******************/ std::string d_to_str (double) const;
    static bool                  round_up_abs (double, unsigned int, bool const);

    // ULONG_MAX(8 bytes) = 18446744073709551615 (20 characters)
    // double (8 bytes) max is 316 chars
    static int const bufsize = 1024;
};

template<typename Type>
void
boost::cnv::strtol::strtol_(char const* str, optional<Type>& result_out) const
{
    char const*        str_end = str + strlen(str);
    char*              cnv_end = 0;
    long long int const result = ::strtoll(str, &cnv_end, base_);
    bool const            good = result != LLONG_MIN && result != LLONG_MAX && cnv_end == str_end;
    Type const             min = std::numeric_limits<Type>::min();
    Type const             max = std::numeric_limits<Type>::max();

    if (good && min <= result && result <= max)
        result_out = Type(result);
}

template<typename Type>
void
boost::cnv::strtol::strtoul_(char const* str, optional<Type>& result_out) const
{
    char const*                 str_end = str + strlen(str);
    char*                       cnv_end = 0;
    unsigned long long int const result = ::strtoull(str, &cnv_end, base_);
    bool const                     good = result != ULLONG_MAX && cnv_end == str_end;
    Type const                      max = std::numeric_limits<Type>::max();

    if (good && result <= max)
        result_out = Type(result);
}

template<typename Type>
void
boost::cnv::strtol::strtod_(char const* str, optional<Type>& result_out) const
{
    char const*    str_end = str + strlen(str);
    char*          cnv_end = 0;
    ldbl_type const result = ::strtold(str, &cnv_end);
    bool const        good = result != -HUGE_VALL && result != HUGE_VALL && cnv_end == str_end;
    Type const         max = std::numeric_limits<Type>::max();

    if (good && -max <= result && result <= max)
        result_out = Type(result);
}

template<typename Type>
std::string
boost::cnv::strtol::i_to_str(Type value) const
{
    char     buf[bufsize];
    int const is_negative = (value < 0) ? (value = -value, 1) : 0;
    char* const   buf_beg = buf + is_negative;
    char*             end = buf + bufsize / 2;
    char*             beg = end;

    for (; value && buf_beg < beg; value /= base_)
    {
        int digit = value % base_;

        if (digit < 10) *(--beg) = digit += '0';
        else            *(--beg) = digit += 'A' - 10;
    }
    if (beg == end)  *(--beg) = '0';
    if (is_negative) *(--beg) = '-';

    if (width_)
    {
        int const num_fillers = width_ - (end - beg);
        bool const      right = adjustment_ == boost::cnv::adjustment::right;

        if (right) for (int k = 0; k < num_fillers; *(--beg) = fill_, ++k);
        else       for (int k = 0; k < num_fillers; *(end++) = fill_, ++k);
    }
    return std::string(beg, end);
}

inline
bool
boost::cnv::strtol::round_up_abs(double fraction, uint_type precision, bool const is_negative)
{
    // C1. Bring forward the fraction coming right after precision digits.
    //     That is, say, fraction=0.234567, precision=2. Then remainder=0.4567
    // C2. ::round() returns the integral value that is nearest to x,
    //     with halfway cases rounded away from zero. Therefore,
    //          round( 0.4) =  0
    //          round( 0.5) =  1
    //          round( 0.6) =  1
    //          round(-0.4) =  0
    //          round(-0.5) = -1
    //          round(-0.6) = -1
    // C3. INT_MAX(4bytes)=2,147,483,647. So, 10^8 seems appropriate. If not, drop it down to 4.

    int const tens[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

    for (int k = precision / 8; k; --k) fraction *= 100000000; //C3.

    fraction *= tens[precision % 8]; //C1

    double  remainder = (fraction - std::floor(fraction)) * (is_negative ? -1 : 1);
//  bool round_up_abs = int(::rint(remainder));
    bool round_up_abs = int(::round(remainder)); //C2

    return round_up_abs;
}

inline
std::string
boost::cnv::strtol::d_to_str(double value) const
{
    int const  is_negative = (value < 0) ? (value = -value, 1) : 0;
    double        int_part = std::floor(value);
    double        fraction = value - int_part;
    unsigned int precision = precision_;
    bool const    round_up = round_up_abs(fraction, precision, is_negative);
    int const         base = 10;
    char      buf[bufsize];
    char* const    buf_beg = buf + is_negative;
    char*              end = buf + bufsize / 2;
    char*              beg = end;

    for (; 1 <= int_part /*&& buf_beg < beg*/; int_part /= base)
    {
        int digit = int_part - std::floor(int_part / base) * base;

        if (digit < 10) *(--beg) = digit += '0';
        else            *(--beg) = digit += 'A' - 10;
    }
    if (beg == end)  *(--beg) = '0';
    if (is_negative) *(--beg) = '-';
    if (precision)   *(end++) = '.';

    for (; fraction && precision; --precision)
    {
        int digit = fraction *= base;
        fraction -= digit;

        if (digit < 10) *(end++) = digit += '0';
        else            *(end++) = digit += 'A' - 10;
    }
    if (round_up)
    {
        char* prev = end;

        while (beg <= (*--prev == '.' ? --prev : prev))
        {
            if (*prev == '9') *prev = '0';
            else
            {
                ++*prev;
                break;
            }
        }
        if (prev < beg)
            *(beg = prev) = '1';
    }
    for (; precision; --precision)
        *(end++) = '0';

    if (width_)
    {
        int const num_fillers = width_ - (end - beg);
        bool const      right = adjustment_ == boost::cnv::adjustment::right;

        if (right) for (int k = 0; k < num_fillers; *(--beg) = fill_, ++k);
        else       for (int k = 0; k < num_fillers; *(end++) = fill_, ++k);
    }
    return std::string(beg, end);
}

#endif // BOOST_CONVERT_STRTOL_CONVERTER_HPP
