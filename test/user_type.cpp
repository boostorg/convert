// Boost.Convert test and usage example
// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

using std::string;
using std::wstring;

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

namespace boost
{
    template<> inline direction make_default<direction>() { return direction(direction::up); }
}

void
test::user_type()
{
    boost::cnv::cstringstream cnv; // stringstream-based char converter

    direction const        up_dir1 = direction::up;
    direction const        dn_dir1 = direction::dn;
    string const       up_dir0_str = boost::convert<string, direction>(direction::up, cnv).value();
    string const       dn_dir0_str = boost::convert<string, direction>(direction::dn, cnv).value();
    string const       up_dir1_str = boost::convert<string>(up_dir1, cnv).value();
    string const       dn_dir1_str = boost::convert<string>(dn_dir1, cnv).value();
    direction const        up_dir2 = boost::convert<direction>(up_dir1_str, cnv).value();
    direction const        dn_dir2 = boost::convert<direction>(dn_dir1_str, cnv).value();
    direction const        up_dir3 = boost::convert<direction>(up_dir1_str, cnv).value();
    direction const        dn_dir3 = boost::convert<direction>(dn_dir1_str, cnv).value();
    direction const        dn_dir4 = boost::convert<direction>("junk", cnv).value_or(direction::dn);
    cnv::optional<direction> up_dir4 = boost::convert<direction>("junk", cnv);

    BOOST_TEST(up_dir0_str == "up");
    BOOST_TEST(dn_dir0_str == "dn");
    BOOST_TEST(up_dir1_str == "up");
    BOOST_TEST(dn_dir1_str == "dn");
    BOOST_TEST(up_dir2 == up_dir1);
    BOOST_TEST(dn_dir2 == dn_dir1);
    BOOST_TEST(up_dir3 == direction::up);
    BOOST_TEST(dn_dir3 == direction::dn);
    BOOST_TEST(dn_dir4 == direction::dn);
    BOOST_TEST(!up_dir4); // Failed conversion
}
