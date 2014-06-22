ifneq ($(DEVMAKE),)

BOOST_ROOT = $(HOME)/dev/boost_1_55_0

cflags    = -Iinclude -I$(BOOST_ROOT) -Wno-unused-local-typedefs -Wno-unused-variable -Wno-uninitialized
target_1  = convert-test.exe
sources_1 = $(wildcard test/*.cpp)

target_2 = convert_example_algorithms
target_3 = convert_example_default_converter
target_4 = convert_example_getting_serious
target_5 = convert_example_getting_started
target_6 = convert_example_lexical_cast
target_7 = convert_example_stream

sources_2 = example/algorithms.cpp
sources_3 = example/default_converter.cpp
sources_4 = example/getting_serious.cpp
sources_5 = example/getting_started.cpp
sources_6 = example/lexical_cast.cpp
sources_7 = example/stream.cpp

include $(DEVMAKE)/makefile

else

all: convert-test convert-example 

convert-test: $(wildcard test/*.cpp) 
	g++ -O3 -o $@ $^ -I./include -I../boost_1_55_0 -lrt

endif

