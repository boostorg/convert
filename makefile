ifneq ($(DEVMAKE),)

BOOST_ROOT = $(HOME)/dev/boost_1_55_0

cflags  = -Iinclude -I$(BOOST_ROOT) -Wno-unused-local-typedefs -Wno-unused-variable -Wno-uninitialized
target  = test-convert.exe
sources = $(wildcard example/*.cpp) $(wildcard test/*.cpp)

include $(DEVMAKE)/makefile

else

all: $(wildcard example/*.cpp) $(wildcard test/*.cpp) 
	g++ $^ -I./include -I../boost_1_55_0 -lrt

endif

