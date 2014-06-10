#include "./example.hpp"
#include <boost/convert.hpp>
#include <boost/convert/sstream.hpp>

using std::string;

static
void
process_failure()
{
}

void
example::getting_serious()
{
    boost::cnv::cstringstream cnv; // stringstream-based char converter
    std::string const         str = "123";
    std::string const        str1 = "123";
    std::string const        str2 = "123";
    std::string const        str3 = "123";
    int const      fallback_value = -1;

    {
        //[getting_serious_example1

        int i2 = boost::convert<int>("not an int", cnv).value_or(-1); // after the call i2 == -1

        if (i2 == -1) process_failure();

        //]
    }
    {
        //[getting_serious_example2

        try
        {
            int i1 = boost::lexical_cast<int>(str); // Throws if the conversion fails
            int i2 = boost::convert<int>(str, cnv).value(); // Throws if the conversion fails
        }
        catch (...)
        {
            process_failure();
        }
        //]
    }
    {
        //[getting_serious_example3

        boost::optional<int> r1 = boost::convert<int>(str1, cnv); // Does not throw on conversion failure
        boost::optional<int> r2 = boost::convert<int>(str2, cnv); // Does not throw on conversion failure

        // ...

        try // Delayed processing of potential exceptions
        {
            int i1 = r1.value(); // Will throw if conversion failed
            int i2 = r2.value(); // Will throw if conversion failed
        }
        catch (...)
        {
            // Handle failed conversion
        }

        // Exceptions are avoided altogether
        int i1 = r1 ? r1.value() : fallback_value;
        int i2 = r2.value_or(fallback_value);
        int i3 = boost::convert<int>(str3, cnv).value_or(fallback_value);
        //]
    }
}
