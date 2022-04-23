include(BoostTest OPTIONAL RESULT_VARIABLE HAVE_BOOST_TEST)

if(HAVE_BOOST_TEST)
    boost_test(TYPE run NAME convert_test_callable SOURCES callable.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_fallbacks SOURCES fallbacks.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_spirit_converter SOURCES spirit_converter.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_stream_converter SOURCES stream_converter.cpp LINK_LIBRARIES Boost::convert Boost::included_unit_test_framework)
    boost_test(TYPE run NAME convert_test_printf_converter SOURCES printf_converter.cpp LINK_LIBRARIES Boost::convert Boost::included_unit_test_framework)
    boost_test(TYPE run NAME convert_test_strtol_converter SOURCES strtol_converter.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_lcast_converter SOURCES lcast_converter.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_encryption SOURCES encryption.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_user_type SOURCES user_type.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_str_to_int SOURCES str_to_int.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_sfinae SOURCES sfinae.cpp LINK_LIBRARIES Boost::convert)
    boost_test(TYPE run NAME convert_test_has_member SOURCES has_member.cpp LINK_LIBRARIES Boost::convert)

    # these tests require C++17
    boost_test(TYPE run NAME convert_test_charconv_converter SOURCES charconv_converter.cpp LINK_LIBRARIES Boost::convert Boost::included_unit_test_framework COMPILE_FEATURES cxx_std_17)
endif()
