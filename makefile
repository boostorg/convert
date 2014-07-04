BOOST_ROOT = $(HOME)/dev/boost

cflags    = -Iinclude -I$(BOOST_ROOT) -Wno-unused-local-typedefs -Wno-unused-variable -Wno-uninitialized
ldflags   = -L$(HOME)/dev/lib -lboost_timer -lboost_system -lboost_chrono
target_1  = convert-test.exe
sources_1 = test/callable.cpp			\
            test/encryption.cpp			\
            test/fallbacks.cpp			\
            test/is_converter.cpp		\
            test/lcast_converter.cpp	\
            test/main.cpp				\
            test/printf_converter.cpp	\
            test/scratchpad.cpp			\
            test/sfinae.cpp				\
            test/stream_converter.cpp	\
            test/strtol_converter.cpp	\
            test/spirit_converter.cpp	\
            test/user_type.cpp

target_2 = convert-test-performance.exe
target_4 = convert-example-algorithms
target_5 = convert-example-default_converter
target_6 = convert-example-getting_serious
target_7 = convert-example-getting_started
target_8 = convert-example-lexical_cast
target_9 = convert-example-stream

sources_2 = test/performance.cpp
sources_4 = example/algorithms.cpp
sources_5 = example/default_converter.cpp
sources_6 = example/getting_serious.cpp
sources_7 = example/getting_started.cpp
sources_8 = example/lexical_cast.cpp
sources_9 = example/stream.cpp

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

