ifneq ($(DEVMAKE),)

BOOST_ROOT = $(HOME)/dev/boost_1_55_0

cflags  = -O3 -Iinclude -I$(BOOST_ROOT) -Wno-unused-local-typedefs -Wno-unused-variable -Wno-uninitialized
target  = test-convert.exe
sources = $(wildcard test/*.cpp) $(wildcard example/*.cpp)

include $(DEVMAKE)/makefile

else

all: $(wildcard test/*.cpp) $(wildcard example/*.cpp) 
	g++ $^ -I./include -I../boost_1_55_0 -lrt

endif

