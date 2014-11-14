BOOST_ROOT = $(HOME)/dev/boost

cflags    = -O3 -Iinclude -I$(BOOST_ROOT) -Wno-unused-local-typedefs -Wno-unused-variable -Wno-uninitialized
ldflags   = -L$(HOME)/dev/lib -lboost_timer -lboost_system -lboost_chrono -lrt
target_1  = convert-test.exe
sources_1 = test/encryption.cpp			\
            test/lcast_converter.cpp	\
            test/main.cpp				\
            test/printf_converter.cpp	\
            test/stream_converter.cpp	\
            test/strtol_converter.cpp	\
            test/spirit_converter.cpp	\
            test/user_type.cpp

target_2  = convert-test-has-memfun.exe
sources_2 = test/has_memfun.cpp

target_3  = convert-test-callable.exe
sources_3 = test/callable.cpp

target_4  = convert-test-is-converter.exe
sources_4 = test/is_converter.cpp

target_5  = convert-test-is-fallbacks.exe
sources_5 = test/fallbacks.cpp

target_21 = convert-test-performance.exe
target_22 = convert-test-performance-spirit.exe
target_23 = convert-example-algorithms
target_24 = convert-example-default_converter
target_25 = convert-example-getting_serious
target_26 = convert-example-getting_started
target_27 = convert-example-lexical_cast
target_28 = convert-example-stream

sources_21 = test/performance.cpp
sources_22 = test/performance_spirit.cpp
sources_23 = example/algorithms.cpp
sources_24 = example/default_converter.cpp
sources_25 = example/getting_serious.cpp
sources_26 = example/getting_started.cpp
sources_27 = example/lexical_cast.cpp
sources_28 = example/stream.cpp

ifneq ($(DEVMAKE),)

include $(DEVMAKE)/makefile

else

all: convert-test convert-test-performance convert-test-performance-spirit

convert-test-performance-spirit: test/performance_spirit.cpp
	g++ -O3 -o $@ $^ -Iinclude -I$(BOOST_ROOT) $(ldflags)

convert-test-performance:
	g++ -O3 -o $@ $(sources_2) -Iinclude -I$(BOOST_ROOT) $(ldflags)

convert-test:  
	g++ -O3 -o $@ $(sources_1) -Iinclude -I$(BOOST_ROOT)

endif

