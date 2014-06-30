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
    typedef long long int                         llint_type;
    typedef unsigned long long int               ullint_type;
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
    static double             adjust_fraction (double, int precision);
    static int                       get_char (int v) { return (v < 10) ? (v += '0') : (v += 'A' - 10); }

    // ULONG_MAX(8 bytes) = 18446744073709551615 (20(10) or 32(2) characters)
    // double (8 bytes) max is 316 chars
    static int const bufsize = 1024;
};

template<typename Type>
void
boost::cnv::strtol::strtol_(char const* str, optional<Type>& result_out) const
{
    char const*     str_end = str + strlen(str);
    char*           cnv_end = 0;
    llint_type const result = ::strtoll(str, &cnv_end, base_);
    bool const         good = result != LLONG_MIN && result != LLONG_MAX && cnv_end == str_end;
    Type const          min = std::numeric_limits<Type>::min();
    Type const          max = std::numeric_limits<Type>::max();

    if (good && min <= result && result <= max)
        result_out = Type(result);
}

template<typename Type>
void
boost::cnv::strtol::strtoul_(char const* str, optional<Type>& result_out) const
{
    char const*      str_end = str + strlen(str);
    char*            cnv_end = 0;
    ullint_type const result = ::strtoull(str, &cnv_end, base_);
    bool const          good = result != ULLONG_MAX && cnv_end == str_end;
    Type const           max = std::numeric_limits<Type>::max();

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
    // C1. Base=10 optimization improves performance 10%

    char   buf[bufsize];
    bool const negative = (value < 0) ? (value = -value, true) : false;
    char*           end = buf + bufsize / 2;
    char*           beg = end;

    if (base_ == 10) for (; value; *(--beg) = int(value % 10) + '0', value /= 10); //C1
    else             for (; value; *(--beg) = get_char(value % base_), value /= base_);

    if (beg == end) *(--beg) = '0';
    if (negative)   *(--beg) = '-';

    return base_type::format<std::string>(beg, end, buf, buf + bufsize);
}

inline
double
boost::cnv::strtol::adjust_fraction(double fraction, int precision)
{
    // C1. Bring forward the fraction coming right after precision digits.
    //     That is, say, fraction=0.234567, precision=2. Then remainder=0.4567
    // C3. INT_MAX(4bytes)=2,147,483,647. So, 10^8 seems appropriate. If not, drop it down to 4.
    // C4. ::round() returns the integral value that is nearest to x,
    //     with halfway cases rounded away from zero. Therefore,
    //          round( 0.4) =  0
    //          round( 0.5) =  1
    //          round( 0.6) =  1
    //          round(-0.4) =  0
    //          round(-0.5) = -1
    //          round(-0.6) = -1

    int const tens[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

    for (int k = precision / 8; k; --k) fraction *= 100000000; //C3.

    fraction *= tens[precision % 8]; //C1

//  return ::rint(fraction); //C4
    return ::round(fraction); //C4
}

inline
std::string
boost::cnv::strtol::d_to_str(double value) const
{
    char   buf[bufsize];
    char*           end = buf + bufsize / 2;
    char*           beg = end;
    char*          ipos = end - 1;
    bool const negative = (value < 0) ? (value = -value, true) : false;
    double        ipart = std::floor(value);
    double        fpart = adjust_fraction(value - ipart, precision_);
    int       precision = precision_;
    int const      base = 10;

    for (; 1 <= ipart; ipart /= base)
        *(--beg) = get_char(ipart - std::floor(ipart / base) * base);

    if (beg == end) *(--beg) = '0';
    if (precision)  *(end++) = '.';

    for (char* fpos = end += precision; precision; --precision, fpart /= base)
        *(--fpos) = get_char(fpart - std::floor(fpart / base) * base);

    if (1 <= fpart)
    {
        for (; beg <= ipos; --ipos)
            if (*ipos == '9') *ipos = '0';
            else { ++*ipos; break; }

        if (ipos < beg)
            *(beg = ipos) = '1';
    }
    if (negative) *(--beg) = '-';

    return base_type::format<std::string>(beg, end, buf, buf + bufsize);
}

#endif // BOOST_CONVERT_STRTOL_CONVERTER_HPP
