#include "./test.hpp"
#include <boost/noncopyable.hpp>
#include <sys/times.h>

namespace test
{
    struct cnv::timer : boost::noncopyable
    {
        timer (int const& sum) : sum_(sum), beg_(times(&tms_beg_)) {}

        double value() const;

        private:

        int const&     sum_;
        struct tms tms_beg_;
        double const   beg_;
    };
}
