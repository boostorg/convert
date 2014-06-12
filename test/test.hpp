#ifndef BOOST_CONVERT_TEST_HPP
#define BOOST_CONVERT_TEST_HPP

#include "../example/example.hpp"
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
#include <boost/convert/sstream.hpp>
#include <boost/convert/printf.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <iomanip>
#include <vector>
#include <list>
#include <iostream>
#include <stdio.h>

//#undef  BOOST_TEST
//#define BOOST_TEST BOOST_ASSERT

struct change
{
    typedef change this_type;

    enum value_type { no, up, dn };

    change(value_type v =no) : value_(v) {}

    friend std::istream& operator>>(std::istream& stream, this_type& chg)
    {
        std::string str; stream >> str;

        /**/ if (str == "up") chg.value_ = up;
        else if (str == "dn") chg.value_ = dn;
        else if (str == "no") chg.value_ = no;
        else stream.setstate(std::ios_base::failbit);

        return stream;
    }
    friend std::ostream& operator<<(std::ostream& stream, this_type const& chg)
    {
        return stream << (chg.value_ == up ? "up" : chg.value_ == dn ? "dn" : "no");
    }

    value_type value() const { return value_; }

    private: value_type value_;
};

struct my_string
{
    typedef my_string        this_type;
    typedef char*             iterator;
    typedef char const* const_iterator;

    my_string()
    {
        BOOST_STATIC_ASSERT(sizeof(this_type) == size_);
        memset(storage_, 0, size_);
    }

    this_type& operator=(std::string const& str)
    {
        BOOST_ASSERT(str.size() < size_);
        strcpy(storage_, str.c_str());
        return *this;
    }

    const_iterator begin () const { return storage_; }
    const_iterator   end () const { return storage_ + strlen(storage_); }

    private:

    static int const size_ = 12;
    char storage_[size_];
};

namespace test
{
    struct cnv
    {
#if defined(_MSC_VER)
        static bool const    is_msc = true;
        static bool const    is_gcc = false;
        static int const num_cycles = 1000000;
#elif defined(__CYGWIN__) 
        static bool const    is_msc = false;
        static bool const    is_gcc = true;
        static int const num_cycles = 1000000;
#elif defined(__GNUC__)
        static bool const    is_msc = false;
        static bool const    is_gcc = true;
        static int const num_cycles = 10000000;
#else
#error "Add here."
#endif

        typedef std::vector<int>            ints;
        typedef std::vector<std::string> strings;

        static ints prepare_ints (int const);
        static void prepare_strs (my_string [], int const);

        static void    is_converter ();
        static void      scratchpad ();
        static void          sfinae ();
        static void      algorithms ();
        static void      encryption ();
        static void       callables ();
        static void lcast_converter ();
        static void         sstream ();
        static void   int_to_string ();
        static void  string_to_bool ();
        static void       user_type ();
        static void   force_in_type ();
        static void     performance (test::cnv::strings const&, test::cnv::ints const&);
        static void          spirit (test::cnv::strings const&);

        template<typename Converter> static void     str_to_int (Converter const&);
        template<typename Converter> static void type_to_string (Converter const&);
        template<typename Converter> static void string_to_type (Converter const&);
        template<typename Converter> static void        invalid (Converter const&);
    };
    struct performance
    {
        template<typename Converter> static double str_to_int (test::cnv::strings const&, Converter const&);
        template<typename Converter> static double int_to_str (test::cnv::ints const&,    Converter const&);
        static int                           spirit_framework ();
    };
}

namespace arg = boost::cnv::parameter;

#endif // BOOST_CONVERT_TEST_HPP
