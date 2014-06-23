#include "./test.hpp"
#include "./timer.hpp"
#include <boost/detail/lightweight_test.hpp>

double
test::cnv::timer::value() const
{
    struct tms tms_end;
    double const   end = times(&tms_end);
    int const  use_sum = (sum_ % 2) ? 0 : (sum_ % 2); BOOST_TEST(use_sum == 0);
    double     use_beg = tms_beg_.tms_utime + tms_beg_.tms_stime;
    double     use_end = tms_end .tms_utime + tms_end .tms_stime;

    return (use_end - use_beg) / sysconf(_SC_CLK_TCK) + use_sum;
}
