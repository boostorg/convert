// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_TEST_HPP
#define BOOST_CONVERT_TEST_HPP

#include <boost/make_default.hpp>
#include <boost/static_assert.hpp>
#include <boost/array.hpp>
#include <string.h>

//[change_declaration
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
//]
//[direction_declaration
struct direction
{
    // Note: the class does NOT have the default constructor.

    enum value_type { up, dn };

    direction(value_type value) : value_(value) {}
    bool operator==(direction const& that) const { return value_ == that.value_; }

    friend std::istream& operator>>(std::istream& stream, direction& dir)
    {
        std::string str; stream >> str;

        /**/ if (str == "up") dir.value_ = up;
        else if (str == "dn") dir.value_ = dn;
        else stream.setstate(std::ios_base::failbit);

        return stream;
    }
    friend std::ostream& operator<<(std::ostream& stream, direction const& dir)
    {
        return stream << (dir.value_ == up ? "up" : "dn");
    }

    private: value_type value_;
};
//]
//[direction_declaration_make_default
namespace boost
{
    template<> inline direction make_default<direction>() { return direction(direction::up); }
}
//]

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

    char const*    c_str () const { return storage_; }
    const_iterator begin () const { return storage_; }
    const_iterator   end () const { return storage_ + strlen(storage_); }

    private:

    static size_t const size_ = 12;
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

        struct timer;

        // C1. 18 = 9 positive + 9 negative numbers with the number of digits from 1 to 9.
        //     Even though INT_MAX(32) = 2147483647, i.e. 10 digits (not to mention long int)
        //     we only test up to 9 digits as Spirit does not handle more than 9.

        typedef boost::array<my_string, 18> strings; //C1
        typedef boost::array<int,       18>    ints;

        static ints const&    get_ints ();
        static strings const& get_strs ();

        static void      is_converter ();
        static void        scratchpad ();
        static void            sfinae ();
        static void        encryption ();
        static void         callables ();
        static void         fallbacks ();
        static void   lcast_converter ();
        static void            stream ();
        static void     int_to_string ();
        static void         user_type ();
        static void     force_in_type ();
        static void       performance ();
        static void            spirit ();

        template<typename Cnv> static void  str_to_int (Cnv const&);
        template<typename Cnv> static void  int_to_str (Cnv const&);
        template<typename Cnv> static void type_to_str (Cnv const&);
        template<typename Cnv> static void str_to_type (Cnv const&);
    };
    struct performance
    {
        template<typename Cnv> static double str_to_int (Cnv const&);
        template<typename Cnv> static double int_to_str (Cnv const&);
        static int                     spirit_framework ();
    };
}

#endif // BOOST_CONVERT_TEST_HPP
