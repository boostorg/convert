ifneq ($(DEVMAKE),)

BOOST_ROOT = $(HOME)/dev/boost_1_55_0

cflags    = -Iinclude -I$(BOOST_ROOT) -Wno-unused-local-typedefs -Wno-unused-variable -Wno-uninitialized
ldflags   = -lboost_timer -lboost_system
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
            test/user_type.cpp

target_2  = convert-test-performance.exe
sources_2 = test/performance.cpp

target_3  = convert-test-spirit.exe
sources_3 = test/spirit.cpp


target_4 = convert_example_algorithms
target_5 = convert_example_default_converter
target_6 = convert_example_getting_serious
target_7 = convert_example_getting_started
target_8 = convert_example_lexical_cast
target_9 = convert_example_stream

sources_4 = example/algorithms.cpp
sources_5 = example/default_converter.cpp
sources_6 = example/getting_serious.cpp
sources_7 = example/getting_started.cpp
sources_8 = example/lexical_cast.cpp
sources_9 = example/stream.cpp

include $(DEVMAKE)/makefile

else

all: convert-test 

convert-test:  
	g++ -O3 -o $@ $(wildcard test/*.cpp) -I./include -I../boost_1_55_0 -lboost_timer -lboost_system

endif

