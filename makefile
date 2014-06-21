ifneq ($(DEVMAKE),)

BOOST_ROOT = $(HOME)/dev/boost_1_55_0

cflags    = -Iinclude -I$(BOOST_ROOT) -Wno-unused-local-typedefs -Wno-unused-variable -Wno-uninitialized
target_1  = convert-test.exe
sources_1 = $(wildcard test/*.cpp)
target_2  = convert-example.exe
sources_2 = $(wildcard example/*.cpp)

include $(DEVMAKE)/makefile

else

all: convert-test convert-example 

convert-test: $(wildcard test/*.cpp) 
	g++ -O3 -o $@ $^ -I./include -I../boost_1_55_0 -lrt

convert-example: $(wildcard example/*.cpp) 
	g++ -O3 -o $@ $^ -I./include -I../boost_1_55_0 -lrt

endif

